#include <logger.hpp>

namespace logger {
    std::shared_ptr<Logger> Logger::create_logger(const std::string &filename, LogLevel default_level) {
        std::shared_ptr<Logger> logger = std::shared_ptr<Logger>(new Logger(filename, default_level));
        return logger->is_valid() ? logger : nullptr;
    }

    Logger::Logger(const std::string &filename, LogLevel default_level) : _default_level(default_level) {
        _file_stream.open(filename, std::ios::app);
    }

    Logger::~Logger() {
        if (_file_stream.is_open()) {
            _file_stream.close();
        }
    }

    bool Logger::log(std::string_view message, LogLevel level) {
        if (level < _default_level) {
            return true;
        }

        if (not is_valid()) {
            return false;
        }

        {
            std::lock_guard<std::mutex> lock(_fs_mutex);

            std::string formatted_message = utility::format_message(message, level);
            _file_stream << formatted_message << std::endl;
        }

        return _file_stream.good();
    }

    bool Logger::log(std::string_view message) { return log(message, _default_level); }

    bool Logger::debug(std::string_view message) { return log(message, LogLevel::DEBUG); }
    bool Logger::info(std::string_view message) { return log(message, LogLevel::INFO); }
    bool Logger::warning(std::string_view message) { return log(message, LogLevel::WARNING); }
    bool Logger::error(std::string_view message) { return log(message, LogLevel::ERROR); }
    bool Logger::fatal(std::string_view message) { return log(message, LogLevel::FATAL); }

    void Logger::set_default_level(LogLevel level) { _default_level = level; }
    LogLevel Logger::get_default_level() const { return _default_level; }

    bool Logger::is_valid() const { return _file_stream.is_open() && _file_stream.good(); }
} // namespace logger
