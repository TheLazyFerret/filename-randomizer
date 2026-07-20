//! Program argument parser.

#pragma once

#include <expected>
#include <string>
#include <vector>

namespace argparse {

enum class ParseErrorType { UnkownOption };

struct ParseError {
  std::string message;
  ParseErrorType type;
};

struct ParsedArgs {
  bool help = false;
  bool version = false;
  bool nonstop = false;
};

std::vector<std::string> argv_to_vec(int argc, char** argv);
std::expected<ParsedArgs, ParseError> parse_args(const std::vector<std::string>&);

} // namespace argparse
