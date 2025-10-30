cmake_minimum_required(VERSION 3.15)

project(demo_serial LANGUAGES C CXX)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

add_compile_options(-Wall -Wextra -Wpedantic -D_WINDOWS_)

set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")

set(FREERTOS_DIR "${CMAKE_SOURCE_DIR}/../../FreeRTOS/FreeRTOS")
set(FREERTOS_CPP_DIR "${CMAKE_SOURCE_DIR}/../../FreeRTOS-Cpp/FreeRTOS-Cpp")
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
    ${CMAKE_SOURCE_DIR}/../../utils
)

set(PROJECT_SRC
    src/main.cpp
    src/FreeRTOS/FreeRTOSHooks.cpp
    src/tasks/Blink.cpp
)

set(SERIAL_SRC
    ${CMAKE_SOURCE_DIR}/../../utils/Serial/BaseChannel.cpp
    ${CMAKE_SOURCE_DIR}/../../utils/Serial/Client.cpp
    ${CMAKE_SOURCE_DIR}/../../utils/Serial/ChannelHandle.cpp
)

add_executable(demo_serial ${PROJECT_SRC} ${SERIAL_SRC})

target_compile_definitions(demo_serial PUBLIC
    projENABLE_TRACING=0
    projCOVERAGE_TEST=0
)

target_link_libraries(demo_serial
    pthread
    freertos_kernel
    freertos_config
)
