#ifndef __FIFO_INTERFACES_H__
#define __FIFO_INTERFACES_H__

template<class T>
struct IFifo {
    virtual ~IFifo() noexcept {};

    virtual T* begin() = 0 ;
    virtual bool push(T*) = 0 ;
    virtual bool pop() = 0 ;
};

#endif