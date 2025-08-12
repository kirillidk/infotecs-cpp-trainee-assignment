set(LOGGER_TESTS "logger_tests")

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(${LOGGER_TESTS} ${TEST_SOURCES})

target_link_libraries(${LOGGER_TESTS} 
    PRIVATE 
    ${LOGGER_LIB}
    GTest::gtest
    GTest::gtest_main
)

target_compile_options(${LOGGER_TESTS} PRIVATE 
    "-Werror" "-Wall" "-Wextra" "-Wpedantic"
)

include(GoogleTest)
gtest_discover_tests(${LOGGER_TESTS})