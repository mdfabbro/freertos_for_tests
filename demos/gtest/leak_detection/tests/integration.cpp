#include <gtest/gtest.h>

extern "C" {
    #include "FreeRTOS.h"
    #include "task.h"
    #include "queue.h"
}

#include <FreeRTOS/Task.hpp>
#include <FreeRTOS/Kernel.hpp>
#include "FreeRTOS/FreeRTOSHooks.h"
#include "FreeRTOS/FreeRTOSRunner.h"
#include "LeakDetector/LeakDetector.h"

// This is meant to leak! 
class Leaker {
private:
    float* neverFree = nullptr;
public:
    Leaker() : neverFree(new float(0.0f)) {}
};

// Template function to run a FreeRTOS task with any class
template<typename ClassUnderTest>
bool runTaskWithLeakCheck(std::chrono::milliseconds maxWait = std::chrono::milliseconds(1000)) {
    FreeRTOSRunner runner{};

    // The system uses two distinct allocators:
    //   1. System Heap: Standard C++ allocations via new/delete.
    //   2. RTOS Heap:   FreeRTOS internal allocations (tasks, queues,
    //                   timers...) via pvPortMalloc/vPortFree.
    // We track both using independent LeakDetectors to ensure that:
    //   - No user-level heap leaks remain after the test.
    //   - No internal RTOS allocations are left unfreed.

    // Reset and record baseline for System Heap (C++ new/delete)
    LeakDetectors::HeapSystem().reset();
    const auto pre = LeakDetectors::HeapSystem().in_use();

     // Reset and record baseline for RTOS Heap (FreeRTOS objects)
    TaskHandle_t pxCreatedTask;
    LeakDetectors::HeapRTOS().reset();
    const auto preRTOS = LeakDetectors::HeapRTOS().in_use();

    // The task will:
    //   - Allocate and destroy an array of ClassUnderTest instances.
    //   - Create and delete a FreeRTOS queue (to exercise the RTOS heap).
    //   - End the scheduler once finished, allowing the test to continue.
    xTaskCreate(
        [](void* ){
            // Allocate C++ objects (system heap)
            ClassUnderTest* buffer = new ClassUnderTest[100]{};
            
            // Allocate a FreeRTOS object (RTOS heap)
            QueueHandle_t q = xQueueCreate(10, sizeof(int));
            vQueueDelete(q);
            
            // Clean up user allocations
            delete[] buffer;
            FreeRTOS::Kernel::endScheduler();
        },
        "MemoryTask",
        configMINIMAL_STACK_SIZE,
        nullptr,
        tskIDLE_PRIORITY + 1,
        &pxCreatedTask
    );

    // Launch scheduler detached
    const auto timeout = runner.waitTask(maxWait);
    const auto post = LeakDetectors::HeapSystem().in_use();

    vTaskDelete(pxCreatedTask); // Ensure task control block is freed
    const auto postRTOS = LeakDetectors::HeapRTOS().in_use();

    // Return true only if:
    //   - The scheduler completed (no timeout).
    //   - The system heap returned to baseline.
    //   - The RTOS heap returned to baseline.
    return !timeout && (post == pre) && (postRTOS == preRTOS);
}

TEST(FreeRTOSTest, MinimalTask_NoLeak) {
    bool ok = runTaskWithLeakCheck<int>();  // int will not leak
    EXPECT_TRUE(ok);

}

TEST(FreeRTOSTest, MinimalTask_LeakerLeaks) {
    bool ok = runTaskWithLeakCheck<Leaker>();  // Leaker leaks
    EXPECT_FALSE(ok);
}

TEST(FreeRTOSTest, MinimalTask_QueueLeak) {
    // Similar to the tests using runTaskWithLeakCheck(), but here we 
    // intentionally omit vQueueDelete() to simulate a memory leak in 
    // the RTOS heap.

    FreeRTOSRunner runner{};

    TaskHandle_t pxCreatedTask;
    LeakDetectors::HeapRTOS().reset();
    const auto preRTOS = LeakDetectors::HeapRTOS().in_use();

    xTaskCreate(
        [](void* ){
            // Allocate a FreeRTOS object (RTOS heap)
            xQueueCreate(10, sizeof(int));
            // (MISSING!) vQueueDelete(q); // <- To create the leak!
            FreeRTOS::Kernel::endScheduler();
        },
        "QueueLeakMemoryTask",
        configMINIMAL_STACK_SIZE,
        nullptr,
        tskIDLE_PRIORITY + 1,
        &pxCreatedTask
    );

    // Launch scheduler detached
    const auto timeout = runner.waitTask(std::chrono::milliseconds(1000));
    vTaskDelete(pxCreatedTask); // Ensure task control block is freed
    const auto postRTOS = LeakDetectors::HeapRTOS().in_use();

    EXPECT_FALSE(timeout);
    EXPECT_NE(postRTOS, preRTOS); // The test expects a mismatch (leak detected)
}
