set(TEST_APPLICATION_TESTS "test_application_tests")

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(${TEST_APPLICATION_TESTS} ${TEST_SOURCES})

target_link_libraries(${TEST_APPLICATION_TESTS} 
    PRIVATE 
    ${TEST_APPLICATION_LIB}
    ${LOGGER_LIB}
    GTest::gtest
    GTest::gtest_main
)

target_compile_options(${TEST_APPLICATION_TESTS} PRIVATE 
    "-Werror" "-Wall" "-Wextra" "-Wpedantic"
)

include(GoogleTest)
gtest_discover_tests(${TEST_APPLICATION_TESTS})