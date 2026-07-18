/// Main program file.

#include "utils/utils.hpp"

#include <cstdlib>
#include <print>

int main(/*int argc, char** argv*/) {
  const auto result = utils::files_in_directory("invalid_dir");
  if (!result) {
    std::println("{}", result.error().message());
    return EXIT_FAILURE;
  }
  for (const auto& path : result.value()) {
    std::println("{}", path.string());
  }
  return EXIT_SUCCESS;
}
