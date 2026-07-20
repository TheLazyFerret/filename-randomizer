/// Main program file.

#include "utils/utils.hpp"
#include "argparse/argparse.hpp"

#include <cstdlib>
#include <print>

int main(int argc, char** argv) {
  const auto args_result = argparse::parse_args(argparse::argv_to_vec(argc, argv));
  if (!args_result) {
    std::println("{}", argparse::format_parse_error(args_result.error()));
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
