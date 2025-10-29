#ifndef __LEAK_DETECTOR_H__
#define __LEAK_DETECTOR_H__

#include <cstddef>
#include <atomic>

class LeakDetector {
private:
    std::atomic<long long> bytes_in_use;

    LeakDetector();

public:
    LeakDetector(const LeakDetector&) = delete;
    LeakDetector& operator=(const LeakDetector&) = delete;

    static auto& get() {
        static LeakDetector singleton;
        return singleton;
    }

    void allocated(std::size_t allocated_bytes);
    void freed(std::size_t deallocated_bytes);

    std::size_t in_use() const;
    void reset();
};

void* operator new(std::size_t bytes_to_allocate);
void operator delete(void* p) noexcept;

#if __cplusplus >= 201402L
void operator delete(void *p, std::size_t) noexcept;
void operator delete[](void* p) noexcept;
void operator delete[](void *p, std::size_t n) noexcept;
#endif

#endif // __LEAK_DETECTOR_H__
