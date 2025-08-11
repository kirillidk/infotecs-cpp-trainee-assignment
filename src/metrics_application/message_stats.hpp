#pragma once

#include <limits>
#include <map>

#include <logger/log_level.hpp>

namespace metrics_application {
    struct MessageStats {
        size_t total_messages = 0;
        std::map<logger::LogLevel, size_t> messages_by_level{{logger::LogLevel::DEBUG, 0},
                                                             {logger::LogLevel::INFO, 0},
                                                             {logger::LogLevel::WARNING, 0},
                                                             {logger::LogLevel::ERROR, 0},
                                                             {logger::LogLevel::FATAL, 0}};
        size_t messages_last_hour = 0;

        size_t min_length = std::numeric_limits<size_t>::max();
        size_t max_length = 0;
        double average_length = 0.0;

        size_t total_length = 0; // for calculating average_length
    };
} // namespace metrics_application
