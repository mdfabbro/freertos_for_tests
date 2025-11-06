#ifndef __FIFO_INTERFACES_H__
#define __FIFO_INTERFACES_H__

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

    virtual T* begin() = 0 ;
    virtual T* end() = 0 ;
    virtual bool push_back(T*) = 0 ;
    virtual bool push_front(T*) = 0 ;
    virtual bool pop_back() = 0 ;
    virtual bool pop_front() = 0 ;
};


#endif