#include "logger.hpp"

#include "file_sink.hpp"
#include "socket_sink.hpp"

namespace logger {
    std::shared_ptr<Logger> Logger::create_logger(const std::string &filename, LogLevel default_level) {
        auto logger = std::shared_ptr<Logger>(new Logger(default_level));

        auto file_sink = std::make_unique<FileSink>(filename);
        if (not file_sink->is_valid()) {
            return nullptr;
        }

        logger->add_sink(std::move(file_sink));
        return logger;
    }

    std::shared_ptr<Logger> Logger::create_logger(const std::string &host, int port, LogLevel default_level) {
        auto logger = std::shared_ptr<Logger>(new Logger(default_level));

        auto socket_sink = std::make_unique<SocketSink>(host, port);
        if (not socket_sink->is_valid()) {
            return nullptr;
        }

        logger->add_sink(std::move(socket_sink));
        return logger;
    }

    Logger::Logger(LogLevel default_level) : default_level_(default_level) {}

    void Logger::add_sink(std::unique_ptr<ILogSink> sink) {
        if (sink) {
            std::lock_guard<std::mutex> lock(sinks_mutex_);
            sinks_.push_back(std::move(sink));
        }
    }

    void Logger::clear_sinks() {
        std::lock_guard<std::mutex> lock(sinks_mutex_);
        sinks_.clear();
    }

    size_t Logger::sink_count() const {
        std::lock_guard<std::mutex> lock(sinks_mutex_);
        return sinks_.size();
    }

    void Logger::log(std::string_view message, LogLevel level) {
        if (level < default_level_) {
            return;
        }

        if (not is_valid()) {
            return;
        }

        std::string formatted_message = utility::format_message(message, level);

        std::lock_guard<std::mutex> lock(sinks_mutex_);
        for (const auto &sink: sinks_) {
            sink->write(formatted_message);
        }
    }

    void Logger::log(std::string_view message) { log(message, default_level_); }

    void Logger::debug(std::string_view message) { log(message, LogLevel::DEBUG); }
    void Logger::info(std::string_view message) { log(message, LogLevel::INFO); }
    void Logger::warning(std::string_view message) { log(message, LogLevel::WARNING); }
    void Logger::error(std::string_view message) { log(message, LogLevel::ERROR); }
    void Logger::fatal(std::string_view message) { log(message, LogLevel::FATAL); }

    void Logger::set_default_level(LogLevel level) { default_level_ = level; }
    LogLevel Logger::get_default_level() const { return default_level_; }

    bool Logger::is_valid() const {
        std::lock_guard<std::mutex> lock(sinks_mutex_);
        if (sinks_.empty()) {
            return false;
        }

        for (const auto &sink: sinks_) {
            if (sink->is_valid()) {
                return true;
            }
        }
        return false;
    }
} // namespace logger
