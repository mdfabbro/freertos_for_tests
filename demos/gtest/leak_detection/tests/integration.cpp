#include <gtest/gtest.h>

extern "C" {
    #include "FreeRTOS.h"
    #include "task.h"
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

    LeakDetector::get().reset();
    auto pre = LeakDetector::get().in_use();

    xTaskCreate(
        [](void* ){
            // Allocate array of objects
            ClassUnderTest* buffer = new ClassUnderTest[100]{};
            // Clean up
            delete[] buffer;
            FreeRTOS::Kernel::endScheduler();
        },
        "MemoryTask",
        configMINIMAL_STACK_SIZE,
        nullptr,
        tskIDLE_PRIORITY + 1,
        nullptr
    );

    // Launch scheduler detached
    const auto timeout = runner.waitTask(maxWait);
    const auto post = LeakDetector::get().in_use();
    return !timeout && (post == pre);
}


TEST(FreeRTOSTest, MinimalTask_NoLeak) {
    bool ok = runTaskWithLeakCheck<int>();  // int will not leak
    EXPECT_TRUE(ok);

}

TEST(FreeRTOSTest, MinimalTask_LeakerLeaks) {
    bool ok = runTaskWithLeakCheck<Leaker>();  // Leaker leaks
    EXPECT_FALSE(ok);
}