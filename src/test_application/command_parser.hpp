#pragma once

#include <optional>
#include <string>
#include <vector>

#include "utility.hpp"

namespace test_application {
    enum class CommandType { LOG_MESSAGE, HELP, EXIT };

    struct ParsedCommand {
        CommandType type;
        std::string message;
        std::optional<logger::LogLevel> level;

        ParsedCommand(CommandType t) : type(t) {}
        ParsedCommand(CommandType t, std::string msg) : type(t), message(std::move(msg)) {}
        ParsedCommand(CommandType t, std::string msg, logger::LogLevel lvl) :
            type(t), message(std::move(msg)), level(lvl) {}
    };

    class CommandParser {
    public:
        static std::optional<ParsedCommand> parse_command(const std::string &input);
        static std::optional<logger::LogLevel> parse_level(const std::string &level_str);

    private:
        static std::string trim(const std::string &str);
        static std::vector<std::string> split(const std::string &str, char delimiter);
    };
} // namespace test_application
