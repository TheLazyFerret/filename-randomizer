//! Methods used by the program.

#pragma once

#include <expected>
#include <filesystem>
#include <system_error>
#include <unordered_set>

namespace utils {

std::expected<std::unordered_set<std::filesystem::path>, std::error_code> files_in_directory(
    const std::filesystem::path&);

}
