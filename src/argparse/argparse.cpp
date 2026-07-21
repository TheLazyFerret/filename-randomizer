//! Implementation of the argument parser.

#include "argparse/argparse.hpp"

#include <cassert>
#include <expected>
#include <format>
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

std::expected<void, ParseError> argparse::parse_args_ops(ParsedArgs& parsed_args, const std::string& arg) {
  if (arg == "-h" || arg == "--help") {
    parsed_args.help = true;
  } else if (arg == "-v" || arg == "--version") {
    parsed_args.version = true;
  } else if (arg == "-n" || arg == "--nonstop") {
    parsed_args.nonstop = true;
  } else if (arg == "-p" || arg == "--print-changes") {
    parsed_args.print_changes = true;
  } else {
    return std::unexpected<ParseError>({arg, ParseErrorType::UnkownOption});
  }
  return {};
}

/// Parse the arguments.
std::expected<ParsedArgs, ParseError> argparse::parse_args(const std::vector<std::string>& args) {
  ParsedArgs parsed_args;
  for (const auto& arg : args) {
    if (arg.starts_with("-") || arg.starts_with("--")) {
      const auto res = parse_args_ops(parsed_args, arg);
      if (!res) return std::unexpected(res.error());
    } else {
      if (parsed_args.path.empty()) parsed_args.path = arg;
      else return std::unexpected<ParseError>({arg, ParseErrorType::UnexpectedArgument});
    }
  }
  return parsed_args;
}

/// Return a formated string of the ParseError.
std::string argparse::format_parse_error(const ParseError& err) {
  switch (err.type) {
  case ParseErrorType::UnkownOption:
    return std::format("Unkown argument: {}", err.message);
    break;
  case ParseErrorType::UnexpectedArgument:
    return std::format("Unexpected argument: {}", err.message);
    break;
  case ParseErrorType::NoPath:
    return std::format("No path was suplied");
    break;
  default:
    return "";
  }
}
