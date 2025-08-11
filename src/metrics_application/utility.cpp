#include "utility.hpp"

#include <iostream>

#include <logger/utility.hpp>

namespace metrics_application {
    namespace utility {
        void print_usage(const char *program_name) {
            std::cout << "Usage: " << program_name << " <host> <port> <N> <T>\n\n";
            std::cout << "Parameters:\n";
            std::cout << "  host - Host address to bind to (e.g., 127.0.0.1, 0.0.0.0)\n";
            std::cout << "  port - Port number to listen on\n";
            std::cout << "  N    - Print stats after every N messages\n";
            std::cout << "  T    - Print stats after T seconds of timeout (if stats changed)\n\n";
            std::cout << "Examples:\n";
            std::cout << "  " << program_name << " 127.0.0.1 9000 10 30\n";
            std::cout << "    (Print stats every 10 messages or after 30 seconds of inactivity)\n";
        }

        std::optional<logger::LogLevel> parse_level_from_log(std::string_view log_message) {
            // Expected format: [timestamp] [LEVEL] message
            // Find first bracket (timestamp)
            size_t first_bracket = log_message.find('[');
            if (first_bracket == std::string_view::npos) {
                return std::nullopt;
            }

            // Find closing bracket of timestamp
            size_t first_closing = log_message.find(']', first_bracket);
            if (first_closing == std::string_view::npos) {
                return std::nullopt;
            }

            // Find second bracket (level)
            size_t second_bracket = log_message.find('[', first_closing);
            if (second_bracket == std::string_view::npos) {
                return std::nullopt;
            }

            // Find closing bracket of level
            size_t second_closing = log_message.find(']', second_bracket);
            if (second_closing == std::string_view::npos) {
                return std::nullopt;
            }

            // Extract level string
            size_t level_start = second_bracket + 1;
            size_t level_length = second_closing - level_start;
            std::string level_str(log_message.substr(level_start, level_length));

            // Convert to LogLevel
            return logger::utility::string_to_level(level_str);
        }

        std::optional<std::string> parse_message_from_log(std::string_view log_message) {
            // Expected format: [timestamp] [LEVEL] message
            // Find first bracket (timestamp)
            size_t first_bracket = log_message.find('[');
            if (first_bracket == std::string_view::npos) {
                return std::nullopt;
            }

            // Find closing bracket of timestamp
            size_t first_closing = log_message.find(']', first_bracket);
            if (first_closing == std::string_view::npos) {
                return std::nullopt;
            }

            // Find second bracket (level)
            size_t second_bracket = log_message.find('[', first_closing);
            if (second_bracket == std::string_view::npos) {
                return std::nullopt;
            }

            // Find closing bracket of level
            size_t second_closing = log_message.find(']', second_bracket);
            if (second_closing == std::string_view::npos) {
                return std::nullopt;
            }

            // Extract message (everything after "[LEVEL] ")
            size_t message_start = second_closing + 1;

            // Skip whitespace after the closing bracket
            while (message_start < log_message.length() && std::isspace(log_message[message_start])) {
                message_start++;
            }

            if (message_start >= log_message.length()) {
                return std::string(); // Empty message
            }

            return std::string(log_message.substr(message_start));
        }
    } // namespace utility
} // namespace metrics_application
