//! Methods used by the program.

#pragma once

#include <cstdint>
#include <expected>
#include <filesystem>
#include <system_error>
#include <unordered_set>

namespace utils {

std::uint64_t random_number() noexcept;

std::expected<std::unordered_set<std::filesystem::path>, std::error_code> files_in_directory(
    const std::filesystem::path&);

std::unordered_set<std::filesystem::path> generate_randomized_paths(const std::unordered_set<std::filesystem::path>&);

std::expected<void, std::error_code> randmize_filename();

} // namespace utils
