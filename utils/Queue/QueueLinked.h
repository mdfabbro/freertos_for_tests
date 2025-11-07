#ifndef __QUEUE_QUEUE_LINKED_H__
#define __QUEUE_QUEUE_LINKED_H__

#include "Interfaces.h"
#include "DequeLinked.h"

template <typename T>
class QueueLinked : public IQueue<T>, private DequeLinked<T> { 
public:

    using Deque = DequeLinked<T>;

    ~QueueLinked() = default;
    QueueLinked() = default;
    
    QueueLinked(const QueueLinked & ) = delete;
    QueueLinked& operator=(const QueueLinked & ) = delete;

    QueueLinked( QueueLinked && toMove) : Deque::DequeLinked(static_cast<Deque&&>(toMove)) {
    }

    QueueLinked& operator=( QueueLinked && toMove) {
        if(this != &toMove) {
            Deque::operator=( static_cast<Deque&&>(toMove) );
        }
        return *this;
    }

    T* front() override { 
        return Deque::front();
    }
    T* back() override { 
        return Deque::back();
    }

    bool push(T* element) override {
        return Deque::push_back(element);
    }

    bool pop() override {
        return Deque::pop_front();
    }

};

template <>
class QueueLinked<void> : public IQueue<void>, private DequeLinked<void> { 
public:

    using Deque = DequeLinked<void>;

    ~QueueLinked() = default;
    QueueLinked() = default;
    
    QueueLinked(const QueueLinked & ) = delete;
    QueueLinked& operator=(const QueueLinked & ) = delete;

    QueueLinked( QueueLinked && toMove) : Deque::DequeLinked(static_cast<Deque&&>(toMove)) {
    }

    QueueLinked& operator=( QueueLinked && toMove) {
        if(this != &toMove) {
            Deque::operator=( static_cast<Deque&&>(toMove) );
        }
        return *this;
    }

    void* front() override { 
        return Deque::front();
    }

    void* back() override { 
        return Deque::back();
    }

    bool push(void* element = nullptr) override {
        return Deque::push_back(element);
    }

    bool pop() override {
        return Deque::pop_front();
    }

};
#endif