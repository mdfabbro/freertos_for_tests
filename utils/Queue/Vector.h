#ifndef __QUEUE_VECTOR_H__
#define __QUEUE_VECTOR_H__

#include "Interfaces.h"
#include <cstdio>

template <typename T, size_t Size>
class Vector : public IVector<T> { 
public:
    ~Vector() {
        while(length) { erase(0); }
    }
    Vector() {};
    explicit Vector(const Vector & ) = delete;
    Vector& operator=(const Vector & ) = delete;

    Vector( Vector && toMove) {
        this->length = toMove.length;
        for(size_t i = 0; i < Size ; ++i) {
            this->list[i] = toMove.list[i];
        }
        toMove.length = 0;
    }

    Vector& operator=( Vector && toMove) {
        if( this != &toMove ) {
            this->length = toMove.length;
            for(size_t i = 0; i < Size ; ++i) {
                this->list[i] = toMove.list[i];
            }
            toMove.length = 0;
        }
        return *this;
    }

    T* front() override { return length ? &list[0] : nullptr; }
    T* back() override { return length ? &list[length - 1] : nullptr; }
    T* at(size_t index) override { return (index < length) ? &list[index] : nullptr; }
    T* operator[] (size_t index) override { return (index < length) ? &list[index] : nullptr; }
    T* data() override { return static_cast<T*>(list); }

    bool push_back(const T& element) override  { return insert(length, element); }
    bool pop_back() override { return length ? erase(length - 1) : false; }

    bool insert(size_t index, const T& toInsert) override {
        if( index > size() )  { return false; }  // Index out of range
        if( length >= Size )  { return false; }  // No more space!
        for( size_t i = length ; i > index ; --i ) {
            list[ i ] = list[ i - 1 ];
        }
        list[ index ] = toInsert;
        ++length;
        return true;
    }

    void clear() override {
        length = 0;
    }

    bool erase(size_t index) override {
        if(index >= length) { return false; }
        // This can be two memcpy:
        for(size_t i = index; i < (length - 1) ; ++i) {
            list[i] = list[i + 1] ;
        }
        --length;
        return true;
    }
    
    inline size_t size() override { return length; }
    inline bool empty() override { return !length; }

private:
    T list[Size] ;
    size_t length { 0 };
};

#endif