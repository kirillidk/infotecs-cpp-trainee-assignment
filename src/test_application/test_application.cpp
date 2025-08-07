#include "test_application.hpp"

#include <iostream>

#include <logger/logger.hpp>
#include <logger/utility.hpp>

#include "command_parser.hpp"
#include "utility.hpp"

namespace test_application {
    std::unique_ptr<TestApplication> TestApplication::create_application(const std::string &log_filename,
                                                                         logger::LogLevel default_level) {
        auto logger = logger::Logger::create_logger(log_filename, default_level);
        if (not logger) {
            return nullptr;
        }

        auto test_application = std::unique_ptr<TestApplication>(new TestApplication(std::move(logger), default_level));
        return test_application;
    }

    TestApplication::TestApplication(std::shared_ptr<logger::Logger> logger, logger::LogLevel default_level) :
        logger_(std::move(logger)), default_level_(default_level) {}

    void TestApplication::run() {
        std::string input;

        while (std::getline(std::cin, input)) {
            std::optional<ParsedCommand> parsed_command = CommandParser::parse_command(input);
            if (parsed_command.has_value()) {
                process_command(parsed_command.value());
            }
        }
    }

    void TestApplication::process_command(const ParsedCommand &command) {
        using namespace utility;

        switch (command.type) {
            case CommandType::LOG_MESSAGE: {
                logger::LogLevel level = command.level.value_or(default_level_);
                logger_->log(command.message, level);
                break;
            }

            case CommandType::HELP:
                utility::print_help();
                break;

            case CommandType::EXIT:
                // todo
                break;
        }
    }

} // namespace test_application
