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

    inline T* front() override { 
        return Deque::front();
    }
    inline T* back() override { 
        return Deque::back();
    }
    inline T* at(size_t index) override {
        return Deque::at(index);
    } 
    inline T* operator[](size_t index) override {
        return Deque::at(index);
    } 

    inline bool push(T* element) override {
        return Deque::push_back(element);
    }

    inline bool pop() override {
        return Deque::pop_front();
    }

    inline void clear() override {
        Deque::clear();
    }

    inline bool erase(size_t index) override {
        return Deque::erase(index);
    } 
    inline size_t size() override {
        return Deque::size();
    } 
    inline bool empty() override {
        return Deque::empty();
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

    inline void* at(size_t index) override {
        return Deque::at(index);
    } 
    inline void* operator[](size_t index) override {
        return Deque::at(index);
    } 

    bool push(void* element = nullptr) override {
        return Deque::push_back(element);
    }

    bool pop() override {
        return Deque::pop_front();
    }

    inline void clear() override {
        Deque::clear();
    }

    inline bool erase(size_t index) override {
        return Deque::erase(index);
    } 
    inline size_t size() override {
        return Deque::size();
    } 
    inline bool empty() override {
        return Deque::empty();
    } 

};
#endif