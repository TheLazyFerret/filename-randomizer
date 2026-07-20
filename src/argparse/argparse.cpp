//! Implementation of the argument parser.

#include "argparse/argparse.hpp"

#include <string>
#include <vector>

using namespace argparse;

/// Convert the program c-like arguments in a more convenient format.
std::vector<std::string> argparse::argv_to_vec(int argc, char** argv) {
  std::vector<std::string> args;
  args.reserve(argc);
  for (int i = 1; i < argc; ++i) {
    args.push_back(argv[i]);
  }
  return args;
}

/// Parse the arguments.
std::expected<ParsedArgs, ParseError> argparse::parse_args(const std::vector<std::string>& args) {
  ParsedArgs parsed_args;
  for (const auto& arg : args) {
    if (arg == "-h" || arg == "--help") {
      parsed_args.help = true;
    } else if (arg == "-v" || arg == "--version") {
      parsed_args.version = true;
    } else {
      return std::unexpected<ParseError>({arg, ParseErrorType::UnkownOption});
    }
  }
  return parsed_args;
}
