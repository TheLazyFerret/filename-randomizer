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

/// Get a set of paths with randomized filenames.
/// It is garantized that the new set is the same size as the passed.
/// It is also garantized that the new set has no collision with the original set.
std::unordered_set<fs::path> utils::generate_randomized_paths(const std::unordered_set<fs::path>& path_set) {
  std::unordered_set<fs::path> randomized_path_set;
  for (const auto& path : path_set) {
    // Generate a new randomized filename.
    auto randomized_filename = [&]() {
      const auto original_filename = path.filename().string();
      const auto aux = sha256_to_string(sha256({original_filename.begin(), original_filename.end()}));
      return aux + path.extension().string();
    }();
    // Create the new path with the randomized filename.
    auto new_path = path;
    new_path.replace_filename(randomized_filename);
    // Collision detection. Generate a new random name if there is a collision.
    while (randomized_path_set.contains(new_path) || path_set.contains(new_path)) {
      randomized_filename += std::to_string(utils::random_number()); // Increase randomization.
      randomized_filename = sha256_to_string(sha256({randomized_filename.begin(), randomized_filename.end()}));
      randomized_filename += path.extension().string();
      new_path.replace_filename(randomized_filename);
    }
    std::println("{} -> {}", path.string(), new_path.string());
    randomized_path_set.insert(new_path);
  }
  assert(randomized_path_set.size() == path_set.size());
  return randomized_path_set;
}

/// Main method of the program.
std::expected<void, std::error_code> utils::randmize_filename() { return {}; }
