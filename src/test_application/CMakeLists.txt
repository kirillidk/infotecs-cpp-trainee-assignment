file(GLOB_RECURSE TEST_APPLICATION_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

set(TEST_APPLICATION "test_application")

add_executable(${TEST_APPLICATION} ${TEST_APPLICATION_SOURCES})

target_link_libraries(${TEST_APPLICATION} PRIVATE ${LOGGER_LIB})

target_compile_options(${TEST_APPLICATION} PRIVATE "-Werror" "-Wall" "-Wextra" "-Wpedantic")