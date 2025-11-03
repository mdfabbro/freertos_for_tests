cmake_minimum_required(VERSION 3.15)

# Project definition

set(EXEC_NAME demo_gtest_leak_detection)
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
set(FREERTOS_FOR_TESTS_DIR "${CMAKE_SOURCE_DIR}/../../..")

# Third-party dependencies
set(THIRD_PARTY_DIR "${FREERTOS_FOR_TESTS_DIR}/third_party")

# FreeRTOS and FreeRTOS-Cpp locations
set(FREERTOS_DIR "${THIRD_PARTY_DIR}/FreeRTOS/FreeRTOS")
set(FREERTOS_KERNEL_PATH "${FREERTOS_DIR}/Source")
set(FREERTOS_CPP_DIR "${THIRD_PARTY_DIR}/FreeRTOS-Cpp/FreeRTOS-Cpp")

# Utilities (common helper code and hooks)
set(UTILS_DIR "${FREERTOS_FOR_TESTS_DIR}/utils")

message(STATUS "Directories:")
message(STATUS "\t- Using FREERTOS_CPP_DIR:       ${FREERTOS_CPP_DIR}")
message(STATUS "\t- Using UTILS_DIR:              ${UTILS_DIR}")

# Include directories

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${FREERTOS_CPP_DIR}/include
    ${UTILS_DIR}
)

# Adding the FreeRTOS
add_subdirectory(${UTILS_DIR}/FreeRTOS ${CMAKE_CURRENT_BINARY_DIR}/FreeRTOS)

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

# Include user FreeRTOS hooks
set(FREERTOS_HOOKS ${UTILS_DIR}/FreeRTOS/FreeRTOSHooks.cpp)

# Collect all .cpp files in the target directories
file(GLOB_RECURSE PROJECT_SRC CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE TESTS_SRC CONFIGURE_DEPENDS "tests/*.cpp")
file(GLOB_RECURSE LEAK_DETECTOR_SRC CONFIGURE_DEPENDS "${UTILS_DIR}/LeakDetector/*.cpp")


# Create the final executable
string(REPLACE ";" "\n\t" CPP_SOURCES "${PROJECT_SRC};${TESTS_SRC};${LEAK_DETECTOR_SRC}")
message(STATUS "Including sources files:\n\t${CPP_SOURCES}")
add_executable(${EXEC_NAME} ${FREERTOS_HOOKS} ${PROJECT_SRC} ${TESTS_SRC} ${LEAK_DETECTOR_SRC})

# Public compile definitions for the main target
target_compile_definitions(${EXEC_NAME} PUBLIC
    projENABLE_TRACING=0
    projCOVERAGE_TEST=0
)

# Link against necessary libraries
target_link_libraries(${EXEC_NAME} gtest_main pthread freertos_kernel freertos_config)

# Adding test
add_test(NAME ${EXEC_NAME} COMMAND ${EXEC_NAME})

message(STATUS "[DONE] Project '${EXEC_NAME}' configuration.")
message(STATUS "")
message(STATUS "Targets configured:")
message(STATUS " - ${EXEC_NAME}:  Runs a basic example with google test")
message(STATUS "")
