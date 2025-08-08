#pragma once

#include <atomic>
#include <memory>
#include <string>
#include <thread>

#include <logger/log_level.hpp>

#include "log_entry.hpp"
#include "thread_safe_queue.hpp"

namespace logger {
    class Logger;
}

namespace test_application {
    struct ParsedCommand;

    class TestApplication {
    public:
        static std::unique_ptr<TestApplication> create_application(const std::string &log_filename,
                                                                   logger::LogLevel default_level);
        ~TestApplication();

    public:
        void run();
        void stop();

    private:
        TestApplication(std::shared_ptr<logger::Logger> logger, logger::LogLevel default_level);

    private:
        void process_command(const ParsedCommand &command);
        void worker_thread_function();

    private:
        std::shared_ptr<logger::Logger> logger_;
        ThreadSafeQueue<LogEntry> log_queue_;
        logger::LogLevel default_level_;
        std::thread worker_thread_;
        std::atomic<bool> is_running_{false};
    };
} // namespace test_application
