#pragma once

#include <string>
#include <vector>

namespace metrics_application {
    class SocketServer {
    public:
        SocketServer(const std::string &host, int port);
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

        void process_message(std::string_view message);

    private:
        std::string host_;
        int port_;
        int server_fd_;
        int client_fd_;
        std::vector<char> client_buffer_;
        bool running_;
    };
} // namespace metrics_application
