#include "socket_sink.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

namespace logger {
    SocketSink::SocketSink(const std::string &host, int port) :
        socket_fd_(-1), host_(host), port_(port), is_connected_(false) {
        init_socket();
        connect_to_server();
    }

    SocketSink::~SocketSink() { cleanup_socket(); }

    void SocketSink::write(std::string_view message) {
        if (not is_valid()) {
            return;
        }

        std::lock_guard<std::mutex> lock(socket_mutex_);

        std::string full_message;
        full_message.reserve(message.size() + 1);
        full_message.append(message);
        full_message.push_back('\n');

        size_t total_sent = 0;
        size_t message_size = full_message.size();

        while (total_sent < message_size) {
            if (not wait_for_socket_ready(socket_fd_, true)) {
                is_connected_ = false;
                return;
            }

            ssize_t sent = send(socket_fd_, full_message.data() + total_sent, message_size - total_sent, MSG_NOSIGNAL);

            if (sent == -1) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    continue;
                }
                is_connected_ = false;
                return;
            }

            total_sent += sent;
        }
    }

    bool SocketSink::is_valid() const {
        std::lock_guard<std::mutex> lock(socket_mutex_);
        return socket_fd_ != -1 && is_connected_;
    }

    bool SocketSink::init_socket() {
        socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);

        if (socket_fd_ == -1) {
            return false;
        }

        if (not set_non_blocking(socket_fd_)) {
            close(socket_fd_);
            socket_fd_ = -1;
            return false;
        }

        return true;
    }

    void SocketSink::cleanup_socket() {
        std::lock_guard<std::mutex> lock(socket_mutex_);

        if (socket_fd_ != -1) {
            close(socket_fd_);
            socket_fd_ = -1;
        }

        is_connected_ = false;
    }

    bool SocketSink::connect_to_server() {
        if (socket_fd_ == -1) {
            return false;
        }

        sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port_);

        if (inet_pton(AF_INET, host_.data(), &server_addr.sin_addr) <= 0) {
            return false;
        }

        int result = connect(socket_fd_, (sockaddr *) &server_addr, sizeof(server_addr));
        if (result == 0) {
            is_connected_ = true;
            return true;
        }

        if (errno == EINPROGRESS) {
            if (wait_for_socket_ready(socket_fd_, true)) {
                int error;
                socklen_t len = sizeof(error);
                if (getsockopt(socket_fd_, SOL_SOCKET, SO_ERROR, &error, &len) == 0 && error == 0) {
                    is_connected_ = true;
                    return true;
                }
            }
        }

        return false;
    }

    bool SocketSink::set_non_blocking(int socket) {
        int flags = fcntl(socket, F_GETFL, 0);
        if (flags == -1) {
            return false;
        }

        flags |= O_NONBLOCK;
        return fcntl(socket, F_SETFL, flags) != -1;
    }

    bool SocketSink::wait_for_socket_ready(int socket, bool for_write) {
        pollfd pfd;
        pfd.fd = socket;
        pfd.events = for_write ? POLLOUT : POLLIN;
        pfd.revents = 0;

        int result = poll(&pfd, 1, POLL_TIMEOUT_MS);
        return result > 0 && (pfd.revents & (for_write ? POLLOUT : POLLIN));
    }
} // namespace logger
