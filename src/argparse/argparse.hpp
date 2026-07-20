//! Program argument parser.

#pragma once

#include <expected>
#include <string>
#include <vector>

namespace argparse {

enum class ParseErrorType { UnkownOption, UnexpectedArgument, NoPath };

struct ParseError {
  std::string message;
  ParseErrorType type;
};

struct ParsedArgs {
  bool help = false;
  bool version = false;
  bool nonstop = false;
  bool print_changes = false;
  std::string path = "";
};

std::expected<void, ParseError> parse_args_ops(ParsedArgs&, const std::string&);

std::vector<std::string> argv_to_vec(int argc, char** argv);
std::expected<ParsedArgs, ParseError> parse_args(const std::vector<std::string>&);
std::string format_parse_error(const ParseError&);

} // namespace argparse
