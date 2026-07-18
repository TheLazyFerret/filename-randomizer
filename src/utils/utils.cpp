//! Implementation of utils.

#include "utils.hpp"

#include <expected>
#include <filesystem>
#include <system_error>
#include <unordered_set>

namespace fs = std::filesystem;

/// Return a set with the paths to all the regular files in a directory.
std::expected<std::unordered_set<fs::path>, std::error_code> utils::files_in_directory(
    const std::filesystem::path& path) {
  std::error_code ec;
  std::unordered_set<fs::path> path_set;
  // Check if the path is a directory.
  if (!fs::is_directory(path, ec)) {
    return std::unexpected(std::make_error_code(std::errc::not_a_directory));
  } else if (ec.value() != 0) {
    return std::unexpected(ec);
  }
  const fs::directory_iterator dir_iterator(path, ec);
  if (ec.value() != 0) {
    return std::unexpected(ec);
  }
  for (const auto& dir_entry : dir_iterator) {
    if (dir_entry.is_regular_file(ec)) {
      path_set.insert(dir_entry.path());
    } else if (ec.value() != 0) {
      return std::unexpected(ec);
    }
  }
  return {path_set};
}
