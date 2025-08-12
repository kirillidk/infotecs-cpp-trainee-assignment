#include <iostream>

#include "argument_parser.hpp"
#include "command_parser.hpp"
#include "test_application.hpp"
#include "utility.hpp"

int main(int argc, char *argv[]) {
    using namespace test_application;

    std::vector<std::string> args = utility::parse_arguments(argc, argv);

    auto config = ArgumentParser::parse_arguments(args);
    if (not config.has_value()) {
        utility::print_usage(argv[0]);
        return 1;
    }

    if (config->mode == AppConfig::Mode::HELP) {
        utility::print_usage(argv[0]);
        return 0;
    }

    std::unique_ptr<TestApplication> testApplication;

    if (config->mode == AppConfig::Mode::FILE) {
        testApplication = TestApplication::create_application(config->filename, config->level);
        if (not testApplication) {
            std::cerr << "Failed to create Test application with file: " << config->filename << std::endl;
            return 1;
        }
    } else if (config->mode == AppConfig::Mode::SOCKET) {
        testApplication = TestApplication::create_application(config->host, config->port, config->level);
        if (not testApplication) {
            std::cerr << "Failed to create Test application with socket: " << config->host << ":" << config->port
                      << std::endl;
            return 1;
        }
    }

    testApplication->run();

    return 0;
}
