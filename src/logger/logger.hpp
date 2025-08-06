#pragma once

#include <fstream>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>

#include <sink.hpp>
#include <utility.hpp>

namespace logger {
    class Logger {
    public:
        static std::shared_ptr<Logger> create_logger(const std::string &filename,
                                                     LogLevel default_level = LogLevel::INFO);

        ~Logger() = default;

        void add_sink(std::unique_ptr<ILogSink> sink);
        void clear_sinks();
        size_t sink_count() const;

        void log(std::string_view message);
        void log(std::string_view message, LogLevel level);

        void debug(std::string_view message);
        void info(std::string_view message);
        void warning(std::string_view message);
        void error(std::string_view message);
        void fatal(std::string_view message);

        void set_default_level(LogLevel level);
        [[nodiscard]] LogLevel get_default_level() const;

        [[nodiscard]] bool is_valid() const;

    private:
        Logger(LogLevel default_level = LogLevel::INFO);

    private:
        std::vector<std::unique_ptr<ILogSink>> sinks_;
        LogLevel default_level_;
        mutable std::mutex sinks_mutex_;
    };
} // namespace logger
