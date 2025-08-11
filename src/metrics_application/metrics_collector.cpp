#include "metrics_collector.hpp"

#include <algorithm>
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
    }

    void MetricsCollector::print_stats() const {
        std::lock_guard<std::mutex> lock(stats_mutex_);

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

        std::cout << std::string(50, '=') << std::endl << std::endl;
    }

    bool MetricsCollector::should_print_stats(size_t message_interval) const {
        std::lock_guard<std::mutex> lock(stats_mutex_);
        return (stats_.total_messages > 0) && (stats_.total_messages % message_interval == 0);
    }
} // namespace metrics_application
