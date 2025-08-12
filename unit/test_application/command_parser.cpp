#include <gtest/gtest.h>

#include <test_application/command_parser.hpp>

using namespace test_application;

class CommandParserTest : public ::testing::Test {};

// Basic command parsing tests
TEST_F(CommandParserTest, ParsesExitCommand) {
    auto result = CommandParser::parse_command("exit");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->type, CommandType::EXIT);
}

TEST_F(CommandParserTest, ParsesQuitCommand) {
    auto result = CommandParser::parse_command("quit");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->type, CommandType::EXIT);
}

TEST_F(CommandParserTest, ParsesHelpCommand) {
    auto result = CommandParser::parse_command("help");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->type, CommandType::HELP);
}

// Log message without level tests
TEST_F(CommandParserTest, ParsesMessageWithoutLevel) {
    auto result = CommandParser::parse_command("Connection established successfully");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->type, CommandType::LOG_MESSAGE);
    EXPECT_EQ(result->message, "Connection established successfully");
    EXPECT_FALSE(result->level.has_value());
}

// Log message with level tests
TEST_F(CommandParserTest, ParsesMessageWithDebugLevel) {
    auto result = CommandParser::parse_command("Debug information here debug");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->type, CommandType::LOG_MESSAGE);
    EXPECT_EQ(result->message, "Debug information here");
    ASSERT_TRUE(result->level.has_value());
    EXPECT_EQ(result->level.value(), logger::LogLevel::DEBUG);
}

TEST_F(CommandParserTest, ParsesMessageWithInfoLevel) {
    auto result = CommandParser::parse_command("Application started info");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->type, CommandType::LOG_MESSAGE);
    EXPECT_EQ(result->message, "Application started");
    ASSERT_TRUE(result->level.has_value());
    EXPECT_EQ(result->level.value(), logger::LogLevel::INFO);
}

TEST_F(CommandParserTest, ParsesMessageWithWarningLevel) {
    auto result = CommandParser::parse_command("Memory usage high warning");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->type, CommandType::LOG_MESSAGE);
    EXPECT_EQ(result->message, "Memory usage high");
    ASSERT_TRUE(result->level.has_value());
    EXPECT_EQ(result->level.value(), logger::LogLevel::WARNING);
}

TEST_F(CommandParserTest, ParsesMessageWithErrorLevel) {
    auto result = CommandParser::parse_command("Connection failed error");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->type, CommandType::LOG_MESSAGE);
    EXPECT_EQ(result->message, "Connection failed");
    ASSERT_TRUE(result->level.has_value());
    EXPECT_EQ(result->level.value(), logger::LogLevel::ERROR);
}

TEST_F(CommandParserTest, ParsesMessageWithFatalLevel) {
    auto result = CommandParser::parse_command("System crash fatal");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->type, CommandType::LOG_MESSAGE);
    EXPECT_EQ(result->message, "System crash");
    ASSERT_TRUE(result->level.has_value());
    EXPECT_EQ(result->level.value(), logger::LogLevel::FATAL);
}

// Case sensitivity tests
TEST_F(CommandParserTest, ParsesLevelCaseInsensitive) {
    auto result = CommandParser::parse_command("Test message DEBUG");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->type, CommandType::LOG_MESSAGE);
    EXPECT_EQ(result->message, "Test message");
    ASSERT_TRUE(result->level.has_value());
    EXPECT_EQ(result->level.value(), logger::LogLevel::DEBUG);
}

// Whitespace handling tests
TEST_F(CommandParserTest, HandlesLeadingWhitespace) {
    auto result = CommandParser::parse_command("   Hello World");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->type, CommandType::LOG_MESSAGE);
    EXPECT_EQ(result->message, "Hello World");
}

TEST_F(CommandParserTest, HandlesTrailingWhitespace) {
    auto result = CommandParser::parse_command("Hello World   ");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->type, CommandType::LOG_MESSAGE);
    EXPECT_EQ(result->message, "Hello World");
}

TEST_F(CommandParserTest, HandlesMultipleSpaces) {
    auto result = CommandParser::parse_command("Hello    World    info");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->type, CommandType::LOG_MESSAGE);
    EXPECT_EQ(result->message, "Hello    World");
    ASSERT_TRUE(result->level.has_value());
    EXPECT_EQ(result->level.value(), logger::LogLevel::INFO);
}

// Edge cases
TEST_F(CommandParserTest, ReturnsNulloptForEmptyString) {
    auto result = CommandParser::parse_command("");

    EXPECT_FALSE(result.has_value());
}

TEST_F(CommandParserTest, ReturnsNulloptForWhitespacesAndSpecialCharacters) {
    auto result = CommandParser::parse_command("   \t\n   ");

    EXPECT_FALSE(result.has_value());
}

TEST_F(CommandParserTest, ParsesSingleWordAsMessage) {
    auto result = CommandParser::parse_command("test");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->type, CommandType::LOG_MESSAGE);
    EXPECT_EQ(result->message, "test");
    EXPECT_FALSE(result->level.has_value());
}

TEST_F(CommandParserTest, ParsesInvalidLevelAsPartOfMessage) {
    auto result = CommandParser::parse_command("This is a test invalidlevel");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->type, CommandType::LOG_MESSAGE);
    EXPECT_EQ(result->message, "This is a test invalidlevel");
    EXPECT_FALSE(result->level.has_value());
}

// Level parsing utility tests
TEST_F(CommandParserTest, ParseLevelReturnsCorrectLevels) {
    EXPECT_EQ(CommandParser::parse_level("debug"), logger::LogLevel::DEBUG);
    EXPECT_EQ(CommandParser::parse_level("info"), logger::LogLevel::INFO);
    EXPECT_EQ(CommandParser::parse_level("warning"), logger::LogLevel::WARNING);
    EXPECT_EQ(CommandParser::parse_level("error"), logger::LogLevel::ERROR);
    EXPECT_EQ(CommandParser::parse_level("fatal"), logger::LogLevel::FATAL);
}

TEST_F(CommandParserTest, ParseLevelReturnsNulloptForInvalid) {
    EXPECT_FALSE(CommandParser::parse_level("invalid").has_value());
    EXPECT_FALSE(CommandParser::parse_level("").has_value());
    EXPECT_FALSE(CommandParser::parse_level("trace").has_value());
}
