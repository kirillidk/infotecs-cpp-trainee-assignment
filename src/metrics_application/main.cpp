#include <iostream>
#include <memory>

#include <logger/logger.hpp>

#include "metrics_application.hpp"
#include "utility.hpp"

int main(int argc, char *argv[]) {
    using namespace metrics_application;

    if (argc != 5) {
        utility::print_usage(argv[0]);
        return 1;
    }

    std::string host = argv[1];
    int port, message_interval, timeout_seconds;

    try {
        port = std::stoi(argv[2]);
        if (port <= 0 || port > 65535) {
            std::cerr << "Error: Port must be between 1 and 65535" << std::endl;
            return 1;
        }

        message_interval = std::stoi(argv[3]);
        if (message_interval <= 0) {
            std::cerr << "Error: Message interval N must be positive" << std::endl;
            return 1;
        }

        timeout_seconds = std::stoi(argv[4]);
        if (timeout_seconds <= 0) {
            std::cerr << "Error: Timeout T must be positive" << std::endl;
            return 1;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: Invalid parameter - " << e.what() << std::endl;
        return 1;
    }

    std::unique_ptr<MetricsApplication> metricsApplication =
            MetricsApplication::create_application(host, port, message_interval, timeout_seconds);

    if (not metricsApplication) {
        std::cerr << "Failed to create Metrics application" << std::endl;
        return 1;
    }

    metricsApplication->run();

    return 0;
}
