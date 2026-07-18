//! SHA256 quick implementation.

#pragma once

#include <array>
#include <bit>
#include <cstdint>
#include <string>
#include <vector>

namespace hash::sha256 {

std::array<std::uint32_t, 8> sha256(const std::vector<std::uint8_t>&) noexcept;
std::string sha256_to_string(const std::array<std::uint32_t, 8>&) noexcept;

} // namespace hash::sha256

namespace hash::sha256::auxiliar_methods {

std::array<std::uint8_t, 8> get_size(const std::size_t) noexcept;
void append_size(std::array<std::uint8_t, 64>&, const std::array<std::uint8_t, 8>&) noexcept;

std::array<std::uint8_t, 64> get_block(const std::vector<std::uint8_t>&, std::size_t&, bool&, bool&) noexcept;

void copy_block_to_schedule(std::array<std::uint32_t, 64>&, const std::array<std::uint8_t, 64>&) noexcept;

void prepare_schedule(std::array<std::uint32_t, 64>&, const std::array<std::uint8_t, 64>&) noexcept;

std::array<std::uint32_t, 8> compute_intermediate_hash(
    const std::array<std::uint32_t, 64>&, std::array<std::uint32_t, 8>) noexcept;

std::array<std::uint32_t, 8> add_hashes(
    const std::array<std::uint32_t, 8>&, const std::array<std::uint32_t, 8>&) noexcept;

} // namespace hash::sha256::auxiliar_methods

namespace hash::sha256::functions {

inline constexpr std::uint32_t choose(std::uint32_t x, std::uint32_t y, std::uint32_t z) noexcept {
  return (x & y) ^ (~x & z);
}

inline constexpr std::uint32_t majority(std::uint32_t x, std::uint32_t y, std::uint32_t z) noexcept {
  return (x & y) ^ (x & z) ^ (y & z);
}

inline constexpr std::uint32_t upper_sigma0(std::uint32_t x) noexcept {
  return std::rotr(x, 2) ^ std::rotr(x, 13) ^ std::rotr(x, 22);
}

inline constexpr std::uint32_t upper_sigma1(std::uint32_t x) noexcept {
  return std::rotr(x, 6) ^ std::rotr(x, 11) ^ std::rotr(x, 25);
}

inline constexpr std::uint32_t lower_sigma0(std::uint32_t x) noexcept {
  return std::rotr(x, 7) ^ std::rotr(x, 18) ^ (x >> 3);
}

inline constexpr std::uint32_t lower_sigma1(std::uint32_t x) noexcept {
  return std::rotr(x, 17) ^ std::rotr(x, 19) ^ (x >> 10);
}

} // namespace hash::sha256::functions

// clang-format off
namespace hash::sha256::constants {

constexpr std::array<std::uint32_t, 8> INITIAL_HASH = {
  0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
  0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

constexpr std::array<std::uint32_t, 64> HASH_CONSTANTS = {
  0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
  0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
  0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
  0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
  0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

} // namespace hash::constants
// clang-format on
