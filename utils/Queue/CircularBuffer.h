#ifndef __QUEUE_CIRCULAR_BUFFER_H__
#define __QUEUE_CIRCULAR_BUFFER_H__

#include "DequeFixed.h"

template<typename T, size_t Size>
class CircularBuffer : public IQueue<T>, private DequeFixed<T,Size> {
private:
    using Deque = DequeFixed<T,Size>;

public:
    CircularBuffer() : Deque() {}
    
    ~CircularBuffer() {};

    CircularBuffer(const CircularBuffer& )  = delete;
    CircularBuffer & operator=(const CircularBuffer& ) = delete;

    CircularBuffer(CircularBuffer && cb) 
        : Deque(static_cast<Deque&&>(cb)) {
    }
    CircularBuffer & operator=(CircularBuffer&& cb) {
        if(this != &cb) {
            Deque::operator=(static_cast<Deque&&>(cb));
        }
        return *this;
    }

    T* front() override {
        return Deque::front();
    }
    T* back() override {
        return Deque::back();
    }
    T* at(size_t index) override {
        return Deque::at(index);
    }
    T* operator[](size_t index) override {
        return Deque::at(index);
    }

    bool push(T* t) override {
        if(Deque::size() == Size) {
            if(!pop()) { // the oldest one
                return false;
            }
        }
        return Deque::push_back(t);
    }

    bool pop() override {
        return Deque::pop_front();
    }

    void clear() override {
        Deque::clear();
    }

    bool erase(size_t index) override {
        return Deque::erase(index);
    }
    size_t size() override {
        return Deque::size();
    }
    bool empty() override {
        return Deque::empty();
    }
};

#endif