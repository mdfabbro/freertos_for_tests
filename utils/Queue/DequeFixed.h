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
        for(size_t i = 0; i != toMove.length ; ++i) {
            this->accessList(i) = toMove.accessList(i);
            toMove.accessList(i) = nullptr;
        }
        toMove.start = toMove.length = 0;
    }

    DequeFixed& operator=( DequeFixed && toMove) {
        if( this != &toMove ) {
            this->start = toMove.start;
            this->length = toMove.length;
            for(size_t i = 0; i != toMove.length ; ++i) {
                this->accessList(i) = toMove.accessList(i);
                toMove.accessList(i) = nullptr; 
            }
            toMove.start = toMove.length = 0;
        }
        return *this;
    }

    T* front() override { 
        if( !length ) {
            return nullptr;
        }
        return accessList(0);
    }
    T* back() override {
        if( !length ) {
            return nullptr;
        }
        return accessList(length - 1);
    }

    T* at(size_t index) override {
        if(index >= length) {
            return nullptr;
        }
        return accessList(index);
    }

    T* operator[] (size_t index) override {
        return at(index);
    }

    bool push_back(T* element) override {
        if( length == Size ) {
            return false; // No more space
        }
        ++length;
        accessList(length - 1) = element;
        return true; 
    }

    bool push_front(T* element) override {
        if( length == Size ) {
            return false; // No more space
        }
        start = start ? (start - 1) : (Size - 1);
        accessList(0) = element;
        ++length;
        return true;
    }

    bool pop_back() override {
        if( !length ) {
            return false;
        }
        --length;
        accessList(length) = nullptr;
        return true;
    }

    bool pop_front() override {
        if( !length ) {
            return false;
        }
        accessList(0) = nullptr; // Optional
        start = (start + 1) % Size;
        --length;
        return true;
    }
    
    bool insert(size_t index, T* toInsert) override {
        if( length == Size )  { return false; }  // No more space!
        if( index > size() )  { return false; }  // Index out of range
        if( index == 0 ) { return push_front(toInsert); }

        for( size_t i = index ; i < (length - 1) ; ++i ) {
            accessList( i + 1 ) = accessList( i );
        }
        accessList( index ) = toInsert;
        ++length;
        return true;
    }

    void clear() override {
        for( size_t i = 0; i < length; ++i ) {
            accessList( i ) = nullptr;
        }
        start = 0;
        length = 0;
    }

    bool erase(size_t index) override {
        if(index >= length) { return false; }
        if(index == 0) { return pop_front(); }
        
        // This can be two memcpy:
        const size_t toMove = length - 1 - index;
        for(size_t moved = 0; moved < toMove ; ++moved) {
            accessList(index + moved) = accessList(index + moved + 1) ;
        }
        accessList(index + toMove) = nullptr;
        --length;
        return true;
    }
    
    inline size_t size() override { return length; }
    inline bool empty() override { return !length; }

private:
    inline T*& accessList(size_t i) {
        return list[(start + i)%Size];
    }

    T* list[Size] {nullptr};
    size_t start{ 0 };
    size_t length { 0 };
};


// In FreeRTOS a queue without elements is used as a mutex.
// So we will support DequeFixed<void, Size>.
template <size_t Size>
class DequeFixed<void, Size> : public IDeque<void> { 
public:

    ~DequeFixed() = default;
    DequeFixed() = default;
    DequeFixed(const DequeFixed & ) = delete;
    DequeFixed& operator=(const DequeFixed & ) = delete;
    DequeFixed( DequeFixed && toMove) = default;
    DequeFixed& operator=( DequeFixed && toMove) = default;

    void* front() override { return nullptr; }
    void* back() override { return nullptr; }
    void* at(size_t ) override { return nullptr; }
    void* operator[](size_t ) override { return nullptr; }

    bool push_back(void* = nullptr) override {
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
        --length;
        return true;
    }

    bool insert(size_t, void*  = nullptr ) override {
        return push_front();
    }

    inline void clear() override {
        length = 0;
    }

    bool erase(size_t index) override {
        if(index >= length) {
            return false;
        }
        --length;
        return true;
    }

    inline size_t size() override { return length; }

    inline bool empty() override { return !length; }

private:
    size_t length { 0 };
};

#endif