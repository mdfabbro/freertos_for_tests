extern "C" {
    #include "FreeRTOS.h"
    #include "task.h"
}

#include <FreeRTOS/Task.hpp>
#include <FreeRTOS/Kernel.hpp>
#include "FreeRTOS/FreeRTOSHooks.h"

#include <gtest/gtest.h>

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}