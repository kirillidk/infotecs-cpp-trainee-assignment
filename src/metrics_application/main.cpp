#include <iostream>
#include <memory>

#include <logger/logger.hpp>

#include "metrics_application.hpp"
#include "utility.hpp"

int main(int argc, char *argv[]) {
    using namespace metrics_application;

    if (argc != 3) {
        print_usage(argv[0]);
        return 1;
    }

    std::string host = argv[1];
    int port;

    try {
        port = std::stoi(argv[2]);
        if (port <= 0 || port > 65535) {
            std::cerr << "Error: Port must be between 1 and 65535" << std::endl;
            return 1;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: Invalid port number '" << argv[2] << "'" << std::endl;
        return 1;
    }

    std::unique_ptr<MetricsApplication> metricsApplication = MetricsApplication::create_application(host, port);

    metricsApplication->run();

    return 0;
}
