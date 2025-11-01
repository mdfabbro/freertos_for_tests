#include <gtest/gtest.h>

extern "C" {
    #include "FreeRTOS.h"
    #include "task.h"
}

#include <FreeRTOS/Task.hpp>
#include <FreeRTOS/Kernel.hpp>
#include "FreeRTOS/FreeRTOSHooks.h"
#include "FreeRTOS/FreeRTOSRunner.h"

// Test
TEST(FreeRTOSTest, MinimalTaskRuns) {
    std::atomic<bool> finished(false);
    ;

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

    // Wait until 'endScheduler' or timeout!
    const auto timeout = FreeRTOSRunner{}.waitTask(); 
    
    // At this point, the scheduler thread is detached; 
    // we can't join it, but 'finished' indicates if the task ran
    EXPECT_TRUE(!timeout && finished);
}
