#ifndef __FIFO_DEQUE_H__
#define __FIFO_DEQUE_H__

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
        this->front = toMove.front;
        this->length = toMove.length;
        for(size_t i = toMove.front; i != toMove.length ; ++i) {
            this->list[i%Size] = toMove.list[i%Size];
            toMove.list[i%Size] = nullptr; // not really needed
        }
        toMove.front = toMove.length = 0;
    }

    DequeFixed& operator=( DequeFixed && toMove) {
        this->front = toMove.front;
        this->length = toMove.length;
        for(size_t i = toMove.front; i != toMove.length ; ++i) {
            this->list[i%Size] = toMove.list[i%Size];
            toMove.list[i%Size] = nullptr; // not really needed
        }
        toMove.front = toMove.length = 0;
        return *this;
    }

    T* begin() override { 
        if( !length ) {
            return nullptr;
        }
        return list[front];
    }
    T* end() override {
        if( !length ) {
            return nullptr;
        }
        return list[(front + length - 1)%Size];
    }

    bool push_back(T* element) override {
        if( length == Size ) {
            return false; // No more space
        }
        ++length;
        list[(front + length - 1)%Size] = element;
        return true; 
    }
    bool push_front(T* element) override {
        if( length == Size ) {
            return false; // No more space
        }
        front = front ? (front - 1) : (Size - 1);
        list[front] = element;
        ++length;
        return true;
    }
    bool pop_back() override {
        if( !length ) {
            return false;
        }
        list[(front + length)%Size] = nullptr; // Optional
        --length;
        return true;
    }
    bool pop_front() override  {
        if( !length ) {
            return false;
        }
        list[front] = nullptr; // Optional
        front = (front + 1) % Size;
        --length;
        return true;
    }

private:
    T* list[Size] {nullptr};
    size_t front{ 0 };
    size_t length { 0 };
};

#endif