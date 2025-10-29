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
bool runTaskWithLeakCheck(std::chrono::seconds timeout = std::chrono::seconds(2)) {
    std::atomic<bool> finished(false);

    LeakDetector::get().reset();
    auto pre = LeakDetector::get().in_use();

    xTaskCreate(
        [](void* done){
            auto* finishPtr = static_cast<std::atomic<bool>*>(done);

            // Allocate array of objects
            ClassUnderTest* buffer = new ClassUnderTest[100]{};

            // Clean up
            delete[] buffer;

            *finishPtr = true;
            FreeRTOS::Kernel::endScheduler();
        },
        "MemoryTask",
        configMINIMAL_STACK_SIZE,
        &finished,
        tskIDLE_PRIORITY + 1,
        nullptr
    );

    // Launch scheduler detached
    std::thread([](){ FreeRTOS::Kernel::startScheduler(); }).detach();

    // Wait until finished or timeout
    auto start = std::chrono::steady_clock::now();
    while (!finished && std::chrono::steady_clock::now() - start < timeout) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    auto post = LeakDetector::get().in_use();
    return (post == pre); // true if no leak
}


TEST(FreeRTOSTest, MinimalTask_NoLeak) {
    bool ok = runTaskWithLeakCheck<int>();  // int will not leak
    EXPECT_TRUE(ok);
}

TEST(FreeRTOSTest, MinimalTask_LeakerLeaks) {
    bool ok = runTaskWithLeakCheck<Leaker>();  // Leaker leaks
    EXPECT_FALSE(ok);
}