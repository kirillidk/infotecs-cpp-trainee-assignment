#include "utility.hpp"

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace logger {
    namespace utility {

        std::string format_message(std::string_view message, logger::LogLevel level) {
            std::ostringstream oss;
            oss << "[" << utility::get_timestamp() << "] "
                << "[" << utility::level_to_string(level) << "] " << message;
            return oss.str();
        }

        std::string level_to_string(logger::LogLevel level) {
            using logger::LogLevel;

            switch (level) {
                case LogLevel::DEBUG:
                    return "DEBUG";
                case LogLevel::INFO:
                    return "INFO";
                case LogLevel::WARNING:
                    return "WARNING";
                case LogLevel::ERROR:
                    return "ERROR";
                case LogLevel::FATAL:
                    return "FATAL";
                default:
                    return "UNKNOWN";
            }
        }

        std::optional<logger::LogLevel> string_to_level(std::string level_str) {
            std::transform(level_str.begin(), level_str.end(), level_str.begin(), ::tolower);

            if (level_str == "debug")
                return logger::LogLevel::DEBUG;
            if (level_str == "info")
                return logger::LogLevel::INFO;
            if (level_str == "warning")
                return logger::LogLevel::WARNING;
            if (level_str == "error")
                return logger::LogLevel::ERROR;
            if (level_str == "fatal")
                return logger::LogLevel::FATAL;

            return std::nullopt;
        }

        std::string get_timestamp() {
            auto now = std::chrono::system_clock::now();
            auto ts = std::chrono::floor<std::chrono::seconds>(now);
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - ts).count();

            std::time_t t_c = std::chrono::system_clock::to_time_t(ts);
            std::tm lt = *std::localtime(&t_c);

            std::ostringstream oss;
            oss << std::put_time(&lt, "%Y-%m-%d %H:%M:%S") << '.' << std::setw(6) << std::setfill('0') << ms;

            return oss.str();
        }

    } // namespace utility
} // namespace logger
