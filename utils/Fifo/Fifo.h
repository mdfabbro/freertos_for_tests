#ifndef __FIFO_H__
#define __FIFO_H__

template<class T>
struct IFifo {
    virtual ~IFifo() noexcept {};

    virtual T* begin() = 0 ;
    virtual void push(T*) = 0 ;
    virtual bool  pop() = 0 ;
};

template<class T>
class FifoBasic : public IFifo<T> {
public:
    T* begin() override {
        return start ? start->value : nullptr;
    }
    virtual void push(T* element) override { 
        if( !start ) {
            start = new Element(element);
        } else {
            Element* prev = start;
            while(prev->next) { prev = prev->next; }
            prev->next = new Element(element);
        }
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