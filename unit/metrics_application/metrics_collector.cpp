#include "metrics_application/metrics_collector.hpp"
#include <chrono>
#include <gtest/gtest.h>
#include <logger/log_level.hpp>
#include <sstream>
#include <thread>

class MetricsCollectorTest : public ::testing::Test {
protected:
    void SetUp() override { collector_ = metrics_application::MetricsCollector::create(); }

    void TearDown() override { collector_.reset(); }

    std::shared_ptr<metrics_application::MetricsCollector> collector_;
};

// Basic functionality tests
TEST_F(MetricsCollectorTest, CreateCollector_InitialState) {
    auto stats = collector_->get_stats();

    EXPECT_EQ(stats.total_messages, 0);
    EXPECT_EQ(stats.messages_last_hour, 0);
    EXPECT_EQ(stats.total_length, 0);
    EXPECT_EQ(stats.min_length, std::numeric_limits<size_t>::max());
    EXPECT_EQ(stats.max_length, 0);
    EXPECT_EQ(stats.average_length, 0.0);
    EXPECT_EQ(stats.last_printed_total_messages, 0);

    // Check all levels are initialized to 0
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::DEBUG), 0);
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::INFO), 0);
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::WARNING), 0);
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::ERROR), 0);
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::FATAL), 0);
}

TEST_F(MetricsCollectorTest, AddSingleMessage_BasicStats) {
    std::string message = "Test message"; // 12 characters
    logger::LogLevel level = logger::LogLevel::INFO;

    collector_->add_message(message, level);
    auto stats = collector_->get_stats();

    EXPECT_EQ(stats.total_messages, 1);
    EXPECT_EQ(stats.messages_last_hour, 1);
    EXPECT_EQ(stats.total_length, 12);
    EXPECT_EQ(stats.min_length, 12);
    EXPECT_EQ(stats.max_length, 12);
    EXPECT_EQ(stats.average_length, 12.0);
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::INFO), 1);
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::DEBUG), 0);
}

TEST_F(MetricsCollectorTest, AddMultipleMessages_DifferentLevels) {
    collector_->add_message("Debug msg", logger::LogLevel::DEBUG); // 9 chars
    collector_->add_message("Info", logger::LogLevel::INFO); // 4 chars
    collector_->add_message("Warning message", logger::LogLevel::WARNING); // 15 chars
    collector_->add_message("Err", logger::LogLevel::ERROR); // 3 chars
    collector_->add_message("Fatal error!", logger::LogLevel::FATAL); // 12 chars

    auto stats = collector_->get_stats();

    EXPECT_EQ(stats.total_messages, 5);
    EXPECT_EQ(stats.messages_last_hour, 5);
    EXPECT_EQ(stats.total_length, 43); // 9+4+15+3+12 = 43
    EXPECT_EQ(stats.min_length, 3); // "Err"
    EXPECT_EQ(stats.max_length, 15); // "Warning message"
    EXPECT_DOUBLE_EQ(stats.average_length, 43.0 / 5.0); // 8.6

    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::DEBUG), 1);
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::INFO), 1);
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::WARNING), 1);
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::ERROR), 1);
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::FATAL), 1);
}

TEST_F(MetricsCollectorTest, MessageLengthStatistics_VariousLengths) {
    collector_->add_message("", logger::LogLevel::INFO); // 0 chars
    collector_->add_message("A", logger::LogLevel::INFO); // 1 char
    collector_->add_message("This is a medium length message", logger::LogLevel::INFO); // 31 chars

    auto stats = collector_->get_stats();

    EXPECT_EQ(stats.total_messages, 3);
    EXPECT_EQ(stats.total_length, 32); // 0+1+31 = 32
    EXPECT_EQ(stats.min_length, 0); // Empty message
    EXPECT_EQ(stats.max_length, 31); // Longest message
    EXPECT_DOUBLE_EQ(stats.average_length, 32.0 / 3.0);
}

TEST_F(MetricsCollectorTest, SameLevelMultipleMessages) {
    for (int i = 0; i < 5; ++i) {
        collector_->add_message("Error " + std::to_string(i), logger::LogLevel::ERROR);
    }

    auto stats = collector_->get_stats();

    EXPECT_EQ(stats.total_messages, 5);
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::ERROR), 5);
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::DEBUG), 0);
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::INFO), 0);
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::WARNING), 0);
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::FATAL), 0);
}

// Stats changed tracking tests
TEST_F(MetricsCollectorTest, HasStatsChanged_InitialState) {
    auto stats = collector_->get_stats();
    EXPECT_EQ(stats.total_messages, 0);
    EXPECT_EQ(stats.last_printed_total_messages, 0);

    EXPECT_FALSE(collector_->has_stats_changed_since_last_print());
}

