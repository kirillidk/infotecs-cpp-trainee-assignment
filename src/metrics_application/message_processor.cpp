#include "message_processor.hpp"

#include <iostream>
#include <logger/utility.hpp>

#include "metrics_collector.hpp"
#include "utility.hpp"

namespace metrics_application {
    MessageProcessor::MessageProcessor(int message_interval, int timeout_seconds) :
        message_interval_(message_interval), timeout_seconds_(timeout_seconds),
        metrics_collector_(MetricsCollector::create()), last_stats_print_time_(std::chrono::steady_clock::now()),
        running_(false) {}

    MessageProcessor::~MessageProcessor() { stop(); }

    void MessageProcessor::start() {
        if (running_) {
            return;
        }

        running_ = true;
        last_stats_print_time_ = std::chrono::steady_clock::now();

        // Start timer thread for periodic stats printing
        stats_timer_thread_ = std::thread(&MessageProcessor::stats_timer_thread, this);
    }

    void MessageProcessor::stop() {
        if (not running_) {
            return;
        }

        running_ = false;

        if (stats_timer_thread_.joinable()) {
            stats_timer_thread_.join();
        }
    }

    void MessageProcessor::process_message(std::string_view log_message) {
        std::cout << "[" << logger::utility::get_current_timestamp() << "] LOG: " << log_message << std::endl;

        auto message_opt = utility::parse_message_from_log(log_message);
        if (not message_opt.has_value()) {
            std::cerr << "Error parsing message from log" << std::endl;
            return;
        }

        auto level_opt = utility::parse_level_from_log(log_message);
        if (not level_opt.has_value()) {
            std::cerr << "Error parsing level from log" << std::endl;
            return;
        }

        metrics_collector_->add_message(message_opt.value(), level_opt.value());

        // Reset timer when new message arrives
        last_stats_print_time_ = std::chrono::steady_clock::now();

        // Check if we should print stats based on message count
        if (metrics_collector_->should_print_stats(message_interval_)) {
            if (stats_callback_) {
                stats_callback_();
            } else {
                metrics_collector_->print_stats();
            }
            last_stats_print_time_ = std::chrono::steady_clock::now();
        }
    }

    void MessageProcessor::set_stats_callback(StatsCallback callback) { stats_callback_ = callback; }

    void MessageProcessor::stats_timer_thread() {
        while (running_) {
            auto now = std::chrono::steady_clock::now();
            auto time_since_last_print = std::chrono::duration_cast<std::chrono::seconds>(now - last_stats_print_time_);

            // Check if timeout has passed and stats have changed
            if (time_since_last_print >= timeout_seconds_ && metrics_collector_->has_stats_changed_since_last_print()) {

                if (stats_callback_) {
                    stats_callback_();
                } else {
                    metrics_collector_->print_stats();
                }
                last_stats_print_time_ = now;
            }
        }
    }
} // namespace metrics_application
