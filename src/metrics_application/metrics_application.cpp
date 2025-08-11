#include "metrics_application.hpp"

#include <iostream>
#include "socket_server.hpp"

namespace metrics_application {
    std::unique_ptr<MetricsApplication> MetricsApplication::create_application(const std::string &host, int port) {
        auto socket_server = std::make_unique<SocketServer>(host, port);

        if (not socket_server->start()) {
            std::cerr << "Failed to start socket server on " << host << ":" << port << std::endl;
            return nullptr;
        }

        return std::unique_ptr<MetricsApplication>(new MetricsApplication(std::move(socket_server)));
    }

    MetricsApplication::MetricsApplication(std::shared_ptr<SocketServer> socket_server) :
        socket_server_(std::move(socket_server)), is_running_(false) {}

    MetricsApplication::~MetricsApplication() { stop(); }

    void MetricsApplication::run() {
        if (not socket_server_) {
            std::cerr << "No socket server available" << std::endl;
            return;
        }

        is_running_ = true;
        socket_server_->run();
    }

    void MetricsApplication::stop() {
        if (is_running_ && socket_server_) {
            is_running_ = false;
            socket_server_->stop();
        }
    }
} // namespace metrics_application
