#ifndef __FREERTOS_RUNNER_H__
#define __FREERTOS_RUNNER_H__

#include <thread>
#include <atomic>
#include <chrono>
#include <FreeRTOS/Kernel.hpp>

class FreeRTOSRunner {
public:
    // Waits until the scheduler ends or timeout...
    bool waitTask(std::chrono::milliseconds timeout = std::chrono::milliseconds(2000)) {
        startScheduler();
        const auto start = std::chrono::steady_clock::now();
        while (!done.load() && 
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start) < timeout) 
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        return !done.load(); // timeout
    }

private:
    std::thread schedulerThread;
    std::atomic<bool> done {false};
    // Launches the scheduler
    void startScheduler() {
        std::thread([this] {
            FreeRTOS::Kernel::startScheduler();
            done.store(true);
        }).detach();
    }
};

#endif // __FREERTOS_RUNNER_H__