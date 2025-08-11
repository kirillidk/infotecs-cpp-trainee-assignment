#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <vector>

namespace metrics_application {
    class MetricsCollector;

    class SocketServer {
    public:
        SocketServer(const std::string &host, int port, int message_interval, int timeout_seconds);
        ~SocketServer();

        bool start();
        void run();
        void stop();

    private:
        static constexpr int POLL_TIMEOUT_MS = 1000;
        static constexpr size_t BUFFER_SIZE = 4096;

        bool init_socket();
        bool set_non_blocking(int socket);

        void handle_recv();

        void process_message(std::string_view log_message);

    private:
        std::string host_;
        int port_;
        int server_fd_;
        int client_fd_;
        std::vector<char> client_buffer_;
        bool running_;

        int message_interval_;
        std::chrono::seconds timeout_seconds_;

        std::shared_ptr<MetricsCollector> metrics_collector_;
    };
} // namespace metrics_application
