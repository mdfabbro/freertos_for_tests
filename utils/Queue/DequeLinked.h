#ifndef __QUEUE_DEQUE_LINKED_H__
#define __QUEUE_DEQUE_LINKED_H__

#include "Interfaces.h"

template <typename T>
class DequeLinked : public IDeque<T> { 
public:

    ~DequeLinked() {
        clear();
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

    T* at(size_t index) override {
        Element * element = elementAt(index);
        return element ? element->value : nullptr;
    }

    T* operator[](size_t index) override {
        return at(index);
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

    void clear() override {
        while(start) {
            pop_front();
        }
    }

    bool erase(size_t index) override {
        Element* element = elementAt(index);
        if(element == nullptr) {
            return false;
        }
        if(element->next) {
            element->next->prev = element->prev;
        } else {
            // We are deleting end...
            end = element->prev;
        }
        if(element->prev) {
            element->prev->next = element->next;
        } else {
            // We are deleting the start...
            start = element->next;
        }
        delete element;
        return true;
    }
    size_t size() override {
        // Just to save using the length but we pay with processing time...
        // TODO: Change to have length...
        size_t res {0};
        for(Element* current = start; current != nullptr ; current = current->next) {
            ++res;
        }
        return res;
    }
    bool empty() override {
        return start == nullptr;
    };
    

private:
    struct Element {
        
        Element* next {nullptr};
        Element* prev {nullptr};
        T* value {nullptr};

        Element(T* value) : value(value) {}
    };

    Element* start {nullptr};
    Element* end {nullptr};

    Element* elementAt(size_t index) {
        size_t i {0};
        for( Element *current = start; current != nullptr ; current = current->next ) {
            if(i == index) {
                return current;
            }
            ++i;
        }
        return nullptr;
    }

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
    void* at(size_t ) override { return nullptr; }
    void* operator[](size_t ) override { return nullptr; }

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

    void clear() override {
        while(start) {
            pop_front();
        }
    }

    bool erase(size_t index) override {
        ElementBasic* element = elementAt(index);
        if(element == nullptr) {
            return false;
        }
        if(element->next) {
            element->next->prev = element->prev;
        } else {
            // We are deleting end...
            end = element->prev;
        }
        if(element->prev) {
            element->prev->next = element->next;
        } else {
            // We are deleting the start...
            start = element->next;
        }
        delete element;
        return true;
    }
    size_t size() override {
        // Just to save using the length but we pay with processing time...
        // TODO: Change to have length...
        size_t res {0};
        for(ElementBasic* current = start; current != nullptr ; current = current->next) {
            ++res;
        }
        return res;
    }
    bool empty() override {
        return start == nullptr;
    };

private:
    struct ElementBasic {
        ElementBasic* next {nullptr};
        ElementBasic* prev {nullptr};
    };
    ElementBasic* start {nullptr};
    ElementBasic* end {nullptr};

    ElementBasic* elementAt(size_t index) {
        size_t i {0};
        for( ElementBasic *current = start; current != nullptr ; current = current->next ) {
            if(i == index) {
                return current;
            }
            ++i;
        }
        return nullptr;
    }

};

#endif