cmake_minimum_required(VERSION 3.15)

# Project definition

set(EXEC_NAME demo_utils_strings)
project(${EXEC_NAME} LANGUAGES C CXX)

message(STATUS "[    ] Project '${EXEC_NAME}' configuration.")

# C++ configuration

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

# Compiler options and flags

add_compile_options(-Wall -Wextra -Wpedantic -D_WINDOWS_)
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")

# Directory layout

# Root of the FreeRTOS-for-tests environment
set(FREERTOS_FOR_TESTS_DIR "${CMAKE_SOURCE_DIR}/../../../..")

# Utilities (common helper code and hooks)
set(UTILS_DIR "${FREERTOS_FOR_TESTS_DIR}/utils")

message(STATUS "Directories:")
message(STATUS "\t- Using UTILS_DIR:              ${UTILS_DIR}")

# Google Test

# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Include CTest and enable tests
include(CTest)
enable_testing()

# Executable definition

# Collect all .cpp files in the target directories
file(GLOB_RECURSE QUEUE_SRC CONFIGURE_DEPENDS "${UTILS_DIR}/Queue/*.cpp")
file(GLOB_RECURSE LEAK_DETECTOR_SRC CONFIGURE_DEPENDS "${UTILS_DIR}/LeakDetector/*.cpp")
file(GLOB_RECURSE TESTS_SRC CONFIGURE_DEPENDS "tests/*.cpp")

string(REPLACE ";" "\n\t" CPP_SOURCES "${PROJECT_SRC};${TESTS_SRC};${LEAK_DETECTOR_SRC}")
message(STATUS "Including sources files:\n\t${CPP_SOURCES}\n\t")

# Add Test Executable
add_executable(${EXEC_NAME}_test ${TESTS_SRC} ${QUEUE_SRC} ${LEAK_DETECTOR_SRC})

target_include_directories(${EXEC_NAME}_test PRIVATE
    ${CMAKE_SOURCE_DIR}/tests/mocks
    ${UTILS_DIR}
)

# Public compile definitions for the main target
target_compile_definitions(${EXEC_NAME}_test PUBLIC
    projENABLE_TRACING=0
    projCOVERAGE_TEST=0
)

# Link against necessary libraries
target_link_libraries(${EXEC_NAME}_test gtest_main pthread)

# Adding test
add_test(NAME ${EXEC_NAME}_test COMMAND ${EXEC_NAME}_test)

message(STATUS "[DONE] Project '${EXEC_NAME}' configuration.")