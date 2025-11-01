cmake_minimum_required(VERSION 3.15)

project(demo_blink LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

add_compile_options(-Wall -Wextra -Wpedantic -D_WINDOWS_)

set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")

set(FREERTOS_DIR "${CMAKE_SOURCE_DIR}/../../FreeRTOS/FreeRTOS")
set(FREERTOS_CPP_DIR "${CMAKE_SOURCE_DIR}/../../FreeRTOS-Cpp/FreeRTOS-Cpp")
set(FREERTOS_KERNEL_PATH "${FREERTOS_DIR}/Source")

set(UTILS_DIR "${CMAKE_SOURCE_DIR}/../../utils")

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
    ${UTILS_DIR}/FreeRTOS
    ${FREERTOS_DIR}/Source/include
    ${FREERTOS_DIR}/Demo/Common/include
)

add_subdirectory(${FREERTOS_KERNEL_PATH} ${CMAKE_CURRENT_BINARY_DIR}/FreeRTOS-Kernel)

target_compile_options(freertos_kernel PRIVATE -Wno-pointer-to-int-cast)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${FREERTOS_CPP_DIR}/include
    ${UTILS_DIR}
)

set(PROJECT_SRC
    src/main.cpp
    ${UTILS_DIR}/FreeRTOS/FreeRTOSHooks.cpp
    src/tasks/Blink.cpp
)

add_executable(demo_blink ${PROJECT_SRC})

target_compile_definitions(demo_blink PUBLIC
    projENABLE_TRACING=0
    projCOVERAGE_TEST=0
)

target_link_libraries(demo_blink
    pthread
    freertos_kernel
    freertos_config
)
