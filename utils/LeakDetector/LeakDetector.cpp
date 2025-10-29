#include "LeakDetector.h"
#include <cstdlib>
#include <new>

LeakDetector::LeakDetector() : bytes_in_use{0LL} {}

void LeakDetector::allocated(std::size_t allocated_bytes) {
    bytes_in_use += allocated_bytes;
}

void LeakDetector::freed(std::size_t deallocated_bytes) {
    bytes_in_use -= deallocated_bytes;
}

std::size_t LeakDetector::in_use() const {
    return bytes_in_use.load();
}

void LeakDetector::reset() {
    bytes_in_use.store(0LL);
}

void* operator new(std::size_t bytes_to_allocate) {
    void* p = std::malloc(sizeof(std::max_align_t) + bytes_to_allocate);
    if (!p) throw std::bad_alloc{};

    new (p) std::size_t{bytes_to_allocate};
    LeakDetector::get().allocated(bytes_to_allocate);
    return static_cast<std::max_align_t*>(p) + 1;
}

void operator delete(void* p) noexcept {
    if (!p) return;

    p = static_cast<std::max_align_t*>(p) - 1;
    LeakDetector::get().freed(*static_cast<std::size_t*>(p));
    std::free(p);
}

#if __cplusplus >= 201402L
void operator delete(void *p, std::size_t) noexcept {
    if (!p) return;

    p = static_cast<std::max_align_t*>(p) - 1;
    LeakDetector::get().freed(*static_cast<std::size_t*>(p));
    std::free(p);
}

void operator delete[](void* p) noexcept {
    ::operator delete(p);
}

void operator delete[](void *p, std::size_t n) noexcept {
    ::operator delete(p, n);
}
#endif