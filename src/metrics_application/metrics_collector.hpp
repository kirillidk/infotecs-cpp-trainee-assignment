#pragma once

#include <chrono>
#include <memory>
#include <mutex>
#include <string_view>

#include "message_stats.hpp"

namespace metrics_application {
    class MetricsCollector {
    public:
        static std::shared_ptr<MetricsCollector> create();

    private:
        MetricsCollector() = default;

    public:
        void add_message(std::string_view message, logger::LogLevel level);
        void print_stats() const;
        bool should_print_stats(size_t message_interval) const;

    private:
        MessageStats stats_;
        mutable std::mutex stats_mutex_;
    };
} // namespace metrics_application
