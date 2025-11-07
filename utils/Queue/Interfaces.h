#ifndef __QUEUE_INTERFACES_H__
#define __QUEUE_INTERFACES_H__

#include <cstdio>

template<class T>
struct IFifo {
    virtual ~IFifo() noexcept {};

    virtual T* begin() = 0 ;
    virtual bool push(T*) = 0 ;
    virtual bool pop() = 0 ;
};

template<class T>
struct IDeque {
    virtual ~IDeque() noexcept {};

    virtual T* front() = 0 ;
    virtual T* back() = 0 ;
    virtual T* at(size_t index) = 0;
    virtual T* operator[](size_t index) = 0;
    
    virtual bool push_back(T*) = 0 ;
    virtual bool push_front(T*) = 0 ;
    virtual bool pop_back() = 0 ;
    virtual bool pop_front() = 0 ;

    virtual bool erase(size_t index) = 0;
    virtual size_t size() = 0;
    virtual bool empty() = 0;
    
};

template<class T>
struct IQueue {
    virtual ~IQueue() noexcept {};

    virtual T* front() = 0 ;
    virtual T* back() = 0 ;
    virtual T* at(size_t index) = 0;
    virtual T* operator[](size_t index) = 0;

    virtual bool push(T*) = 0 ;
    virtual bool pop() = 0 ;

    virtual bool erase(size_t index) = 0;
    virtual size_t size() = 0;
    virtual bool empty() = 0;
};


#endif