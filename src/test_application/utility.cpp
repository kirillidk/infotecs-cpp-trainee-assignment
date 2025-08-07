#include "utility.hpp"

#include <iostream>

namespace test_application {
    namespace utility {
        void print_usage(const char *program_name) {
            std::cout << "Usage: " << program_name << " <log_filename> [default_log_level]\n\n";
            std::cout << "Parameters:\n";
            std::cout << "  log_filename      - Log filename\n";
            std::cout << "  default_log_level - Optional default log level (debug, info, warning, error, fatal) (info "
                         "by default)\n";
            std::cout << "Examples:\n";
            std::cout << "  " << program_name << " app.log\n";
            std::cout << "  " << program_name << " app.log debug\n";
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
    } // namespace utility
} // namespace test_application
