/// Main program file.

#include "argparse/argparse.hpp"
#include "utils/utils.hpp"

#include <cstdlib>
#include <expected>
#include <print>
#include <system_error>

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
std::expected<void, std::error_code> handle_file(/*const argparse::ParsedArgs& args*/) {
  // TODO!.
  std::println("Not implemented yet :3");
  return {};
}

int main(int argc, char** argv) {
  const auto args_result = argparse::parse_args(argparse::argv_to_vec(argc, argv));
  if (!args_result) {
    std::println("{}", argparse::format_parse_error(args_result.error()));
    return EXIT_FAILURE;
  }
  const auto parsed_args = args_result.value();
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
    const auto result = handle_file();
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
