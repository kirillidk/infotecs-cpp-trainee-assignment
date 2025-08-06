#pragma once

#include <fstream>
#include <mutex>
#include <string>

#include "sink.hpp"

namespace logger {
    class FileSink : public ILogSink {
    private:
        std::ofstream file_stream_;
        mutable std::mutex fs_mutex_;

    public:
        explicit FileSink(const std::string &filename);
        ~FileSink() override;

        void write(std::string_view message) override;
        bool is_valid() const override;
    };
} // namespace logger
