//! Methods used by the program.

#pragma once

#include <expected>
#include <filesystem>
#include <set>
#include <system_error>
#include <unordered_set>
#include <utility>

namespace utils {

void print_path_set(const std::set<std::pair<std::filesystem::path, std::filesystem::path>>&);

std::set<std::pair<std::filesystem::path, std::filesystem::path>> create_path_pair_set(
    const std::unordered_set<std::filesystem::path>&, const std::unordered_set<std::filesystem::path>&);

std::uint64_t random_number() noexcept;

std::expected<bool, std::error_code> is_regular_file_wrapper(const std::filesystem::path&);
std::expected<bool, std::error_code> is_directory_wrapper(const std::filesystem::path&);

std::expected<std::unordered_set<std::filesystem::path>, std::error_code> paths_in_directory(
    const std::filesystem::path&);

std::expected<std::unordered_set<std::filesystem::path>, std::error_code> filter_regular_files(
    const std::unordered_set<std::filesystem::path>&);

struct RenameOptions {
  bool nonstop = false;
  bool print = false;
};

std::expected<void, std::error_code> mass_rename(
    const std::set<std::pair<std::filesystem::path, std::filesystem::path>>&, const RenameOptions);

std::expected<void, std::error_code> rename(const std::pair<std::filesystem::path, std::filesystem::path>&, const RenameOptions);

std::filesystem::path generate_random_path(const std::unordered_set<std::filesystem::path>&, std::filesystem::path);

std::unordered_set<std::filesystem::path> generate_random_path_set(const std::unordered_set<std::filesystem::path>&);

} // namespace utils
