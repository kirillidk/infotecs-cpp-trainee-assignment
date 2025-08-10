#include "test_application.hpp"

#include <iostream>

#include <logger/logger.hpp>
#include <logger/socket_sink.hpp>
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

    std::unique_ptr<TestApplication> TestApplication::create_application(const std::string &host, int port,
                                                                         logger::LogLevel default_level) {
        auto logger = logger::Logger::create_logger(host, port, default_level);
        if (not logger) {
            return nullptr;
        }

        auto test_application = std::unique_ptr<TestApplication>(new TestApplication(std::move(logger), default_level));
        return test_application;
    }

    TestApplication::TestApplication(std::shared_ptr<logger::Logger> logger, logger::LogLevel default_level) :
        logger_(std::move(logger)), default_level_(default_level) {}

    TestApplication::~TestApplication() { stop(); }

    void TestApplication::run() {
        utility::print_welcome_message();
        std::cout << "> ";

        is_running_.store(true);
        worker_thread_ = std::thread(&TestApplication::worker_thread_function, this);

        std::string input;
        while (is_running_.load() && std::getline(std::cin, input)) {
            std::optional<ParsedCommand> parsed_command = CommandParser::parse_command(input);
            if (parsed_command.has_value()) {
                process_command(parsed_command.value());
            }

            if (is_running_.load()) {
                std::cout << "> ";
            }
        }
    }

    void TestApplication::stop() {
        if (is_running_.load()) {
            is_running_.store(false);
            log_queue_.stop();

            if (worker_thread_.joinable()) {
                worker_thread_.join();
            }
        }
    }

    void TestApplication::process_command(const ParsedCommand &command) {
        using namespace utility;

        switch (command.type) {
            case CommandType::LOG_MESSAGE: {
                logger::LogLevel level = command.level.value_or(default_level_);
                log_queue_.push(LogEntry(command.message, level));
                break;
            }

            case CommandType::HELP:
                utility::print_help();
                break;

            case CommandType::EXIT:
                stop();
                break;
        }
    }

    void TestApplication::worker_thread_function() {
        LogEntry entry("", logger::LogLevel::INFO);

        while (is_running_.load()) {
            if (log_queue_.pop(entry)) {
                if (logger_ && logger_->is_valid()) {
                    logger_->log(entry.message, entry.level);
                }
            }
        }

        while (log_queue_.pop(entry)) {
            if (logger_ && logger_->is_valid()) {
                logger_->log(entry.message, entry.level);
            }
        }
    }

} // namespace test_application
