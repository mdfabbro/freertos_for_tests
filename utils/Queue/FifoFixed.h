#ifndef __FIFO_BASIC_H__
#define __FIFO_BASIC_H__

#include "Interfaces.h"
#include <cstdio>

template<class T, size_t Size>
class FifoFixed : public IFifo<T> {
public:
    T* begin() override {
        return size ? list[beginPos] : nullptr;
    }
    virtual bool push(T* element) override { 
        if( size == Size ) {
            return false;
        }
        const size_t endPos = (beginPos + size) % Size;
        list[endPos] = element;
        ++size;
        return true;
    }
    virtual bool pop() override {
        if( !size ) {
            return false;
        }
        --size;
        beginPos = (beginPos + 1) % Size;
        return true;
    }

private:
    T* list[Size];
    size_t size {0};
    size_t beginPos { 0 };
};

#endif