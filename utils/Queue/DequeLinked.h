#ifndef __FIFO_DEQUE_H__
#define __FIFO_DEQUE_H__

#include "Interfaces.h"

template <typename T>
class DequeLinked : public IDeque<T> { 
public:

    ~DequeLinked() {
        while(front) {
            pop_front();
        }
    }

    DequeLinked() = default;
    
    DequeLinked(const DequeLinked & ) = delete;
    DequeLinked& operator=(const DequeLinked & ) = delete;

    DequeLinked( DequeLinked && toMove) {
        this->front = toMove.front;
        this->back = toMove.back;
        toMove.front = toMove.back = nullptr;
    }

    DequeLinked& operator=( DequeLinked && toMove) {
        this->front = toMove.front;
        this->back = toMove.back;
        toMove.front = toMove.back = nullptr;
        return *this;
    }

    T* begin() override { 
        if( !front ) {
            return nullptr;
        }
        return front->value;
    }
    T* end() override {
        if( !back ) {
            return nullptr;
        }
        return back->value;
    }

    bool push_back(T* element) override {
        if( !back ) {
            back = new Element{element};
            if(!back) {
                return false;
            }
            front = back;
        } else {
            auto* toPush = new Element{element};
            if(!toPush) {
                return false;
            }
            back->next = toPush;
            toPush->prev = back;
            back = toPush;
        }
        return true;
    }
    bool push_front(T* element) override {
        if( !front ) {
            front = new Element{element};
            if(!front) {
                return false;
            }
            back = front;
        } else {
            auto* toPush = new Element{element};
            if(!toPush) {
                return false;
            }
            front->prev = toPush;
            toPush->next = front;
            front = toPush;
        }
        return true;
    }
    bool pop_back() override {
        if( !back ) {
            return false;
        }
        auto * toRemove = back;
        if(!back->prev) {
            // It is the only element
            back = front = nullptr;
        } else {
            back->prev->next = nullptr;
            back = back->prev;
        }
        delete toRemove;
        return true;
    }
    bool pop_front() override  {
        if( !front ) {
            return false;
        }
        auto * toRemove = front;
        if(!front->next) {
            // It is the only element
            back = front = nullptr;
        } else {
            front->next->prev = nullptr;
            front = front->next;
        }
        delete toRemove;
        return true;
    }

private:
    struct Element {
        
        Element* next {nullptr};
        Element* prev {nullptr};
        T* value {nullptr};

        Element(T* value) : value(value) {}
    };

    Element* front {nullptr};
    Element* back {nullptr};

};

#endif