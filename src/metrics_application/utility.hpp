#pragma once

#include <optional>
#include <string_view>

#include <logger/log_level.hpp>

namespace metrics_application {
    namespace utility {
        void print_usage(const char *program_name);

        std::optional<logger::LogLevel> parse_level_from_log(std::string_view log_message);

        std::optional<std::string> parse_message_from_log(std::string_view log_message);
    } // namespace utility
} // namespace metrics_application
