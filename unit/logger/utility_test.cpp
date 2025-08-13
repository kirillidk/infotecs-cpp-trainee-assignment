#include <regex>

#include <gtest/gtest.h>

#include <logger/utility.hpp>

class UtilityTest : public ::testing::Test {};

// Tests for level_to_string
TEST_F(UtilityTest, LevelToString_AllLevels) {
    EXPECT_EQ(logger::utility::level_to_string(logger::LogLevel::DEBUG), "DEBUG");
    EXPECT_EQ(logger::utility::level_to_string(logger::LogLevel::INFO), "INFO");
    EXPECT_EQ(logger::utility::level_to_string(logger::LogLevel::WARNING), "WARNING");
    EXPECT_EQ(logger::utility::level_to_string(logger::LogLevel::ERROR), "ERROR");
    EXPECT_EQ(logger::utility::level_to_string(logger::LogLevel::FATAL), "FATAL");
}

TEST_F(UtilityTest, LevelToString_UnknownLevel) {
    auto unknown_level = static_cast<logger::LogLevel>(999);
    EXPECT_EQ(logger::utility::level_to_string(unknown_level), "UNKNOWN");
}

// Tests for string_to_level
TEST_F(UtilityTest, StringToLevel_ValidLevels) {
    EXPECT_EQ(logger::utility::string_to_level("debug"), logger::LogLevel::DEBUG);
    EXPECT_EQ(logger::utility::string_to_level("info"), logger::LogLevel::INFO);
    EXPECT_EQ(logger::utility::string_to_level("warning"), logger::LogLevel::WARNING);
    EXPECT_EQ(logger::utility::string_to_level("error"), logger::LogLevel::ERROR);
    EXPECT_EQ(logger::utility::string_to_level("fatal"), logger::LogLevel::FATAL);
}

TEST_F(UtilityTest, StringToLevel_CaseInsensitive) {
    EXPECT_EQ(logger::utility::string_to_level("DEBUG"), logger::LogLevel::DEBUG);
    EXPECT_EQ(logger::utility::string_to_level("Info"), logger::LogLevel::INFO);
    EXPECT_EQ(logger::utility::string_to_level("WARNING"), logger::LogLevel::WARNING);
    EXPECT_EQ(logger::utility::string_to_level("Error"), logger::LogLevel::ERROR);
    EXPECT_EQ(logger::utility::string_to_level("FATAL"), logger::LogLevel::FATAL);
}

TEST_F(UtilityTest, StringToLevel_InvalidString) {
    EXPECT_FALSE(logger::utility::string_to_level("invalid").has_value());
    EXPECT_FALSE(logger::utility::string_to_level("").has_value());
    EXPECT_FALSE(logger::utility::string_to_level("trace").has_value());
    EXPECT_FALSE(logger::utility::string_to_level("123").has_value());
}

// Tests for get_current_timestamp
TEST_F(UtilityTest, GetCurrentTimestamp_Format) {
    std::string timestamp = logger::utility::get_current_timestamp();

    // Should match format: YYYY-MM-DD HH:MM:SS.ffffff
    std::regex timestamp_regex(R"(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{6})");
    EXPECT_TRUE(std::regex_match(timestamp, timestamp_regex));
}

// Tests for format_message
TEST_F(UtilityTest, FormatMessage_BasicFormat) {
    std::string message = "Test message";
    std::string formatted = logger::utility::format_message(message, logger::LogLevel::INFO);

    // Should contain timestamp, level, and message
    EXPECT_TRUE(formatted.find("[INFO]") != std::string::npos);
    EXPECT_TRUE(formatted.find("Test message") != std::string::npos);

    // Should match format: [timestamp] [LEVEL] message
    std::regex format_regex(R"(\[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{6}\] \[INFO\] Test message)");
    EXPECT_TRUE(std::regex_match(formatted, format_regex));
}

TEST_F(UtilityTest, FormatMessage_AllLevels) {
    std::string message = "Test";

    EXPECT_TRUE(logger::utility::format_message(message, logger::LogLevel::DEBUG).find("[DEBUG]") != std::string::npos);
    EXPECT_TRUE(logger::utility::format_message(message, logger::LogLevel::INFO).find("[INFO]") != std::string::npos);
    EXPECT_TRUE(logger::utility::format_message(message, logger::LogLevel::WARNING).find("[WARNING]") !=
                std::string::npos);
    EXPECT_TRUE(logger::utility::format_message(message, logger::LogLevel::ERROR).find("[ERROR]") != std::string::npos);
    EXPECT_TRUE(logger::utility::format_message(message, logger::LogLevel::FATAL).find("[FATAL]") != std::string::npos);
}

TEST_F(UtilityTest, FormatMessage_EmptyMessage) {
    std::string formatted = logger::utility::format_message("", logger::LogLevel::INFO);

    EXPECT_TRUE(formatted.find("[INFO]") != std::string::npos);
}

TEST_F(UtilityTest, FormatMessage_SpecialCharacters) {
    std::string message = "Message with\nnewlines\tand\ttabs";
    std::string formatted = logger::utility::format_message(message, logger::LogLevel::ERROR);

    EXPECT_TRUE(formatted.find("[ERROR]") != std::string::npos);
    EXPECT_TRUE(formatted.find("Message with\nnewlines\tand\ttabs") != std::string::npos);
}
