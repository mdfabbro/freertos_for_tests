cmake_minimum_required(VERSION 3.15)

message(STATUS "Using utils/FreeRTOS")

# FreeRTOS build configuration

set(FREERTOS_HEAP "3" CACHE STRING "" FORCE)            # Use heap_3.c (malloc/free)
set(FREERTOS_PORT "GCC_POSIX" CACHE STRING "" FORCE)    # POSIX port for Linux builds

# FreeRTOS and FreeRTOS-Cpp locations
set(FREERTOS_SOURCE_DIR      "../../third_party/FreeRTOS/FreeRTOS/Source")
set(FREERTOS_DEMO_COMMON_DIR "../../third_party/FreeRTOS/FreeRTOS/Demo/Common")

message(STATUS "From 'freertos_config', we are using the directories:")
message(STATUS "\t- Using FREERTOS_SOURCE_DIR:      ${FREERTOS_SOURCE_DIR}")
message(STATUS "\t- Using FREERTOS_DEMO_COMMON_DIR: ${FREERTOS_DEMO_COMMON_DIR}")


# Create an INTERFACE library for FreeRTOS configuration
add_library(freertos_config INTERFACE)

target_compile_definitions(freertos_config INTERFACE
    projENABLE_TRACING=0
    projCOVERAGE_TEST=0
)

target_include_directories(freertos_config INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${FREERTOS_SOURCE_DIR}/include
    ${FREERTOS_DEMO_COMMON_DIR}/include
)

# FreeRTOS kernel

# Adds the FreeRTOS Kernel as a subdirectory (builds freertos_kernel target)
add_subdirectory(${FREERTOS_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/FreeRTOS-Kernel)

# Disable specific warnings from the FreeRTOS source
target_compile_options(freertos_kernel PRIVATE -Wno-pointer-to-int-cast)
