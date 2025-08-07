#include <iostream>

#include "command_parser.hpp"
#include "test_application.hpp"
#include "utility.hpp"

int main(int argc, char *argv[]) {
    using namespace test_application;

    if (argc < 2 || argc > 3) {
        utility::print_usage(argv[0]);
        return 1;
    }

    std::string log_filename = argv[1];
    logger::LogLevel default_level = logger::LogLevel::INFO;

    // parse default log level if specified
    if (argc == 3) {
        auto parsed_level = CommandParser::parse_level(argv[2]);
        if (parsed_level.has_value()) {
            default_level = parsed_level.value();
        } else {
            std::cerr << "Invalid default log level: " << argv[2] << '\n';
            utility::print_available_levels();
            return 1;
        }
    }

    std::unique_ptr<TestApplication> testApplication = TestApplication::create_application(log_filename, default_level);
    if (not testApplication) {
        std::cerr << "Failed to create Test application" << std::endl;
        return 1;
    }

    testApplication->run();

    return 0;
}
