#pragma once

#include <memory>
#include <string>

namespace metrics_application {
    class SocketServer;

    class MetricsApplication {
    public:
        [[nodiscard]] static std::unique_ptr<MetricsApplication> create_application(const std::string &host, int port);
        ~MetricsApplication();

        void run();
        void stop();

    private:
        MetricsApplication(std::shared_ptr<SocketServer> socket_server);

    private:
        std::shared_ptr<SocketServer> socket_server_;
        bool is_running_;
    };
} // namespace metrics_application
