#include "command_parser.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

#include <logger/utility.hpp>

#include "utility.hpp"

namespace test_application {
    std::optional<ParsedCommand> CommandParser::parse_command(const std::string &input) {
        std::string trimmed_input = trim(input);

        if (trimmed_input.empty()) {
            return std::nullopt;
        }

        if (trimmed_input == "exit" || trimmed_input == "quit") {
            return ParsedCommand(CommandType::EXIT);
        }

        if (trimmed_input == "help") {
            return ParsedCommand(CommandType::HELP);
        }

        std::vector<std::string> tokens = split(trimmed_input, ' ');
        if (tokens.empty()) {
            return std::nullopt;
        }

        std::string level_str = tokens[tokens.size() - 1];
        std::optional<logger::LogLevel> level = parse_level(level_str);

        if (level.has_value() && tokens.size() > 1) {
            std::string message = trimmed_input.substr(0, trimmed_input.rfind(' '));
            return ParsedCommand(CommandType::LOG_MESSAGE, message, level.value());
        } else {
            return ParsedCommand(CommandType::LOG_MESSAGE, trimmed_input);
        }
    }

    std::optional<logger::LogLevel> CommandParser::parse_level(const std::string &level_str) {
        std::string lower_level = level_str;
        std::transform(lower_level.begin(), lower_level.end(), lower_level.begin(), ::tolower);

        if (lower_level == "debug")
            return logger::LogLevel::DEBUG;
        if (lower_level == "info")
            return logger::LogLevel::INFO;
        if (lower_level == "warning" || lower_level == "warn")
            return logger::LogLevel::WARNING;
        if (lower_level == "error")
            return logger::LogLevel::ERROR;
        if (lower_level == "fatal")
            return logger::LogLevel::FATAL;

        return std::nullopt;
    }


    std::string CommandParser::trim(const std::string &str) {
        size_t start = str.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) {
            return {};
        }
        size_t end = str.find_last_not_of(" \t\r\n");
        return str.substr(start, end - start + 1);
    }

    std::vector<std::string> CommandParser::split(const std::string &str, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;

        while (std::getline(ss, token, delimiter)) {
            token = trim(token);
            if (!token.empty()) {
                tokens.push_back(token);
            }
        }

        return tokens;
    }
} // namespace test_application
