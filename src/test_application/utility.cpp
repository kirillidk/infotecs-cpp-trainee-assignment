#include "utility.hpp"

#include <iostream>

namespace test_application {
    namespace utility {
        void print_usage(const char *program_name) {
            std::cout << "Usage:\n";
            std::cout << "  " << program_name << " --file <filename> [--level <level>]\n";
            std::cout << "  " << program_name << " --socket <host> <port> [--level <level>]\n";
            std::cout << "  " << program_name << " --help\n\n";

            std::cout << "Options:\n";
            std::cout << "  --file <filename>      Log to file\n";
            std::cout << "  --socket <host> <port> Log to socket server\n";
            std::cout << "  --level <level>        Set default log level (debug, info, warning, error, fatal) "
                         "(Default: info)\n";
            std::cout << "  --help, -h             Show this help\n\n";

            std::cout << "Examples:\n";
            std::cout << "  " << program_name << " --file app.log\n";
            std::cout << "  " << program_name << " --file app.log --level debug\n";
            std::cout << "  " << program_name << " --socket 127.0.0.1 9000 --level error\n";
        }

        void print_help() {
            std::cout << "\n=== Logger Console Application Help ===\n";
            std::cout << "Commands:\n";
            std::cout << "  <message>                - Log message with default level\n";
            std::cout << "  <message> <level>        - Log message with specified level\n";
            std::cout << "  help                     - Show this help\n";
            std::cout << "  exit/quit                - Exit application\n\n";
            print_available_levels();
            std::cout << "\nExamples:\n";
            std::cout << "  Hello World              - Log 'Hello World' with default level\n";
            std::cout << "  Connection failed error  - Log 'Connection failed' as ERROR\n";
            std::cout << "========================================\n\n";
        }

        void print_available_levels() { std::cout << "Available levels: debug, info, warning, error, fatal\n"; }

        void print_welcome_message() {
            std::cout << "\n=== Logger Console Application ===\n";
            std::cout << "Type 'help' for available commands\n";
            std::cout << "Type 'exit' or 'quit' to exit\n";
            std::cout << "===================================\n\n";
        }

        std::vector<std::string> parse_arguments(int argc, char *argv[]) {
            std::vector<std::string> args;
            args.reserve(argc);

            for (int i = 1; i < argc; ++i) {
                args.emplace_back(argv[i]);
            }

            return args;
        }
    } // namespace utility
} // namespace test_application
