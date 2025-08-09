#pragma once

#include <mutex>
#include <string>

#include "sink.hpp"

namespace logger {
    class SocketSink : public ILogSink {
    public:
        explicit SocketSink(const std::string &host, int port);
        ~SocketSink() override;

        void write(std::string_view message) override;
        bool is_valid() const override;

    private:
        static constexpr int POLL_TIMEOUT_MS = 1000;

        bool init_socket();
        void cleanup_socket();
        bool connect_to_server();
        bool set_non_blocking(int socket);
        bool wait_for_socket_ready(int socket, bool for_write = true);

    private:
        int socket_fd_;
        mutable std::mutex socket_mutex_;
        std::string host_;
        int port_;
        bool is_connected_;
    };
} // namespace logger
