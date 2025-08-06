#pragma once

#include <string>
#include <string_view>

namespace logger {
    enum LogLevel { DEBUG = 0, INFO = 1, WARNING = 2, ERROR = 3, FATAL = 4 };
}

namespace utility {
    [[nodiscard]] std::string format_message(std::string_view message, logger::LogLevel level);
    [[nodiscard]] std::string level_to_string(logger::LogLevel level);
    [[nodiscard]] std::string get_timestamp();
} // namespace utility
