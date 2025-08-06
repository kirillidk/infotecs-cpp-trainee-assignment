#include "file_sink.hpp"

namespace logger {
    FileSink::FileSink(const std::string &filename) { file_stream_.open(filename, std::ios::app); }

    FileSink::~FileSink() {
        if (file_stream_.is_open()) {
            file_stream_.close();
        }
    }

    void FileSink::write(std::string_view message) {
        if (not is_valid()) {
            return;
        }

        std::lock_guard<std::mutex> lock(fs_mutex_);
        file_stream_ << message << std::endl;
    }

    bool FileSink::is_valid() const {
        std::lock_guard<std::mutex> lock(fs_mutex_);
        return file_stream_.is_open() && file_stream_.good();
    }
} // namespace logger
