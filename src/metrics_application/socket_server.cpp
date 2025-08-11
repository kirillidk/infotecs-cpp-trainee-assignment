#include "socket_server.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <iomanip>
#include <iostream>
#include <poll.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

#include <logger/utility.hpp>

namespace metrics_application {
    SocketServer::SocketServer(const std::string &host, int port) :
        host_(host), port_(port), server_fd_(-1), client_fd_(-1), running_(false) {}

    SocketServer::~SocketServer() { stop(); }

    bool SocketServer::start() {
        if (!init_socket()) {
            return false;
        }

        std::cout << "Waiting for logger connection on " << host_ << ":" << port_ << "..." << std::endl;

        sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        client_fd_ = accept(server_fd_, (sockaddr *) &client_addr, &client_addr_len);
        if (client_fd_ == -1) {
            std::cerr << "Failed to accept connection: " << strerror(errno) << std::endl;
            return false;
        }

        if (not set_non_blocking(client_fd_)) {
            close(client_fd_);
            client_fd_ = -1;
            return false;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);

        std::cout << "Logger connected from " << client_ip << ":" << ntohs(client_addr.sin_port) << std::endl;
        std::cout << "Receiving log messages..." << std::endl;

        running_ = true;
        return true;
    }

    void SocketServer::run() {
        pollfd poll_fd;
        poll_fd.fd = client_fd_;
        poll_fd.events = POLLIN;

        while (running_) {
            poll_fd.revents = 0;

            int poll_result = poll(&poll_fd, 1, POLL_TIMEOUT_MS);

            if (poll_result < 0) {
                if (errno != EINTR) {
                    std::cerr << "Poll error: " << strerror(errno) << std::endl;
                    break;
                }
                continue;
            }

            if (poll_result == 0) {
                continue;
            }

            if (poll_fd.revents & POLLIN) {
                handle_recv();
            }

            if (poll_fd.revents & (POLLHUP | POLLERR | POLLNVAL)) {
                std::cout << "[" << logger::utility::get_current_timestamp() << "] Connection closed" << std::endl;
                break;
            }
        }
    }


    void SocketServer::stop() {
        running_ = false;

        if (client_fd_ != -1) {
            close(client_fd_);
            client_fd_ = -1;
        }

        if (server_fd_ != -1) {
            close(server_fd_);
            server_fd_ = -1;
        }
    }

    bool SocketServer::init_socket() {
        server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd_ == -1) {
            std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
            return false;
        }

        int opt = 1;
        if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
            std::cerr << "Failed to set SO_REUSEADDR: " << strerror(errno) << std::endl;
            close(server_fd_);
            server_fd_ = -1;
            return false;
        }

        sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port_);

        if (inet_pton(AF_INET, host_.c_str(), &server_addr.sin_addr) <= 0) {
            std::cerr << "Invalid host address: " << host_ << std::endl;
            close(server_fd_);
            server_fd_ = -1;
            return false;
        }

        if (bind(server_fd_, (sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
            std::cerr << "Failed to bind socket: " << strerror(errno) << std::endl;
            close(server_fd_);
            server_fd_ = -1;
            return false;
        }

        if (listen(server_fd_, 1) == -1) {
            std::cerr << "Failed to listen on socket: " << strerror(errno) << std::endl;
            close(server_fd_);
            server_fd_ = -1;
            return false;
        }

        return true;
    }

    bool SocketServer::set_non_blocking(int socket) {
        int flags = fcntl(socket, F_GETFL, 0);
        if (flags == -1) {
            return false;
        }

        flags |= O_NONBLOCK;
        return fcntl(socket, F_SETFL, flags) != -1;
    }

    void SocketServer::handle_recv() {
        char buffer[BUFFER_SIZE];

        while (true) {
            ssize_t bytes_read = recv(client_fd_, buffer, BUFFER_SIZE, 0);
            if (bytes_read <= 0) {
                if (bytes_read == 0) {
                    std::cout << "[" << logger::utility::get_current_timestamp() << "] Logger disconnected"
                              << std::endl;
                    stop();
                    return;
                } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
                    std::cerr << "Error reading from socket: " << strerror(errno) << std::endl;
                    stop();
                    return;
                }
                break;
            }

            client_buffer_.insert(client_buffer_.end(), buffer, buffer + bytes_read);
        }

        if (not client_buffer_.empty()) {
            process_message({client_buffer_.data(), client_buffer_.size()});
            client_buffer_.clear();
        }
    }

    void SocketServer::process_message(std::string_view message) {
        std::cout << "[" << logger::utility::get_current_timestamp() << "] LOG: " << message;
    }
} // namespace metrics_application
