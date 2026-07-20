//! Methods used by the program.

#pragma once

#include <cstdint>
#include <expected>
#include <filesystem>
#include <system_error>
#include <unordered_map>
#include <unordered_set>

namespace utils {

std::uint64_t random_number() noexcept;

std::expected<bool, std::error_code> is_regular_file_wrapper(const std::filesystem::path&);
std::expected<bool, std::error_code> is_directory_wrapper(const std::filesystem::path&);

std::expected<std::unordered_set<std::filesystem::path>, std::error_code> paths_in_directory(
    const std::filesystem::path&);

std::expected<std::unordered_set<std::filesystem::path>, std::error_code> filter_regular_files(
    const std::unordered_set<std::filesystem::path>&);

std::expected<void, std::error_code> mass_rename(
    const std::unordered_map<std::filesystem::path, std::filesystem::path>&);

std::filesystem::path generate_random_path(const std::unordered_set<std::filesystem::path>&, std::filesystem::path);

std::unordered_set<std::filesystem::path> generate_random_path_set(const std::unordered_set<std::filesystem::path>&);

} // namespace utils
