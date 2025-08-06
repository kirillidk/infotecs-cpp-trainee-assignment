#pragma once

#include <string_view>

namespace logger {
    class ILogSink {
    public:
        virtual ~ILogSink() = default;
        virtual void write(std::string_view message) = 0;
        virtual bool is_valid() const = 0;
    };
} // namespace logger
