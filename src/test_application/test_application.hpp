#pragma once

#include <memory>
#include <string>

#include <logger/log_level.hpp>

namespace logger {
    class Logger;
}

namespace test_application {
    struct ParsedCommand;

    class TestApplication {
    public:
        static std::unique_ptr<TestApplication> create_application(const std::string &log_filename,
                                                                   logger::LogLevel default_level);
        ~TestApplication() = default;

    public:
        void run();

    private:
        TestApplication(std::shared_ptr<logger::Logger> logger, logger::LogLevel default_level);

    private:
        void process_command(const ParsedCommand &command);

    private:
        std::shared_ptr<logger::Logger> logger_;
        logger::LogLevel default_level_;
    };
} // namespace test_application