TEST_F(MetricsCollectorTest, HasStatsChanged_AfterAddingMessage) {
    collector_->add_message("Test message", logger::LogLevel::INFO);

    auto stats = collector_->get_stats();
    EXPECT_EQ(stats.total_messages, 1);
    EXPECT_EQ(stats.last_printed_total_messages, 0);

    EXPECT_TRUE(collector_->has_stats_changed_since_last_print());
}

TEST_F(MetricsCollectorTest, HasStatsChanged_AfterPrintStats) {
    collector_->add_message("Test message", logger::LogLevel::INFO);
    EXPECT_TRUE(collector_->has_stats_changed_since_last_print());

    collector_->print_stats();

    auto stats = collector_->get_stats();
    EXPECT_EQ(stats.total_messages, 1);
    EXPECT_EQ(stats.last_printed_total_messages, 1);

    EXPECT_FALSE(collector_->has_stats_changed_since_last_print());
}

// Edge cases
TEST_F(MetricsCollectorTest, EmptyMessage) {
    collector_->add_message("", logger::LogLevel::WARNING);

    auto stats = collector_->get_stats();

    EXPECT_EQ(stats.total_messages, 1);
    EXPECT_EQ(stats.total_length, 0);
    EXPECT_EQ(stats.min_length, 0);
    EXPECT_EQ(stats.max_length, 0);
    EXPECT_EQ(stats.average_length, 0.0);
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::WARNING), 1);
}

TEST_F(MetricsCollectorTest, MessageWithSpecialCharacters) {
    std::string special_msg = "Message with\nnewlines\tand\ttabs";
    collector_->add_message(special_msg, logger::LogLevel::DEBUG);

    auto stats = collector_->get_stats();

    EXPECT_EQ(stats.total_messages, 1);
    EXPECT_EQ(stats.total_length, special_msg.length());
    EXPECT_EQ(stats.min_length, special_msg.length());
    EXPECT_EQ(stats.max_length, special_msg.length());
    EXPECT_EQ(stats.average_length, static_cast<double>(special_msg.length()));
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::DEBUG), 1);
}

// Complex scenarios
TEST_F(MetricsCollectorTest, ComplexScenario_MixedMessages) {
    // Add messages of different lengths and levels
    collector_->add_message("Short", logger::LogLevel::DEBUG); // 5 chars
    collector_->add_message("", logger::LogLevel::INFO); // 0 chars
    collector_->add_message("This is a longer message with more text", logger::LogLevel::WARNING); // 39 chars
    collector_->add_message("Error!", logger::LogLevel::ERROR); // 6 chars
    collector_->add_message("X", logger::LogLevel::FATAL); // 1 char

    auto stats = collector_->get_stats();

    // Basic counts
    EXPECT_EQ(stats.total_messages, 5);
    EXPECT_EQ(stats.messages_last_hour, 5);

    // Length statistics
    EXPECT_EQ(stats.total_length, 51); // 5+0+39+6+1
    EXPECT_EQ(stats.min_length, 0);
    EXPECT_EQ(stats.max_length, 39);
    EXPECT_DOUBLE_EQ(stats.average_length, 51.0 / 5.0);

    // Level distribution
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::DEBUG), 1);
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::INFO), 1);
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::WARNING), 1);
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::ERROR), 1);
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::FATAL), 1);

    // Should print stats
    EXPECT_TRUE(collector_->should_print_stats(5));
    EXPECT_FALSE(collector_->should_print_stats(3));
    EXPECT_TRUE(collector_->has_stats_changed_since_last_print());
}

// Thread safety tests
TEST_F(MetricsCollectorTest, ThreadSafety_ConcurrentAddMessages) {
    const int num_threads = 4;
    const int messages_per_thread = 100;
    std::vector<std::thread> threads;

    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back([this, t, messages_per_thread]() {
            for (int i = 0; i < messages_per_thread; ++i) {
                std::string msg = "T" + std::to_string(t) + "M" + std::to_string(i); // Consistent length
                logger::LogLevel level = static_cast<logger::LogLevel>(i % 5);
                collector_->add_message(msg, level);
            }
        });
    }

    for (auto &thread: threads) {
        thread.join();
    }

    auto stats = collector_->get_stats();

    int total_expected = num_threads * messages_per_thread;
    EXPECT_EQ(stats.total_messages, total_expected);
    EXPECT_EQ(stats.messages_last_hour, total_expected);

    // Check level distribution (each level should have equal count)
    int expected_per_level = total_expected / 5; // 5 log levels
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::DEBUG), expected_per_level);
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::INFO), expected_per_level);
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::WARNING), expected_per_level);
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::ERROR), expected_per_level);
    EXPECT_EQ(stats.messages_by_level.at(logger::LogLevel::FATAL), expected_per_level);
}
