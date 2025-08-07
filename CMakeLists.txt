cmake_minimum_required(VERSION 3.17)
project(infotecs-cpp-trainee-assignment VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LOGGER_LIB "${CMAKE_PROJECT_NAME}_logger_lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src/logger)
add_subdirectory(src/test_application)