file(GLOB_RECURSE METRICS_APPLICATION_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

set(METRICS_APPLICATION "metrics_application")

add_executable(${METRICS_APPLICATION} ${METRICS_APPLICATION_SOURCES})

target_link_libraries(${METRICS_APPLICATION} PRIVATE ${LOGGER_LIB})

target_compile_options(${METRICS_APPLICATION} PRIVATE "-Werror" "-Wall" "-Wextra" "-Wpedantic")