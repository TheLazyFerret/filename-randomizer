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
#include <utility>

namespace fs = std::filesystem;

using namespace hash::sha256;

/// Print the map with pairs of paths with a cute format :3
/// Mostly for debugging.
void utils::print_path_set(const std::set<std::pair<fs::path, fs::path>>& set) {
  for (const auto& iter : set) {
    std::println("{} -> {}", iter.first.string(), iter.second.string());
  }
}

/// Single thread random number generator.
std::uint64_t utils::random_number() noexcept {
  static std::random_device rd;
  static std::mt19937_64 gen(rd());
  static std::uniform_int_distribution<std::uint64_t> distribution(0, UINT64_MAX);
  return distribution(gen);
}

/// Small wrapper around std::filesystem::is_regular_file to work with std::expected.
std::expected<bool, std::error_code> utils::is_regular_file_wrapper(const fs::path& path) {
  std::error_code ec;
  if (fs::is_regular_file(path, ec)) {
    return true;
  } else if (ec.value() != 0) {
    return std::unexpected(ec);
  }
  return false;
}

/// Small wrapper around std::filesystem::is_directort to work with std::expected.
std::expected<bool, std::error_code> utils::is_directory_wrapper(const fs::path& path) {
  std::error_code ec;
  if (fs::is_directory(path, ec)) {
    return true;
  } else if (ec.value() != 0) {
    return std::unexpected(ec);
  }
  return false;
}

/// Return a set with all the paths in a directory. Simply as that.
/// The path can point to anything, including files and directories.
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

/// Rename iteratively all paths in the first value of the pair to the second value.
/// It does not check if there is no collision.
/// Internally just call utils::rename().
std::expected<void, std::error_code> utils::mass_rename(
    const std::set<std::pair<std::filesystem::path, std::filesystem::path>>& set, const RenameOptions ops) {
  // For each pair [old_path, new_path].
  for (const auto& iter : set) {
    const auto result = rename(iter, ops);
    if (!result && ops.nonstop) {
      std::println("Error with file {} ({}). Continuing", iter.first.string(), result.error().message());
      continue;
    } else if (!result && !ops.nonstop) {
      std::println("Error with file {} ({}). Continuing", iter.first.string(), result.error().message());
      return std::unexpected(result.error());
    }
  }
  return {};
}

/// Rename a single pair old_path -> new_path.
std::expected<void, std::error_code> utils::rename(
    const std::pair<std::filesystem::path, std::filesystem::path>& pair, const RenameOptions ops) {
  std::error_code ec;
  const auto& old_path = pair.first;
  const auto& new_path = pair.second;
  if (ops.print) {
    // Print the change.
    std::println("{} -> {}", old_path.string(), new_path.string());
  }
  fs::rename(old_path, new_path, ec);
  if (ec.value() != 0) {
    return std::unexpected(ec);
  }
  return {};
}

/// Generate an unique path with a randomized filename from the original path.
/// It is garantized that new path will no collision with an existent one in the set.
/// Pass by copy the parameter path and modify it, avoiding copying it inside the function.
/// Probably has non impact in the performance, but idk, looks cooler :)
fs::path utils::generate_random_path(const std::unordered_set<fs::path>& set, fs::path path) {
  assert(set.contains(path));
  // Anonymous function to reduce boilerplate.
  auto gen_name = [](std::string filename, const std::string& extension) -> std::string {
    filename += std::to_string(utils::random_number());
    const auto aux = sha256_to_string(sha256({filename.begin(), filename.end()}));
    return aux + extension;
  };
  // We use only the filename instead of the full path attempting to reduce the number of blocks
  // during the sha256.
  path.replace_filename(gen_name(path.filename().string(), path.extension().string()));
  while (set.contains(path)) {
    std::println("Collision with the hash: {}. Generating a new one.", path.filename().string());
    path.replace_filename(gen_name(path.filename().string(), path.extension().string()));
  }
  return path;
}

/// Get a set of paths with randomized filenames.
/// It is garantized that the new set is the same size as the passed.
/// It is also garantized that the new set has no collision with the original set
///   because internally calls utils::generate_random_path().
std::unordered_set<fs::path> utils::generate_random_path_set(const std::unordered_set<fs::path>& path_set) {
  std::unordered_set<fs::path> randomized_path_set;
  for (const auto& path : path_set) {
    // Inside the function checks the generated path has no collision with the original set.
    auto new_path = utils::generate_random_path(path_set, path);
    while (randomized_path_set.contains(new_path)) {
      std::println("Collision with the hash: {}. Generating a new one.", path.filename().string());
      new_path = utils::generate_random_path(path_set, path);
    }
    randomized_path_set.insert(new_path);
  }
  assert(randomized_path_set.size() == path_set.size());
  return randomized_path_set;
}

/// Set to each old path (key) to a new path (value).
/// Basically just merge two sets (where the second one is equal or bigger than the first one),
///   in a single set made of pairs.
/// Use std::set instead of std::unordered_set because compiler cried about std::pair not being hashable.
std::set<std::pair<fs::path, fs::path>> utils::create_path_pair_set(
    const std::unordered_set<fs::path>& old_path_set, const std::unordered_set<fs::path>& new_path_set) {
  assert(new_path_set.size() >= old_path_set.size());
  std::set<std::pair<fs::path, fs::path>> set;
  auto new_path_set_iter = new_path_set.begin();
  for (const auto& iter : old_path_set) {
    set.insert(std::make_pair(iter, *new_path_set_iter));
    ++new_path_set_iter;
  }
  return set;
}
