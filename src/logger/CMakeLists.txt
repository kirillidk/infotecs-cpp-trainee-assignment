file(GLOB_RECURSE LOGGER_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

option(BUILD_SHARED "Build shared library instead of static" OFF)

if(BUILD_SHARED)
    add_library(${LOGGER_LIB} SHARED ${LOGGER_SOURCES})
    message(STATUS "Building SHARED library")
else()
    add_library(${LOGGER_LIB} STATIC ${LOGGER_SOURCES})
    message(STATUS "Building STATIC library")
endif()

target_include_directories(${LOGGER_LIB} PUBLIC 
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
)

target_compile_options(${LOGGER_LIB} PUBLIC "-Werror" "-Wall" "-Wextra" "-Wpedantic" "-Wno-error=maybe-uninitialized")