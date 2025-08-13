#include <gtest/gtest.h>

#include <test_application/argument_parser.hpp>

using namespace test_application;

class ArgumentParserTest : public ::testing::Test {};

// Help option tests
TEST_F(ArgumentParserTest, ParsesHelpShortOption) {
    std::vector<std::string> args = {"-h"};

    auto config_opt = ArgumentParser::parse_arguments(args);

    ASSERT_TRUE(config_opt.has_value());
    EXPECT_EQ(config_opt->mode, AppConfig::Mode::HELP);
}

TEST_F(ArgumentParserTest, ParsesHelpLongOption) {
    std::vector<std::string> args = {"--help"};

    auto config_opt = ArgumentParser::parse_arguments(args);

    ASSERT_TRUE(config_opt.has_value());
    EXPECT_EQ(config_opt->mode, AppConfig::Mode::HELP);
}

// File mode tests
TEST_F(ArgumentParserTest, ParsesFileMode) {
    std::vector<std::string> args = {"--file", "test.log"};

    auto config_opt = ArgumentParser::parse_arguments(args);

    ASSERT_TRUE(config_opt.has_value());
    EXPECT_EQ(config_opt->mode, AppConfig::Mode::FILE);
    EXPECT_EQ(config_opt->filename, "test.log");
    EXPECT_EQ(config_opt->level, logger::LogLevel::INFO);
}

TEST_F(ArgumentParserTest, ParsesFileModeWithLevel) {
    std::vector<std::string> args = {"--file", "test.log", "--level", "debug"};

    auto config_opt = ArgumentParser::parse_arguments(args);

    ASSERT_TRUE(config_opt.has_value());
    EXPECT_EQ(config_opt->mode, AppConfig::Mode::FILE);
    EXPECT_EQ(config_opt->filename, "test.log");
    EXPECT_EQ(config_opt->level, logger::LogLevel::DEBUG);
}

TEST_F(ArgumentParserTest, FileModeMissingFilename) {
    std::vector<std::string> args = {"--file"};

    auto config_opt = ArgumentParser::parse_arguments(args);

    EXPECT_FALSE(config_opt.has_value());
}

TEST_F(ArgumentParserTest, FileModeInvalidLevel) {
    std::vector<std::string> args = {"--file", "test.log", "--level", "invalid"};

    auto config_opt = ArgumentParser::parse_arguments(args);

    EXPECT_FALSE(config_opt.has_value());
}

// Socket mode tests
TEST_F(ArgumentParserTest, ParsesSocketMode) {
    std::vector<std::string> args = {"--socket", "127.0.0.1", "9000"};

    auto config_opt = ArgumentParser::parse_arguments(args);

    ASSERT_TRUE(config_opt.has_value());
    EXPECT_EQ(config_opt->mode, AppConfig::Mode::SOCKET);
    EXPECT_EQ(config_opt->host, "127.0.0.1");
    EXPECT_EQ(config_opt->port, 9000);
    EXPECT_EQ(config_opt->level, logger::LogLevel::INFO);
}

TEST_F(ArgumentParserTest, ParsesSocketModeWithLevel) {
    std::vector<std::string> args = {"--socket", "localhost", "8080", "--level", "error"};

    auto config_opt = ArgumentParser::parse_arguments(args);

    ASSERT_TRUE(config_opt.has_value());
    EXPECT_EQ(config_opt->mode, AppConfig::Mode::SOCKET);
    EXPECT_EQ(config_opt->host, "localhost");
    EXPECT_EQ(config_opt->port, 8080);
    EXPECT_EQ(config_opt->level, logger::LogLevel::ERROR);
}

TEST_F(ArgumentParserTest, SocketModeMissingHost) {
    std::vector<std::string> args = {"--socket"};

    auto config_opt = ArgumentParser::parse_arguments(args);

    EXPECT_FALSE(config_opt.has_value());
}

TEST_F(ArgumentParserTest, SocketModeMissingPort) {
    std::vector<std::string> args = {"--socket", "127.0.0.1"};

    auto config_opt = ArgumentParser::parse_arguments(args);

    EXPECT_FALSE(config_opt.has_value());
}

TEST_F(ArgumentParserTest, SocketModeInvalidPort) {
    std::vector<std::string> args = {"--socket", "127.0.0.1", "invalid"};

    auto config_opt = ArgumentParser::parse_arguments(args);

    EXPECT_FALSE(config_opt.has_value());
}

TEST_F(ArgumentParserTest, SocketModePortOutOfRange) {
    std::vector<std::string> args = {"--socket", "127.0.0.1", "99999"};

    auto config_opt = ArgumentParser::parse_arguments(args);

    EXPECT_FALSE(config_opt.has_value());
}

// Error cases
TEST_F(ArgumentParserTest, TooFewArguments) {
    std::vector<std::string> args = {};

    auto config_opt = ArgumentParser::parse_arguments(args);

    EXPECT_FALSE(config_opt.has_value());
}

TEST_F(ArgumentParserTest, UnknownOption) {
    std::vector<std::string> args = {"--unknown"};

    auto config_opt = ArgumentParser::parse_arguments(args);

    EXPECT_FALSE(config_opt.has_value());
}

TEST_F(ArgumentParserTest, ExtraArguments) {
    std::vector<std::string> args = {"--file", "test.log", "--level", "info", "extra"};

    auto config_opt = ArgumentParser::parse_arguments(args);

    EXPECT_FALSE(config_opt.has_value());
}
