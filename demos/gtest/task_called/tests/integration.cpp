// tests/FreeRTOSTest.cpp
#include <gtest/gtest.h>
#include <thread>
#include <chrono>

extern "C" {
    #include "FreeRTOS.h"
    #include "task.h"
}

#include <FreeRTOS/Task.hpp>
#include <FreeRTOS/Kernel.hpp>
#include "FreeRTOS/FreeRTOSHooks.h"

// Test
TEST(FreeRTOSTest, MinimalTaskRuns) {
    std::atomic<bool> finished(false);

    // Create a minimal FreeRTOS task that sets 'finished' to true
    // and attempts to stop the scheduler
    xTaskCreate(
        [](void* done) {
            auto* finishPtr = static_cast<std::atomic<bool>*>(done);
            *finishPtr = true;                  // Mark task as finished
            FreeRTOS::Kernel::endScheduler();   // Attempt to stop the scheduler
        },
        "TestTask",
        configMINIMAL_STACK_SIZE,
        &finished,             // Pass pointer to 'finished' variable
        tskIDLE_PRIORITY + 1,
        nullptr
    );

    // Launch FreeRTOS scheduler in a detached thread
    // Detached in case endScheduler() is never called, so the thread won't block forever
    std::thread schedulerThread([](){ 
        FreeRTOS::Kernel::startScheduler(); 
    });
    schedulerThread.detach();

    // Wait until 'finished' becomes true or timeout is reached
    const auto timeout = std::chrono::seconds(2);
    auto start = std::chrono::steady_clock::now();
    while (!finished && std::chrono::steady_clock::now() - start < timeout) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // polling interval
    }

    // At this point, the scheduler thread is detached; 
    // we can't join it, but 'finished' indicates if the task ran
    EXPECT_TRUE(finished);
}
