#include "argument_parser.hpp"

#include <cstring>
#include <iostream>

#include <logger/utility.hpp>

namespace test_application {

    std::optional<AppConfig> ArgumentParser::parse_arguments(int argc, char *argv[]) {
        if (argc < 2) {
            print_error("Too few arguments");
            return std::nullopt;
        }

        OptionType option = get_option_type(argv[1]);

        switch (option) {
            case OptionType::HELP:
                return AppConfig(AppConfig::Mode::HELP);

            case OptionType::FILE:
                return parse_file_mode(argv + 2, argc - 2);

            case OptionType::SOCKET:
                return parse_socket_mode(argv + 2, argc - 2);

            case OptionType::UNKNOWN:
            default:
                print_error(std::string("Unknown option: ") + argv[1] + ". Use --file, --socket, or --help");
                return std::nullopt;
        }
    }

    ArgumentParser::OptionType ArgumentParser::get_option_type(const char *arg) {
        if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0) {
            return OptionType::HELP;
        }
        if (strcmp(arg, "--file") == 0) {
            return OptionType::FILE;
        }
        if (strcmp(arg, "--socket") == 0) {
            return OptionType::SOCKET;
        }
        return OptionType::UNKNOWN;
    }

    std::optional<AppConfig> ArgumentParser::parse_file_mode(char *argv[], int remaining) {
        if (remaining < 1) {
            print_error("Missing filename for --file option");
            return std::nullopt;
        }

        AppConfig config(AppConfig::Mode::FILE);
        config.filename = argv[0];

        if (not parse_optional_level(argv + 1, remaining - 1, config)) {
            return std::nullopt;
        }

        return config;
    }

    std::optional<AppConfig> ArgumentParser::parse_socket_mode(char *argv[], int remaining) {
        if (remaining < 2) {
            print_error("Missing host or port for --socket option");
            return std::nullopt;
        }

        AppConfig config(AppConfig::Mode::SOCKET);
        config.host = argv[0];

        try {
            config.port = std::stoi(argv[1]);
        } catch (const std::exception &) {
            print_error(std::string("Invalid port number: ") + argv[1]);
            return std::nullopt;
        }

        if (not is_valid_port(config.port)) {
            print_error("Port number must be between 1 and 65535");
            return std::nullopt;
        }

        if (not parse_optional_level(argv + 2, remaining - 2, config)) {
            return std::nullopt;
        }

        return config;
    }

    bool ArgumentParser::parse_optional_level(char *argv[], int remaining, AppConfig &config) {
        if (remaining == 0) {
            return true; // no more arguments, use default level
        }

        if (strcmp(argv[0], "--level") != 0) {
            print_error(std::string("Unknown argument: ") + argv[0]);
            return false;
        }

        if (remaining < 2) {
            print_error("Missing value for --level option");
            return false;
        }

        auto level = logger::utility::string_to_level(argv[1]);
        if (not level.has_value()) {
            print_error(std::string("Invalid log level: ") + argv[1]);
            return false;
        }

        config.level = level.value();

        // Check for extra arguments
        if (remaining > 2) {
            print_error(std::string("Unexpected argument: ") + argv[2]);
            return false;
        }

        return true;
    }

    bool ArgumentParser::is_valid_port(int port) { return port >= 1 && port <= 65535; }

    void ArgumentParser::print_error(std::string_view message) {
        std::cerr << "Error: " << message << "\n";
        std::cerr << "Use --help for usage information\n";
    }

} // namespace test_application
