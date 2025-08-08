#pragma once

#include <string>

#include <logger/log_level.hpp>

namespace test_application {
    struct LogEntry {
        std::string message;
        logger::LogLevel level;

        LogEntry(std::string msg, logger::LogLevel lvl) : message(std::move(msg)), level(lvl) {}
    };
} // namespace test_application
