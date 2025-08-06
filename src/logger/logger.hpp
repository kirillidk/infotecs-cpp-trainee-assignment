#pragma once

#include <fstream>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>

#include <utility.hpp>

namespace logger {
    class Logger {
    public:
        static std::shared_ptr<Logger> create_logger(const std::string &filename,
                                                     LogLevel default_level = LogLevel::INFO);

        ~Logger();

        bool log(std::string_view message);
        bool log(std::string_view message, LogLevel level);

        bool debug(std::string_view message);
        bool info(std::string_view message);
        bool warning(std::string_view message);
        bool error(std::string_view message);
        bool fatal(std::string_view message);

        void set_default_level(LogLevel level);
        [[nodiscard]] LogLevel get_default_level() const;

        [[nodiscard]] bool is_valid() const;

    private:
        Logger(const std::string &filename, LogLevel default_level = LogLevel::INFO);

    private:
        std::ofstream _file_stream;
        LogLevel _default_level;
        mutable std::mutex _fs_mutex;
    };
} // namespace logger
