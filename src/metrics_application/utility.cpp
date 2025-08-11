#include "utility.hpp"

#include <iostream>

namespace metrics_application {
    void print_usage(const char *program_name) {
        std::cout << "Usage: " << program_name << " <host> <port>\n\n";
        std::cout << "Parameters:\n";
        std::cout << "  host - Host address to bind to (e.g., 127.0.0.1, 0.0.0.0)\n";
        std::cout << "  port - Port number to listen on\n\n";
        std::cout << "Examples:\n";
        std::cout << "  " << program_name << " 127.0.0.1 9000\n";
    }
} // namespace metrics_application
