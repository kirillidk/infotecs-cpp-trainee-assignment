#include "metrics_collector.hpp"

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>

#include <logger/utility.hpp>

namespace metrics_application {
    std::shared_ptr<MetricsCollector> MetricsCollector::create() {
        return std::shared_ptr<MetricsCollector>(new MetricsCollector());
    }

    void MetricsCollector::add_message(std::string_view message, logger::LogLevel level) {
        std::lock_guard<std::mutex> lock(stats_mutex_);

        stats_.total_messages++;
        stats_.messages_by_level[level]++;

        size_t message_length = message.length();
        stats_.total_length += message_length;

        stats_.min_length = std::min(stats_.min_length, message_length);
        stats_.max_length = std::max(stats_.max_length, message_length);
        stats_.average_length = static_cast<double>(stats_.total_length) / stats_.total_messages;

        auto now = std::chrono::steady_clock::now();
        stats_.message_timestamps.push_back(now);
    }

    void MetricsCollector::update_messages_last_hour() {
        auto now = std::chrono::steady_clock::now();
        auto one_hour_ago = now - std::chrono::hours(1);

        // Remove timestamps older than one hour
        while (!stats_.message_timestamps.empty() && stats_.message_timestamps.front() < one_hour_ago) {
            stats_.message_timestamps.pop_front();
        }

        stats_.messages_last_hour = stats_.message_timestamps.size();
    }

    void MetricsCollector::print_stats() {
        std::lock_guard<std::mutex> lock(stats_mutex_);
        update_messages_last_hour();

        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "           MESSAGE STATISTICS" << std::endl;
        std::cout << std::string(50, '=') << std::endl;

        // General message statistics
        std::cout << "Total messages: " << stats_.total_messages << std::endl;
        std::cout << "Messages in last hour: " << stats_.messages_last_hour << std::endl;

        // Statistics by level
        std::cout << "\nMessages by log level:" << std::endl;
        for (const auto &[level, count]: stats_.messages_by_level) {
            if (count > 0) {
                std::cout << "  " << logger::utility::level_to_string(level) << ": " << count << std::endl;
            }
        }

        // Message length statistics
        std::cout << "\nMessage length statistics:" << std::endl;
        if (stats_.total_messages > 0) {
            std::cout << "  Minimum length: " << stats_.min_length << " characters" << std::endl;
            std::cout << "  Maximum length: " << stats_.max_length << " characters" << std::endl;
            std::cout << "  Average length: " << std::fixed << std::setprecision(2) << stats_.average_length
                      << " characters" << std::endl;
        } else {
            std::cout << "  No data available" << std::endl;
        }

        std::cout << std::string(50, '=') << '\n' << std::endl;

        // Update last printed count
        stats_.last_printed_total_messages = stats_.total_messages;
    }

    bool MetricsCollector::should_print_stats(size_t message_interval) const {
        std::lock_guard<std::mutex> lock(stats_mutex_);
        return (stats_.total_messages > 0) && (stats_.total_messages % message_interval == 0);
    }

    bool MetricsCollector::has_stats_changed_since_last_print() const {
        std::lock_guard<std::mutex> lock(stats_mutex_);
        return stats_.total_messages != stats_.last_printed_total_messages;
    }

    MessageStats MetricsCollector::get_stats() {
        std::lock_guard<std::mutex> lock(stats_mutex_);
        update_messages_last_hour();
        return stats_;
    }
} // namespace metrics_application
