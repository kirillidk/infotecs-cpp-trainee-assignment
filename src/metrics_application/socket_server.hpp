#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace metrics_application {
    class SocketServer {
    public:
        using MessageCallback = std::function<void(std::string_view)>;

    public:
        SocketServer(const std::string &host, int port);
        ~SocketServer();

    public:
        bool start();
        void run();
        void stop();

        // Set callback for when a message is received
        void set_message_callback(MessageCallback callback);

    private:
        static constexpr int POLL_TIMEOUT_MS = 1000;
        static constexpr size_t BUFFER_SIZE = 4096;

        bool init_socket();
        bool set_non_blocking(int socket);
        void handle_recv();

    private:
        std::string host_;
        int port_;
        int server_fd_;
        int client_fd_;
        std::vector<char> client_buffer_;
        bool running_;

        MessageCallback message_callback_;
    };
} // namespace metrics_application
