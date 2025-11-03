cmake_minimum_required(VERSION 3.15)

# Project definition

set(EXEC_NAME demo_serial)
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
set(FREERTOS_CPP_DIR "${THIRD_PARTY_DIR}/wrappers/FreeRTOS-Cpp/FreeRTOS-Cpp")

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

# Executable definition

# Include user FreeRTOS hooks
set(FREERTOS_HOOKS ${UTILS_DIR}/FreeRTOS/FreeRTOSHooks.cpp)

# Collect all .cpp files in the target directories
file(GLOB_RECURSE PROJECT_SERVER_SRC CONFIGURE_DEPENDS "src/server/*.cpp")
file(GLOB_RECURSE PROJECT_CLIENT_SRC CONFIGURE_DEPENDS "src/client/*.cpp")
file(GLOB_RECURSE SERIAL_SRC CONFIGURE_DEPENDS "${UTILS_DIR}/Serial/*.cpp")

# Function:     create_executable
# Description:  Configures the executable, the compile definitions and a link the 
#               libraries.
# Usage:        create_executable(target_name source_list extra_sources)
function(create_executable TARGET_NAME PROJECT_SRC EXTRA_SRC)
    # Create executable
    string(REPLACE ";" "\n\t" CPP_SOURCES "${PROJECT_SRC};${EXTRA_SRC}")
    message(STATUS "Including source files for ${TARGET_NAME}:\n\t${CPP_SOURCES}")
    add_executable(${TARGET_NAME} ${FREERTOS_HOOKS} ${PROJECT_SRC} ${EXTRA_SRC})

    # Compile definitions
    target_compile_definitions(${TARGET_NAME} PUBLIC
        projENABLE_TRACING=0
        projCOVERAGE_TEST=0
    )

    # Link libraries
    target_link_libraries(${TARGET_NAME} pthread freertos_kernel freertos_config)
endfunction()

# Create the final executable

# SERVER
create_executable("${EXEC_NAME}_server" "${PROJECT_SERVER_SRC}" "${SERIAL_SRC}")

# CLIENT
create_executable("${EXEC_NAME}_client" "${PROJECT_CLIENT_SRC}" "${SERIAL_SRC}")

message(STATUS "[DONE] Project '${EXEC_NAME}' configuration.")