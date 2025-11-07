#ifndef __QUEUE_DEQUE_FIXED_H__
#define __QUEUE_DEQUE_FIXED_H__

#include "Interfaces.h"
#include <cstdio>

template <typename T, size_t Size>
class DequeFixed : public IDeque<T> { 
public:

    ~DequeFixed() {
        while(length) {
            pop_front();
        }
    }

    DequeFixed() = default;
    
    DequeFixed(const DequeFixed & ) = delete;
    DequeFixed& operator=(const DequeFixed & ) = delete;

    DequeFixed( DequeFixed && toMove) {
        this->start = toMove.start;
        this->length = toMove.length;
        for(size_t i = toMove.start; i != toMove.length ; ++i) {
            this->list[i%Size] = toMove.list[i%Size];
            toMove.list[i%Size] = nullptr; // not really needed
        }
        toMove.start = toMove.length = 0;
    }

    DequeFixed& operator=( DequeFixed && toMove) {
        if( this != &toMove ) {
            this->start = toMove.start;
            this->length = toMove.length;
            for(size_t i = toMove.start; i != toMove.length ; ++i) {
                this->list[i%Size] = toMove.list[i%Size];
                toMove.list[i%Size] = nullptr; // not really needed
            }
            toMove.start = toMove.length = 0;
        }
        return *this;
    }

    T* front() override { 
        if( !length ) {
            return nullptr;
        }
        return list[start];
    }
    T* back() override {
        if( !length ) {
            return nullptr;
        }
        return list[(start + length - 1)%Size];
    }

    bool push_back(T* element) override {
        if( length == Size ) {
            return false; // No more space
        }
        ++length;
        list[(start + length - 1)%Size] = element;
        return true; 
    }
    bool push_front(T* element) override {
        if( length == Size ) {
            return false; // No more space
        }
        start = start ? (start - 1) : (Size - 1);
        list[start] = element;
        ++length;
        return true;
    }
    bool pop_back() override {
        if( !length ) {
            return false;
        }
        list[(start + length)%Size] = nullptr; // Optional
        --length;
        return true;
    }
    bool pop_front() override  {
        if( !length ) {
            return false;
        }
        list[start] = nullptr; // Optional
        start = (start + 1) % Size;
        --length;
        return true;
    }

private:
    T* list[Size] {nullptr};
    size_t start{ 0 };
    size_t length { 0 };
};


// In FreeRTOS a queue without elements is used as a mutex.
// So we will support DequeFixed<void, Size>.
template <size_t Size>
class DequeFixed<void, Size> : public IDeque<void> { 
public:

    ~DequeFixed() {
        while(length) {
            pop_front();
        }
    }

    DequeFixed() = default;
    
    DequeFixed(const DequeFixed & ) = delete;
    DequeFixed& operator=(const DequeFixed & ) = delete;

    DequeFixed( DequeFixed && toMove) {
        this->start = toMove.start;
        this->length = toMove.length;
        for(size_t i = toMove.start; i != toMove.length ; ++i) {
            this->list[i%Size] = toMove.list[i%Size];
            toMove.list[i%Size] = nullptr; // not really needed
        }
        toMove.start = toMove.length = 0;
    }

    DequeFixed& operator=( DequeFixed && toMove) {
        this->start = toMove.start;
        this->length = toMove.length;
        for(size_t i = toMove.start; i != toMove.length ; ++i) {
            this->list[i%Size] = toMove.list[i%Size];
            toMove.list[i%Size] = nullptr; // not really needed
        }
        toMove.start = toMove.length = 0;
        return *this;
    }

    void* front() override { return nullptr; }
    void* back() override { return nullptr; }

    bool push_back(void*  = nullptr) override {
        if( length == Size ) {
            return false; // No more space
        }
        ++length;
        return true; 
    }
    bool push_front(void* = nullptr) override {
        if( length == Size ) {
            return false; // No more space
        }
        start = start ? (start - 1) : (Size - 1);
        ++length;
        return true;
    }
    bool pop_back() override {
        if( !length ) {
            return false;
        }
        --length;
        return true;
    }
    bool pop_front() override  {
        if( !length ) {
            return false;
        }
        start = (start + 1) % Size;
        --length;
        return true;
    }

private:
    size_t start{ 0 };
    size_t length { 0 };
};

#endif