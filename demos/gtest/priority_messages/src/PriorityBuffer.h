#ifndef __PRIORITY_BUFFER_H__
#define __PRIORITY_BUFFER_H__

#include "Queue/CircularBuffer.h"

template<class T>
struct IPriorityBuffer {
    virtual ~IPriorityBuffer() noexcept {};

    virtual T* front() = 0 ;
    virtual T* back() = 0 ;
    virtual bool pop() = 0 ;
    virtual bool insert(T*, bool reorder) = 0 ;

    virtual T* at(size_t index) = 0;
    virtual T* operator[](size_t index) = 0;

    virtual void reorder() = 0 ;

    virtual void clear() = 0;

    virtual bool erase(size_t index) = 0;
    virtual size_t size() = 0;
    virtual bool empty() = 0;
};

template<class T, size_t Size>
class PriorityCircularBuffer : public IPriorityBuffer<T>, private CircularBuffer<T,Size> {
public:
    using Buffer = CircularBuffer<T,Size>;
private:


public:
    PriorityCircularBuffer() : Buffer() {}

    ~PriorityCircularBuffer() noexcept {}

    T* front() override { return Buffer::front(); }
    T* back() override { return Buffer::back(); }
    bool pop() override { return Buffer::pop(); }

    bool insert(T* element, bool reorderAfterInsert = true) override { 
        bool res = Buffer::insert(0, element);
        if(reorderAfterInsert) { reorder(); }
        return res;
    }

    void reorder() override {
        bool changed;
        do {
            changed = false;
            for( size_t i = 1; i < size() ; ++i ) {
                if( *Buffer::at(i-1) < *Buffer::at(i) ) { // We want the higher first!
                    // We should erase and insert, but to avoid the memory
                    // allocation, T must support operator=
                    T tmp = *Buffer::at(i);
                    *Buffer::at(i) = *Buffer::at(i-1) ;
                    *Buffer::at(i-1) = tmp;
                }
            }
        } while (changed);
    }

    inline T* at(size_t index) override { return Buffer::at(index);};
    inline T* operator[](size_t index) override { return Buffer::at(index);};

    inline void clear() override { return Buffer::clear(); }

    inline bool erase(size_t index) override { 
        return Buffer::erase(index); 
    }
    inline size_t size() override { return Buffer::size(); }
    inline bool empty() override { return Buffer::empty(); }

};

#endif // __PRIORITY_BUFFER_H__