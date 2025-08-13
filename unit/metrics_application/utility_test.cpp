#include <gtest/gtest.h>

#include <metrics_application/utility.hpp>

class UtilityTest : public ::testing::Test {};

// Tests for parse_level_from_log function
TEST_F(UtilityTest, ParseLevelFromLog_ValidDebugLevel) {
    std::string log_message = "[2024-01-15 10:30:45] [DEBUG] This is a debug message";
    auto result = metrics_application::utility::parse_level_from_log(log_message);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), logger::LogLevel::DEBUG);
}

TEST_F(UtilityTest, ParseLevelFromLog_ValidInfoLevel) {
    std::string log_message = "[2024-01-15 10:30:45] [INFO] This is an info message";
    auto result = metrics_application::utility::parse_level_from_log(log_message);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), logger::LogLevel::INFO);
}

TEST_F(UtilityTest, ParseLevelFromLog_ValidWarningLevel) {
    std::string log_message = "[2024-01-15 10:30:45] [WARNING] This is a warning message";
    auto result = metrics_application::utility::parse_level_from_log(log_message);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), logger::LogLevel::WARNING);
}

TEST_F(UtilityTest, ParseLevelFromLog_ValidErrorLevel) {
    std::string log_message = "[2024-01-15 10:30:45] [ERROR] This is an error message";
    auto result = metrics_application::utility::parse_level_from_log(log_message);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), logger::LogLevel::ERROR);
}

TEST_F(UtilityTest, ParseLevelFromLog_ValidFatalLevel) {
    std::string log_message = "[2024-01-15 10:30:45] [FATAL] This is a fatal message";
    auto result = metrics_application::utility::parse_level_from_log(log_message);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), logger::LogLevel::FATAL);
}

TEST_F(UtilityTest, ParseLevelFromLog_InvalidFormat_NoTimestampBrackets) {
    std::string log_message = "2024-01-15 10:30:45 [DEBUG] This is a debug message";
    auto result = metrics_application::utility::parse_level_from_log(log_message);

    EXPECT_FALSE(result.has_value());
}

TEST_F(UtilityTest, ParseLevelFromLog_InvalidFormat_NoLevelBrackets) {
    std::string log_message = "[2024-01-15 10:30:45] DEBUG This is a debug message";
    auto result = metrics_application::utility::parse_level_from_log(log_message);

    EXPECT_FALSE(result.has_value());
}

TEST_F(UtilityTest, ParseLevelFromLog_InvalidFormat_IncompleteTimestamp) {
    std::string log_message = "[2024-01-15 10:30:45 [DEBUG] This is a debug message";
    auto result = metrics_application::utility::parse_level_from_log(log_message);

    EXPECT_FALSE(result.has_value());
}

TEST_F(UtilityTest, ParseLevelFromLog_InvalidFormat_IncompleteLevel) {
    std::string log_message = "[2024-01-15 10:30:45] [DEBUG This is a debug message";
    auto result = metrics_application::utility::parse_level_from_log(log_message);

    EXPECT_FALSE(result.has_value());
}

TEST_F(UtilityTest, ParseLevelFromLog_InvalidLevel) {
    std::string log_message = "[2024-01-15 10:30:45] [UNKNOWN] This is an unknown level message";
    auto result = metrics_application::utility::parse_level_from_log(log_message);

    EXPECT_FALSE(result.has_value());
}

// Tests for parse_message_from_log function
TEST_F(UtilityTest, ParseMessageFromLog_ValidMessage) {
    std::string log_message = "[2024-01-15 10:30:45] [DEBUG] This is a debug message";
    auto result = metrics_application::utility::parse_message_from_log(log_message);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "This is a debug message");
}

TEST_F(UtilityTest, ParseMessageFromLog_MessageWithExtraSpaces) {
    std::string log_message = "[2024-01-15 10:30:45] [INFO]   Message with leading spaces";
    auto result = metrics_application::utility::parse_message_from_log(log_message);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "Message with leading spaces");
}

TEST_F(UtilityTest, ParseMessageFromLog_EmptyMessage) {
    std::string log_message = "[2024-01-15 10:30:45] [WARNING]";
    auto result = metrics_application::utility::parse_message_from_log(log_message);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "");
}

TEST_F(UtilityTest, ParseMessageFromLog_EmptyMessageWithSpaces) {
    std::string log_message = "[2024-01-15 10:30:45] [ERROR]   ";
    auto result = metrics_application::utility::parse_message_from_log(log_message);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "");
}

TEST_F(UtilityTest, ParseMessageFromLog_MultilineMessage) {
    std::string log_message = "[2024-01-15 10:30:45] [INFO] First line\nSecond line\nThird line";
    auto result = metrics_application::utility::parse_message_from_log(log_message);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "First line\nSecond line\nThird line");
}

TEST_F(UtilityTest, ParseMessageFromLog_MessageWithBrackets) {
    std::string log_message = "[2024-01-15 10:30:45] [DEBUG] Message with [brackets] in it";
    auto result = metrics_application::utility::parse_message_from_log(log_message);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "Message with [brackets] in it");
}

TEST_F(UtilityTest, ParseMessageFromLog_InvalidFormat_NoTimestampBrackets) {
    std::string log_message = "2024-01-15 10:30:45 [DEBUG] This is a debug message";
    auto result = metrics_application::utility::parse_message_from_log(log_message);

    EXPECT_FALSE(result.has_value());
}

TEST_F(UtilityTest, ParseMessageFromLog_InvalidFormat_NoLevelBrackets) {
    std::string log_message = "[2024-01-15 10:30:45] DEBUG This is a debug message";
    auto result = metrics_application::utility::parse_message_from_log(log_message);

    EXPECT_FALSE(result.has_value());
}

TEST_F(UtilityTest, ParseMessageFromLog_InvalidFormat_IncompleteTimestamp) {
    std::string log_message = "[2024-01-15 10:30:45 [DEBUG] This is a debug message";
    auto result = metrics_application::utility::parse_message_from_log(log_message);

    EXPECT_FALSE(result.has_value());
}

TEST_F(UtilityTest, ParseMessageFromLog_InvalidFormat_IncompleteLevel) {
    std::string log_message = "[2024-01-15 10:30:45] [DEBUG This is a debug message";
    auto result = metrics_application::utility::parse_message_from_log(log_message);

    EXPECT_FALSE(result.has_value());
}

// Edge cases
TEST_F(UtilityTest, ParseFromLog_EmptyString) {
    std::string log_message = "";

    auto level_result = metrics_application::utility::parse_level_from_log(log_message);
    auto message_result = metrics_application::utility::parse_message_from_log(log_message);

    EXPECT_FALSE(level_result.has_value());
    EXPECT_FALSE(message_result.has_value());
}

TEST_F(UtilityTest, ParseFromLog_OnlyBrackets) {
    std::string log_message = "[][]";

    auto level_result = metrics_application::utility::parse_level_from_log(log_message);
    auto message_result = metrics_application::utility::parse_message_from_log(log_message);

    EXPECT_FALSE(level_result.has_value());
    ASSERT_TRUE(message_result.has_value());
    EXPECT_EQ(message_result.value(), "");
}
