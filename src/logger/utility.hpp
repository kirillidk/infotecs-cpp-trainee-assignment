#pragma once

#include <string>
#include <string_view>

#include "log_level.hpp"

namespace logger {
    namespace utility {
        [[nodiscard]] std::string format_message(std::string_view message, logger::LogLevel level);
        [[nodiscard]] std::string level_to_string(logger::LogLevel level);
        [[nodiscard]] std::string get_timestamp();
    } // namespace utility
} // namespace logger
