#pragma once

#include <memory>
#include <string>

namespace metrics_application {
    class SocketServer;
    class MessageProcessor;

    class MetricsApplication {
    public:
        [[nodiscard]] static std::unique_ptr<MetricsApplication>
        create_application(const std::string &host, int port, int message_interval, int timeout_seconds);

    public:
        ~MetricsApplication();

        void run();
        void stop();

    private:
        MetricsApplication(std::unique_ptr<SocketServer> socket_server,
                           std::unique_ptr<MessageProcessor> message_processor);

    private:
        std::unique_ptr<SocketServer> socket_server_;
        std::unique_ptr<MessageProcessor> message_processor_;
        bool is_running_;
    };
} // namespace metrics_application
