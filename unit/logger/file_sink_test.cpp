#include <filesystem>
#include <fstream>
#include <optional>
#include <thread>
#include <vector>

#include <gtest/gtest.h>

#include <logger/file_sink.hpp>

class FileSinkTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_filename_ = "test_file_sink.log";
        cleanup_file();
    }

    void TearDown() override { cleanup_file(); }

    void cleanup_file() {
        if (std::filesystem::exists(test_filename_)) {
            std::filesystem::remove(test_filename_);
        }
    }

    std::optional<std::string> read_file_content() {
        std::ifstream file(test_filename_);
        if (not file.is_open()) {
            return std::nullopt;
        }

        std::string content;
        std::string line;
        while (std::getline(file, line)) {
            if (!content.empty()) {
                content += "\n";
            }
            content += line;
        }
        return content;
    }

    std::string test_filename_;
};

// Constructor tests
TEST_F(FileSinkTest, Constructor_ValidFile) {
    logger::FileSink sink(test_filename_);
    EXPECT_TRUE(sink.is_valid());
}

TEST_F(FileSinkTest, Constructor_InvalidPath) {
    logger::FileSink sink("/invalid/path/file.log");
    EXPECT_FALSE(sink.is_valid());
}

TEST_F(FileSinkTest, Constructor_EmptyFilename) {
    logger::FileSink sink("");
    EXPECT_FALSE(sink.is_valid());
}

// Write functionality tests
TEST_F(FileSinkTest, Write_SingleMessage) {
    logger::FileSink sink(test_filename_);
    ASSERT_TRUE(sink.is_valid());

    sink.write("Test message");

    auto content_opt = read_file_content();
    ASSERT_TRUE(content_opt.has_value());

    std::string content = content_opt.value();
    EXPECT_EQ(content, "Test message");
}

TEST_F(FileSinkTest, Write_MultipleMessages) {
    logger::FileSink sink(test_filename_);
    ASSERT_TRUE(sink.is_valid());

    sink.write("First message");
    sink.write("Second message");
    sink.write("Third message");

    auto content_opt = read_file_content();
    ASSERT_TRUE(content_opt.has_value());

    std::string content = content_opt.value();
    EXPECT_EQ(content, "First message\nSecond message\nThird message");
}

TEST_F(FileSinkTest, Write_EmptyMessage) {
    logger::FileSink sink(test_filename_);
    ASSERT_TRUE(sink.is_valid());

    sink.write("");

    auto content_opt = read_file_content();
    ASSERT_TRUE(content_opt.has_value());

    std::string content = content_opt.value();
    EXPECT_EQ(content, "");
}

TEST_F(FileSinkTest, Write_MessageWithSpecialCharacters) {
    logger::FileSink sink(test_filename_);
    ASSERT_TRUE(sink.is_valid());

    std::string special_message = "Message with\ttabs\nand newlines\rand\x01special chars";
    sink.write(special_message);

    auto content_opt = read_file_content();
    ASSERT_TRUE(content_opt.has_value());

    std::string content = content_opt.value();
    EXPECT_TRUE(content.find("Message with\ttabs\nand newlines\rand\x01special chars") != std::string::npos);
}

TEST_F(FileSinkTest, Write_ToInvalidSink) {
    logger::FileSink sink("/invalid/path/file.log");
    ASSERT_FALSE(sink.is_valid());

    // Should not crash, should gracefully handle invalid sink
    sink.write("This should not be written");

    EXPECT_FALSE(std::filesystem::exists("/invalid/path/file.log"));
}

// Thread safety tests
TEST_F(FileSinkTest, Write_ThreadSafety) {
    logger::FileSink sink(test_filename_);
    ASSERT_TRUE(sink.is_valid());

    const int num_threads = 10;
    const int messages_per_thread = 100;
    std::vector<std::thread> threads;

    // Launch multiple threads writing concurrently
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&sink, i, messages_per_thread]() {
            for (int j = 0; j < messages_per_thread; ++j) {
                sink.write("Thread " + std::to_string(i) + " Message " + std::to_string(j));
            }
        });
    }

    // Wait for all threads to complete
    for (auto &t: threads) {
        t.join();
    }

    // Verify file contains expected number of lines
    std::ifstream file(test_filename_);

    int line_count = 0;
    std::string line;
    while (std::getline(file, line)) {
        line_count++;
    }

    EXPECT_EQ(line_count, num_threads * messages_per_thread);
}

// is_valid tests
TEST_F(FileSinkTest, IsValid_AfterConstruction) {
    logger::FileSink valid_sink(test_filename_);
    EXPECT_TRUE(valid_sink.is_valid());

    logger::FileSink invalid_sink("/invalid/path/file.log");
    EXPECT_FALSE(invalid_sink.is_valid());
}
