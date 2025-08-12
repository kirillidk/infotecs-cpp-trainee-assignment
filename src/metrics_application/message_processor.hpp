#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <string_view>
#include <thread>

namespace metrics_application {
    class MetricsCollector;

    class MessageProcessor {
    public:
        using StatsCallback = std::function<void()>;

    public:
        MessageProcessor(int message_interval, int timeout_seconds);
        ~MessageProcessor();

    public:
        void start();
        void stop();
        void process_message(std::string_view log_message);

        // Set callback for when stats should be printed
        void set_stats_callback(StatsCallback callback);

    private:
        void stats_timer_thread();

    private:
        int message_interval_;
        std::chrono::seconds timeout_seconds_;

        std::shared_ptr<MetricsCollector> metrics_collector_;

        // For periodic stats printing
        std::thread stats_timer_thread_;
        std::chrono::steady_clock::time_point last_stats_print_time_;
        bool running_;

        StatsCallback stats_callback_;
    };
} // namespace metrics_application
