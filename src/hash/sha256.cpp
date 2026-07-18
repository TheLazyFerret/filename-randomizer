//! SHA256 implementation.

#include "sha256.hpp"

#include <array>
#include <cstdint>
#include <format>
#include <string>
#include <vector>

using namespace hash::sha256::auxiliar_methods;
using namespace hash::sha256::functions;
using namespace hash::sha256::constants;

/// Return the size (in bits) of the original message (in big endiam).
std::array<std::uint8_t, 8> hash::sha256::auxiliar_methods::get_size(const std::size_t msg_length) noexcept {
  const std::uint64_t bit_length = static_cast<std::uint64_t>(msg_length * 8);
  std::array<std::uint8_t, 8> size_splitted;
  size_splitted.fill(0);
  for (std::size_t i = 0; i < size_splitted.size(); ++i) {
    const std::size_t pos = (size_splitted.size() - 1) - i;
    size_splitted[pos] = static_cast<std::uint8_t>(bit_length >> (8 * i));
  }
  return size_splitted;
}

/// Add the size (as bytes in big endiam) at the end of the block.
void hash::sha256::auxiliar_methods::append_size(
    std::array<std::uint8_t, 64>& block, const std::array<std::uint8_t, 8>& size) noexcept {
  for (std::size_t i = 1; i <= size.size(); ++i) {
    block[block.size() - i] = size[size.size() - i];
  }
}

/// Calculate the next block from the message.
std::array<std::uint8_t, 64> hash::sha256::auxiliar_methods::get_block(
    const std::vector<std::uint8_t>& msg, std::size_t& count, bool& appended, bool& finished) noexcept {
  std::array<std::uint8_t, 64> block;
  block.fill(0);
  std::size_t bytes_readed = 0;
  for (std::size_t i = 0; i < block.size(); ++i) {
    if (count + i >= msg.size()) break;
    block[i] = msg[count + i];
    bytes_readed += 1;
  }
  count += bytes_readed;
  if (bytes_readed < block.size() && !appended) {
    block[bytes_readed] = 0x80;
    appended = true;
  }
  if (bytes_readed < block.size() - 8) {
    append_size(block, get_size(msg.size()));
    finished = true;
  }
  return block;
}

/// First step of the algorithm.
void hash::sha256::auxiliar_methods::prepare_schedule(
    std::array<std::uint32_t, 64>& schedule, const std::array<std::uint8_t, 64>& block) noexcept {
  // Merge the bytes and add them at the begin of the schedule message.
  // Basically add the block to the first 16 [0-15] of the schedule.
  for (std::size_t i = 0; i < 16; ++i) {
    const std::size_t base_index = 4 * i;
    schedule[i] = (static_cast<std::uint32_t>(block[base_index]) << 24) |
                  (static_cast<std::uint32_t>(block[base_index + 1]) << 16) |
                  (static_cast<std::uint32_t>(block[base_index + 2]) << 8) |
                  (static_cast<std::uint32_t>(block[base_index + 3]));
  }

  // Calculate the rest of the schedule message.
  for (std::size_t i = 16; i < schedule.size(); ++i) {
    schedule[i] = lower_sigma1(schedule[i - 2]) + schedule[i - 7] + lower_sigma0(schedule[i - 15]) + schedule[i - 16];
  }
}

/// Second step of the algorithm.
std::array<std::uint32_t, 8> hash::sha256::auxiliar_methods::compute_intermediate_hash(
    const std::array<std::uint32_t, 64>& schedule, std::array<std::uint32_t, 8> hash) noexcept {
  for (std::size_t i = 0; i < schedule.size(); ++i) {
    const std::uint32_t t1 =
        hash[7] + upper_sigma1(hash[4]) + choose(hash[4], hash[5], hash[6]) + HASH_CONSTANTS[i] + schedule[i];
    const std::uint32_t t2 = upper_sigma0(hash[0]) + majority(hash[0], hash[1], hash[2]);
    hash[7] = hash[6];
    hash[6] = hash[5];
    hash[5] = hash[4];
    hash[4] = hash[3] + t1;
    hash[3] = hash[2];
    hash[2] = hash[1];
    hash[1] = hash[0];
    hash[0] = t1 + t2;
  }
  return hash;
}

/// Third step of the algorithm.
std::array<std::uint32_t, 8> hash::sha256::auxiliar_methods::add_hashes(
    const std::array<std::uint32_t, 8>& a, const std::array<std::uint32_t, 8>& b) noexcept {
  return {a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3], a[4] + b[4], a[5] + b[5], a[6] + b[6], a[7] + b[7]};
}

/// Main function, calculates the sha256 from a vector of bytes.
std::array<std::uint32_t, 8> hash::sha256::sha256(const std::vector<std::uint8_t>& msg) noexcept {
  // Required block flags.
  bool appended = false;
  bool finished = false;
  std::size_t count = 0;

  std::array<std::uint32_t, 8> hash = constants::INITIAL_HASH;
  std::array<std::uint32_t, 64> message_schedule;

  while (!finished) {
    prepare_schedule(message_schedule, get_block(msg, count, appended, finished));
    hash = add_hashes(hash, compute_intermediate_hash(message_schedule, hash));
  }
  return hash;
}

/// convert the numerical hash into a string.
std::string hash::sha256::sha256_to_string(const std::array<std::uint32_t, 8>& hash) noexcept {
  return std::format(
      "{:0>8x}{:0>8x}{:0>8x}{:0>8x}{:0>8x}{:0>8x}{:0>8x}{:0>8x}", hash[0], hash[1], hash[2], hash[3], hash[4], hash[5],
      hash[6], hash[7]);
}
