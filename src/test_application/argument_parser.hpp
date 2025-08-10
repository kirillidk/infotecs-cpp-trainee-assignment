#pragma once

#include <optional>
#include <string>
#include <vector>

#include <logger/log_level.hpp>

namespace test_application {
    struct AppConfig {
        enum class Mode { FILE, SOCKET, HELP } mode;

        // FILE mode parameters
        std::string filename;

        // SOCKET mode parameters
        std::string host;
        int port = 0;

        // Common parameters
        logger::LogLevel level = logger::LogLevel::INFO;

        AppConfig(Mode m) : mode(m) {}
    };

    class ArgumentParser {
    public:
        static std::optional<AppConfig> parse_arguments(int argc, char *argv[]);

    private:
        enum class OptionType { FILE, SOCKET, HELP, UNKNOWN };

        static OptionType get_option_type(const char *arg);
        static std::optional<AppConfig> parse_file_mode(char *argv[], int remaining);
        static std::optional<AppConfig> parse_socket_mode(char *argv[], int remaining);
        static bool parse_optional_level(char *argv[], int remaining, AppConfig &config);

        static bool is_valid_port(int port);
        static void print_error(std::string_view message);
    };
} // namespace test_application
