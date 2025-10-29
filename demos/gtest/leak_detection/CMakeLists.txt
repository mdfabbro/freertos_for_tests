cmake_minimum_required(VERSION 3.15)
project(demo_blink LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

add_compile_options(-Wall -Wextra -Wpedantic -D_WINDOWS_)

set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")

set(FREERTOS_DIR "${CMAKE_SOURCE_DIR}/../../../FreeRTOS/FreeRTOS")
set(FREERTOS_CPP_DIR "${CMAKE_SOURCE_DIR}/../../../FreeRTOS-Cpp/FreeRTOS-Cpp")
set(FREERTOS_KERNEL_PATH "${FREERTOS_DIR}/Source")

message(STATUS "Using FREERTOS_DIR: ${FREERTOS_DIR}")
message(STATUS "Using FREERTOS_CPP_DIR: ${FREERTOS_CPP_DIR}")

set(FREERTOS_HEAP "3" CACHE STRING "" FORCE)
set(FREERTOS_PORT "GCC_POSIX" CACHE STRING "" FORCE)

add_library(freertos_config INTERFACE)
target_compile_definitions(freertos_config INTERFACE
    projENABLE_TRACING=0
    projCOVERAGE_TEST=0
)
target_include_directories(freertos_config INTERFACE
    ${CMAKE_SOURCE_DIR}/src/FreeRTOS
    ${FREERTOS_DIR}/Source/include
    ${FREERTOS_DIR}/Demo/Common/include
)

add_subdirectory(${FREERTOS_KERNEL_PATH} ${CMAKE_CURRENT_BINARY_DIR}/FreeRTOS-Kernel)
target_compile_options(freertos_kernel PRIVATE -Wno-pointer-to-int-cast)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${FREERTOS_CPP_DIR}/include
    ${CMAKE_SOURCE_DIR}/../../../utils
)

set(PROJECT_SRC
    src/FreeRTOS/FreeRTOSHooks.cpp
)

# GOOGLE TEST

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

set(LEAK_SRC
    ${CMAKE_SOURCE_DIR}/../../../utils/LeakDetector/LeakDetector.cpp
)


set(TEST_SRC
    tests/integration.cpp
)

add_executable(demo_leak_detector ${TEST_SRC} ${PROJECT_SRC} ${LEAK_SRC})

target_compile_definitions(demo_leak_detector PUBLIC
    projENABLE_TRACING=0
    projCOVERAGE_TEST=0
)

target_link_libraries(demo_leak_detector
    gtest_main
    freertos_kernel
    freertos_config
    pthread
)

add_test(NAME demo_leak_detector COMMAND demo_leak_detector)

message(STATUS "")
message(STATUS "Targets configurados:")
message(STATUS " - demo_leak_detector:  Calls a task to make sure and changes a value.")
message(STATUS "")
