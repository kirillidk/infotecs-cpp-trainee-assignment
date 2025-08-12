#include "argument_parser.hpp"

#include <iostream>

#include <logger/utility.hpp>

namespace test_application {

    std::optional<AppConfig> ArgumentParser::parse_arguments(const std::vector<std::string> &args) {
        if (args.empty()) {
            print_error("Too few arguments");
            return std::nullopt;
        }

        OptionType option = get_option_type(args[0]);

        switch (option) {
            case OptionType::HELP:
                return AppConfig(AppConfig::Mode::HELP);

            case OptionType::FILE:
                return parse_file_mode(args, 1);

            case OptionType::SOCKET:
                return parse_socket_mode(args, 1);

            case OptionType::UNKNOWN:
            default:
                print_error("Unknown option: " + args[0] + ". Use --file, --socket, or --help");
                return std::nullopt;
        }
    }

    ArgumentParser::OptionType ArgumentParser::get_option_type(std::string_view arg) {
        if (arg == "--help" || arg == "-h") {
            return OptionType::HELP;
        }
        if (arg == "--file") {
            return OptionType::FILE;
        }
        if (arg == "--socket") {
            return OptionType::SOCKET;
        }
        return OptionType::UNKNOWN;
    }

    std::optional<AppConfig> ArgumentParser::parse_file_mode(const std::vector<std::string> &args, size_t start_index) {
        if (args.size() <= start_index) {
            print_error("Missing filename for --file option");
            return std::nullopt;
        }

        AppConfig config(AppConfig::Mode::FILE);
        config.filename = args[start_index];

        if (not parse_optional_level(args, start_index + 1, config)) {
            return std::nullopt;
        }

        return config;
    }

    std::optional<AppConfig> ArgumentParser::parse_socket_mode(const std::vector<std::string> &args,
                                                               size_t start_index) {
        if (args.size() <= start_index + 1) {
            print_error("Missing host or port for --socket option");
            return std::nullopt;
        }

        AppConfig config(AppConfig::Mode::SOCKET);
        config.host = args[start_index];

        try {
            config.port = std::stoi(args[start_index + 1]);
        } catch (const std::exception &) {
            print_error("Invalid port number: " + args[start_index + 1]);
            return std::nullopt;
        }

        if (not is_valid_port(config.port)) {
            print_error("Port number must be between 1 and 65535");
            return std::nullopt;
        }

        if (not parse_optional_level(args, start_index + 2, config)) {
            return std::nullopt;
        }

        return config;
    }

    bool ArgumentParser::parse_optional_level(const std::vector<std::string> &args, size_t start_index,
                                              AppConfig &config) {
        if (args.size() <= start_index) {
            return true; // no more arguments, use default level
        }

        if (args[start_index] != "--level") {
            print_error("Unknown argument: " + args[start_index]);
            return false;
        }

        if (args.size() < start_index + 2) {
            print_error("Missing value for --level option");
            return false;
        }

        auto level = logger::utility::string_to_level(args[start_index + 1]);
        if (not level.has_value()) {
            print_error("Invalid log level: " + args[start_index + 1]);
            return false;
        }

        config.level = level.value();

        // Check for extra arguments
        if (args.size() > start_index + 2) {
            print_error("Unexpected argument: " + args[start_index + 2]);
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
