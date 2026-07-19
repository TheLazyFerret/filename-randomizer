/// Main program file.

#include "utils/utils.hpp"

#include <cstdlib>
#include <print>

int main(/*int argc, char** argv*/) {
  const auto result = utils::files_in_directory("test1");
  if (!result) {
    std::println("{}", result.error().message());
    return EXIT_FAILURE;
  }
  const auto result2 = utils::generate_randomized_paths(result.value());

  return EXIT_SUCCESS;
}
