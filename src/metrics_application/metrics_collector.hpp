#pragma once

#include <memory>
#include <mutex>
#include <string_view>

#include "message_stats.hpp"

namespace metrics_application {
    class MetricsCollector {
    public:
        [[nodiscard]] static std::shared_ptr<MetricsCollector> create();

    private:
        MetricsCollector() = default;

    public:
        void add_message(std::string_view message, logger::LogLevel level);
        void print_stats();
        [[nodiscard]] bool should_print_stats(size_t message_interval) const;
        [[nodiscard]] bool has_stats_changed_since_last_print() const;
        [[nodiscard]] MessageStats get_stats();

    private:
        void update_messages_last_hour();

    private:
        MessageStats stats_;
        mutable std::mutex stats_mutex_;
    };
} // namespace metrics_application
