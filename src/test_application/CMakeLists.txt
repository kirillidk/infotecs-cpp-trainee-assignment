set(TEST_APPLICATION "test_application")

file(GLOB_RECURSE TEST_APPLICATION_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)
list(REMOVE_ITEM TEST_APPLICATION_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

add_library(${TEST_APPLICATION_LIB} STATIC ${TEST_APPLICATION_SOURCES})

target_include_directories(${TEST_APPLICATION_LIB} PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)

target_link_libraries(${TEST_APPLICATION_LIB} PRIVATE ${LOGGER_LIB})

target_compile_options(${TEST_APPLICATION_LIB} PUBLIC "-Werror" "-Wall" "-Wextra" "-Wpedantic" "-Wno-error=maybe-uninitialized")

add_executable(${TEST_APPLICATION} "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

target_link_libraries(${TEST_APPLICATION} PRIVATE ${TEST_APPLICATION_LIB} ${LOGGER_LIB})