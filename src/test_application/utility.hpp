#pragma once

#include <optional>
#include <string>
#include <vector>

#include <logger/log_level.hpp>

namespace test_application {
    namespace utility {
        void print_usage(const char *program_name);

        void print_help();

        void print_available_levels();

        void print_welcome_message();

        std::vector<std::string> parse_arguments(int argc, char *argv[]);
    } // namespace utility
} // namespace test_application
