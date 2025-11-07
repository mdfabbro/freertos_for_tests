#ifndef __QUEUE_DEQUE_LINKED_H__
#define __QUEUE_DEQUE_LINKED_H__

#include "Interfaces.h"

template <typename T>
class DequeLinked : public IDeque<T> { 
public:

    ~DequeLinked() {
        while(start) {
            pop_front();
        }
    }

    DequeLinked() = default;
    
    DequeLinked(const DequeLinked & ) = delete;
    DequeLinked& operator=(const DequeLinked & ) = delete;

    DequeLinked( DequeLinked && toMove) {
        this->start = toMove.start;
        this->end = toMove.end;
        toMove.start = toMove.end = nullptr;
    }

    DequeLinked& operator=( DequeLinked && toMove) {
        if(this != &toMove) {
            this->start = toMove.start;
            this->end = toMove.end;
            toMove.start = toMove.end = nullptr;
        }
        return *this;
    }

    T* front() override { 
        if( !start ) {
            return nullptr;
        }
        return start->value;
    }
    T* back() override {
        if( !end ) {
            return nullptr;
        }
        return end->value;
    }

    bool push_back(T* element) override {
        if( !end ) {
            end = new Element{element};
            if(!end) {
                return false;
            }
            start = end;
        } else {
            auto* toPush = new Element{element};
            if(!toPush) {
                return false;
            }
            end->next = toPush;
            toPush->prev = end;
            end = toPush;
        }
        return true;
    }
    bool push_front(T* element) override {
        if( !start ) {
            start = new Element{element};
            if(!start) {
                return false;
            }
            end = start;
        } else {
            auto* toPush = new Element{element};
            if(!toPush) {
                return false;
            }
            start->prev = toPush;
            toPush->next = start;
            start = toPush;
        }
        return true;
    }
    bool pop_back() override {
        if( !end ) {
            return false;
        }
        auto * toRemove = end;
        if(!end->prev) {
            // It is the only element
            end = start = nullptr;
        } else {
            end->prev->next = nullptr;
            end = end->prev;
        }
        delete toRemove;
        return true;
    }
    bool pop_front() override  {
        if( !start ) {
            return false;
        }
        auto * toRemove = start;
        if(!start->next) {
            // It is the only element
            end = start = nullptr;
        } else {
            start->next->prev = nullptr;
            start = start->next;
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

    Element* start {nullptr};
    Element* end {nullptr};

};

template<>
class DequeLinked<void> : public IDeque<void> { 
public:

    ~DequeLinked() {
        while(start) {
            pop_front();
        }
    }

    DequeLinked() = default;
    
    DequeLinked(const DequeLinked & ) = delete;
    DequeLinked& operator=(const DequeLinked & ) = delete;

    DequeLinked( DequeLinked && toMove) {
        this->start = toMove.start;
        this->end = toMove.end;
        toMove.start = toMove.end = nullptr;
    }

    DequeLinked& operator=( DequeLinked && toMove) {
        this->start = toMove.start;
        this->end = toMove.end;
        toMove.start = toMove.end = nullptr;
        return *this;
    }

    void* front() override { return nullptr; }
    void* back() override { return nullptr; }

    bool push_back(void* = nullptr) override {
        if( !end ) {
            end = new ElementBasic{};
            if(!end) {
                return false;
            }
            start = end;
        } else {
            auto* toPush = new ElementBasic{};
            if(!toPush) {
                return false;
            }
            end->next = toPush;
            toPush->prev = end;
            end = toPush;
        }
        return true;
    }
    bool push_front(void* = nullptr) override {
        if( !start ) {
            start = new ElementBasic{};
            if(!start) {
                return false;
            }
            end = start;
        } else {
            auto* toPush = new ElementBasic{};
            if(!toPush) {
                return false;
            }
            start->prev = toPush;
            toPush->next = start;
            start = toPush;
        }
        return true;
    }
    bool pop_back() override {
        if( !end ) {
            return false;
        }
        auto * toRemove = end;
        if(!end->prev) {
            // It is the only element
            end = start = nullptr;
        } else {
            end->prev->next = nullptr;
            end = end->prev;
        }
        delete toRemove;
        return true;
    }
    bool pop_front() override  {
        if( !start ) {
            return false;
        }
        auto * toRemove = start;
        if(!start->next) {
            // It is the only element
            end = start = nullptr;
        } else {
            start->next->prev = nullptr;
            start = start->next;
        }
        delete toRemove;
        return true;
    }

private:
    struct ElementBasic {
        ElementBasic* next {nullptr};
        ElementBasic* prev {nullptr};
    };
    ElementBasic* start {nullptr};
    ElementBasic* end {nullptr};

};

#endif