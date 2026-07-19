/// Main program file.

#include "utils/utils.hpp"

#include <cstdlib>
#include <print>

int main(/*int argc, char** argv*/) {
  const auto result = utils::paths_in_directory("./test1");
  if (!result) {
    std::println("{}", result.error().message());
    return EXIT_FAILURE;
  }
  const auto result2 = utils::filter_regular_files(result.value());
  if (!result2) {
    std::println("{}", result2.error().message());
    return EXIT_FAILURE;
  }
  const auto random_set = utils::generate_random_path_set(result2.value());
  return EXIT_SUCCESS;
}
