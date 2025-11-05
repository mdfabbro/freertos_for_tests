#ifndef __LEAK_DETECTOR_H__
#define __LEAK_DETECTOR_H__

#include <atomic>
#include <cstddef>

class LeakDetector {
private:
    std::atomic<long long> bytes_in_use;

public:
    LeakDetector();
    ~LeakDetector() = default;

    LeakDetector(const LeakDetector&) = delete;
    LeakDetector& operator=(const LeakDetector&) = delete;

    void allocated(std::size_t allocated_bytes);
    void freed(std::size_t deallocated_bytes);

    std::size_t in_use() const;
    void reset();
};

// Two separate global singletons for different contexts
namespace LeakDetectors {
    LeakDetector& HeapSystem();
    LeakDetector& HeapRTOS();
}

#endif // __LEAK_DETECTOR_H__
