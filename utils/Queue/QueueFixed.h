#ifndef __QUEUE_QUEQUE_FIXED_H__
#define __QUEUE_QUEQUE_FIXED_H__

#include "DequeFixed.h"

template <typename T, size_t Size>
class QueueFixed : public IQueue<T>, private DequeFixed<T,Size> { 
public:

    ~QueueFixed() = default;
    QueueFixed() = default;
    
    QueueFixed(const QueueFixed & ) = delete;
    QueueFixed& operator=(const QueueFixed & ) = delete;

    QueueFixed( QueueFixed && toMove) : DequeFixed<T, Size>::DequeFixed(static_cast<DequeFixed<T, Size>&&>(toMove)) {
    }

    QueueFixed& operator=( QueueFixed && toMove) {
        if (this != &toMove) {
            DequeFixed<T, Size>::operator=(static_cast<DequeFixed<T, Size>&&>(toMove));
        }
        return *this;
    }

    inline T* front() override { 
        return DequeFixed<T, Size>::front();
    }

    inline T* back() override { 
        return DequeFixed<T, Size>::back();
    }

    inline T* at(size_t index) override { 
        return DequeFixed<T, Size>::at(index);
    }
    inline T* operator[](size_t index) override { 
        return DequeFixed<T, Size>::operator[](index);
    }

    inline bool push(T* val) override { 
        return DequeFixed<T, Size>::push_back(val);
    }

    inline bool pop() override { 
        return DequeFixed<T, Size>::pop_front();
    }

    inline void clear() override {
        DequeFixed<T, Size>::clear();
    }

    inline bool erase(size_t index) override { 
        return DequeFixed<T, Size>::erase(index);
    }
    inline size_t size() override { 
        return DequeFixed<T, Size>::size();
    }
    inline bool empty() override { 
        return DequeFixed<T, Size>::empty();
    }
};

template <size_t Size>
class QueueFixed<void, Size> : public IQueue<void>, public DequeFixed<void,Size> { 
public:

    ~QueueFixed() = default;
    QueueFixed() = default;
    
    QueueFixed(const QueueFixed & ) = delete;
    QueueFixed& operator=(const QueueFixed & ) = delete;

    QueueFixed( QueueFixed && toMove) : DequeFixed<void, Size>::DequeFixed(static_cast<DequeFixed<void, Size>&&>(toMove)) {
    }

    QueueFixed& operator=( QueueFixed && toMove) {
        if (this != &toMove) {
            DequeFixed<void, Size>::operator=(static_cast<DequeFixed<void, Size>&&>(toMove));
        }
        return *this;
    }

    inline void* front() override { 
        return DequeFixed<void, Size>::front();
    }

    inline void* back() override { 
        return DequeFixed<void, Size>::back();
    }

    inline void* at(size_t index){ 
        return DequeFixed<void, Size>::at(index);
    }

    inline void* operator[](size_t index) override {
        return DequeFixed<void, Size>::at(index);
    }

    inline bool push(void * val = nullptr) override { 
        return DequeFixed<void, Size>::push_back(val);
    }

    inline bool pop() override { 
        return DequeFixed<void, Size>::pop_front();
    }

    inline void clear() override {
        DequeFixed<void, Size>::clear();
    }

    inline bool erase(size_t index) override {
        return DequeFixed<void, Size>::erase(index);
    }
    inline size_t size() override {
        return DequeFixed<void, Size>::size();
    }
    inline bool empty() override {
        return DequeFixed<void, Size>::empty();
    }
};

#endif