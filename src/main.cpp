/// Main program file.

#include "argparse/argparse.hpp"
#include "utils/utils.hpp"

#include <cstdlib>
#include <expected>
#include <print>
#include <system_error>
#include <utility>

/// Print the version.
void print_version() { std::println("filename-randomizer version: {}", PROJECT_VERSION); }

/// Print the help.
void print_help() {
  std::println("Usage: filename-randomizer <options> path\n \
    -v --version: Print the version of the program and exits.\n \
    -h --help: Print this info and exits.\n \
    -p --print-changes: rint a list of all the changes made.\n \
    -n --nonstop If an error is encountered during the name change, it is ignored and the process continues.\n \
    ");
}

/// Organizational function that do what the program does (:p)
///   if the parameter is a directory (rename all regular files inside it).
std::expected<void, std::error_code> handle_directory(const argparse::ParsedArgs& args) {
  const auto paths_in_directory_result = utils::paths_in_directory(args.path);
  if (!paths_in_directory_result) {
    return std::unexpected(paths_in_directory_result.error());
  }
  const auto& paths_in_directory = paths_in_directory_result.value();
  const auto random_paths_set = utils::generate_random_path_set(paths_in_directory);
  const auto files_in_directory_result = utils::filter_regular_files(paths_in_directory);
  if (!files_in_directory_result) {
    return std::unexpected(files_in_directory_result.error());
  }
  const auto& files_in_directory = files_in_directory_result.value();
  const auto set = utils::create_path_pair_set(files_in_directory, random_paths_set);
  return utils::mass_rename(set, {.nonstop = args.nonstop, .print = args.print_changes});
}

/// Organizational function when the parameter is a file.
std::expected<void, std::error_code> handle_file(const argparse::ParsedArgs& args) {
  const auto absolute_path_result = utils::absolute_wrapper(args.path);
  if (!absolute_path_result) {
    return std::unexpected(absolute_path_result.error());
  }
  const auto paths_in_directory_result = utils::paths_in_directory(absolute_path_result.value().parent_path());
  if (!paths_in_directory_result) {
    return std::unexpected(paths_in_directory_result.error());
  }
  const auto random_path = utils::generate_random_path(paths_in_directory_result.value(), absolute_path_result.value());
  return utils::rename(std::make_pair(args.path, random_path), {.nonstop = args.nonstop, .print = args.print_changes});
}

int main(int argc, char** argv) {
  const auto args_result = argparse::parse_args(argparse::argv_to_vec(argc, argv));
  if (!args_result) {
    std::println("{}", argparse::format_parse_error(args_result.error()));
    return EXIT_FAILURE;
  }
  const auto parsed_args = args_result.value();
  if (parsed_args.help) {
    print_help();
    return EXIT_SUCCESS;
  } else if (parsed_args.version) {
    print_version();
    return EXIT_SUCCESS;
  }
  if (parsed_args.path.empty()) {
    std::println("{}", argparse::format_parse_error({"", argparse::ParseErrorType::NoPath}));
    return EXIT_FAILURE;
  }
  const auto is_directory_result = utils::is_directory_wrapper(parsed_args.path);
  if (!is_directory_result) {
    std::println("{}", is_directory_result.error().message());
    return EXIT_FAILURE;
  } else if (is_directory_result.value()) {
    const auto result = handle_directory(parsed_args);
    if (!result) {
      std::println("{}", result.error().message());
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }
  const auto is_file_result = utils::is_regular_file_wrapper(parsed_args.path);
  if (!is_file_result) {
    std::println("{}", is_file_result.error().message());
    return EXIT_FAILURE;
  } else if (is_file_result.value()) {
    const auto result = handle_file(parsed_args);
    if (!result) {
      std::println("{}", result.error().message());
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }
  // The path is not a file or a directory.
  std::println("Error: Not a file, neither a directory");
  return EXIT_FAILURE;
}
