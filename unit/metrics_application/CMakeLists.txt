set(METRICS_APPLICATION_TESTS "metrics_application_tests")

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(${METRICS_APPLICATION_TESTS} ${TEST_SOURCES})

target_link_libraries(${METRICS_APPLICATION_TESTS} 
    PRIVATE 
    ${METRICS_APPLICATION_LIB}
    ${LOGGER_LIB}
    GTest::gtest
    GTest::gtest_main
)

target_compile_options(${METRICS_APPLICATION_TESTS} PRIVATE 
    "-Werror" "-Wall" "-Wextra" "-Wpedantic"
)

include(GoogleTest)
gtest_discover_tests(${METRICS_APPLICATION_TESTS})