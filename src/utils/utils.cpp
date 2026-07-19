//! Implementation of utils.

#include "utils.hpp"
#include "hash/sha256.hpp"

#include <cassert>
#include <cstdint>
#include <expected>
#include <filesystem>
#include <print>
#include <random>
#include <system_error>
#include <unordered_set>

namespace fs = std::filesystem;

using namespace hash::sha256;

/// Single thread random number generator.
std::uint64_t utils::random_number() noexcept {
  static std::random_device rd;
  static std::mt19937_64 gen(rd());
  static std::uniform_int_distribution<std::uint64_t> distribution(0, UINT64_MAX);
  return distribution(gen);
}

/// Return a set with the paths in a directory.
std::expected<std::unordered_set<fs::path>, std::error_code> utils::paths_in_directory(
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
    path_set.insert(dir_entry.path());
  }
  return {path_set};
}

/// From a set of paths, return a new set with only the valid regular files.
std::expected<std::unordered_set<fs::path>, std::error_code> utils::filter_regular_files(
    const std::unordered_set<fs::path>& set) {
  std::unordered_set<fs::path> filtered_set;
  std::error_code ec;
  for (const auto& path : set) {
    if (fs::is_regular_file(path, ec)) {
      filtered_set.insert(path);
    } else if (ec.value() != 0) {
      return std::unexpected(ec);
    }
  }
  return filtered_set;
}

/// Rename all the paths from og_path, to a not deterministic from new_path.
std::expected<void, std::error_code> utils::mass_rename(const std::unordered_map<fs::path, fs::path>& map) {
  std::error_code ec;
  for (const auto& iter : map) {
    const auto& old_path = iter.first;
    const auto& new_path = iter.second;
    fs::rename(old_path, new_path, ec);
    if (ec.value() != 0) {
      return std::unexpected(ec);
    }
  }
  return {};
}

/// Generate an unique path with a randomized filename from the original path.
/// It is garantized that new path will no collision with an existent one in the set.
fs::path utils::generate_random_path(const std::unordered_set<fs::path>& set, fs::path path) {
  assert(set.contains(path));
  const auto gen_name = [](std::string filename, const std::string& extension) -> std::string {
    filename += std::to_string(utils::random_number());
    const auto aux = sha256_to_string(sha256({filename.begin(), filename.end()}));
    return aux + extension;
  };
  while (set.contains(path)) {
    path.replace_filename(gen_name(path.filename().string(), path.extension().string()));
  }
  return path;
}

/// Get a set of paths with randomized filenames.
/// It is garantized that the new set is the same size as the passed.
/// It is also garantized that the new set has no collision with the original set.
std::unordered_set<fs::path> utils::generate_random_path_set(const std::unordered_set<fs::path>& path_set) {
  std::unordered_set<fs::path> randomized_path_set;
  for (const auto& path : path_set) {
    // Inside the function checks the generated path has no collision with the original set.
    auto new_path = utils::generate_random_path(path_set, path);
    while (randomized_path_set.contains(new_path)) {
      new_path = utils::generate_random_path(path_set, path);
    }
    std::println("{} -> {}", path.string(), new_path.string());
    randomized_path_set.insert(new_path);
  }
  assert(randomized_path_set.size() == path_set.size());
  return randomized_path_set;
}
