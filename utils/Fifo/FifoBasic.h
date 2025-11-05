#ifndef __FIFO_BASIC_H__
#define __FIFO_BASIC_H__

#include "Interfaces.h"

template<class T>
class FifoBasic : public IFifo<T> {
public:
    T* begin() override {
        return start ? start->value : nullptr;
    }
    virtual bool push(T* element) override { 
        if( !start ) {
            start = new Element(element);
        } else {
            Element* prev = start;
            while(prev->next) { prev = prev->next; }
            prev->next = new Element(element);
        }
        return true;
    }
    virtual bool pop() override {
        if( !start ) {
            return false;
        }
        Element* toDelete = start;
        start = start->next;
        delete toDelete;
        return true;
    }

private:
    struct Element {
        Element* next {nullptr};
        T* value;
        explicit Element(T* value) : value(value) {}
    };
    Element* start {nullptr};
};

#endif