#include "metrics_application.hpp"

#include <iostream>
#include "message_processor.hpp"
#include "socket_server.hpp"

namespace metrics_application {
    std::unique_ptr<MetricsApplication> MetricsApplication::create_application(const std::string &host, int port,
                                                                               int message_interval,
                                                                               int timeout_seconds) {

        auto socket_server = std::make_unique<SocketServer>(host, port);
        auto message_processor = std::make_unique<MessageProcessor>(message_interval, timeout_seconds);

        if (not socket_server->start()) {
            std::cerr << "Failed to start socket server on " << host << ":" << port << std::endl;
            return nullptr;
        }

        std::cout << "Stats will be printed every " << message_interval << " messages" << std::endl;
        std::cout << "or after " << timeout_seconds << " seconds of inactivity" << std::endl;

        return std::unique_ptr<MetricsApplication>(
                new MetricsApplication(std::move(socket_server), std::move(message_processor)));
    }

    MetricsApplication::MetricsApplication(std::unique_ptr<SocketServer> socket_server,
                                           std::unique_ptr<MessageProcessor> message_processor) :
        socket_server_(std::move(socket_server)), message_processor_(std::move(message_processor)), is_running_(false) {
    }

    MetricsApplication::~MetricsApplication() { stop(); }

    void MetricsApplication::run() {
        if (not socket_server_ || not message_processor_) {
            std::cerr << "Socket server or message processor not available" << std::endl;
            return;
        }

        is_running_ = true;

        // Setup callback from socket server to message processor
        socket_server_->set_message_callback(
                [this](std::string_view message) { message_processor_->process_message(message); });

        // Start message processor
        message_processor_->start();

        // Run socket server (blocking)
        socket_server_->run();
    }

    void MetricsApplication::stop() {
        if (is_running_) {
            is_running_ = false;

            if (message_processor_) {
                message_processor_->stop();
            }

            if (socket_server_) {
                socket_server_->stop();
            }
        }
    }
} // namespace metrics_application
