#ifndef __QUEUE_DEQUE_LINKED_LENGTH_H__
#define __QUEUE_DEQUE_LINKED_LENGTH_H__

#include "Interfaces.h"
#include <iostream>

template<class T>
class DequeLinked : public IDeque<T> {
private:
    struct Element {
        T* value;
        Element *next {nullptr};
        Element *prev {nullptr};
        explicit Element(T* value) : value(value) {}
    };

    Element *head {nullptr};
    Element *tail {nullptr};
    size_t length {0};

    Element * getElementAt(size_t index) {
        if( index >= length ) {
            return nullptr;
        }
        Element * toReturn {nullptr};
        if( index <= ((length-1)/2) ) {
            toReturn = head;
            for( size_t i = 0; i < index ; ++i ) {
                toReturn = toReturn->next;
            }
        } else {
            toReturn = tail;
            for( size_t i = (length-1); i > index ; --i ) {
                toReturn = toReturn->prev;
            }
        }
        return toReturn;
    }

public:

    DequeLinked() {}

    explicit DequeLinked(DequeLinked && other) : head (other.head) , tail(other.tail) , length(other.length) {
        other.head = nullptr;
        other.tail = nullptr;
        other.length = 0;
    }

    DequeLinked& operator=(DequeLinked && other) {
        if(this == &other) {
            return *this;
        }
        head  = other.head ;
        tail = other.tail; 
        length = other.length;
        other.head = nullptr;
        other.tail = nullptr;
        other.length = 0;
        return *this;
    }

    explicit DequeLinked(const DequeLinked & ) = delete;
    DequeLinked& operator=(const DequeLinked &) = delete;

    ~DequeLinked() { clear(); }

    T* front() override { return head ? head->value : nullptr ; }

    T* back() override  { return tail ? tail->value : nullptr ; }

    T* at(size_t index) override { 
        auto * toReturn = getElementAt(index);
        return toReturn ? toReturn->value : nullptr; 
    }

    T* operator[](size_t index) override { return at(index); }

    bool push_back(T* value) override { return insert( length, value ); }

    bool push_front(T* value) override  { return insert( 0, value ); }
    
    bool insert(size_t index, T* value) override {
        if( index > length ) { return false; } // Non-valid index
        Element * toInsert = new Element( value );
        if( index == length ) {
            if( tail ) {
                tail->next = toInsert;
                toInsert->prev = tail;
            }
            tail = toInsert;
        } else {
            auto * next = getElementAt(index);
            auto* prev = next->prev;

            if( prev ) {
                prev->next = toInsert;
                toInsert->prev = prev;
                next->prev = toInsert;
            }
            toInsert->next = next;
        }
        if(index == 0) {
            head = toInsert;
        }
        ++length;
        return true;
    }

    bool pop_back() override  { return empty() ? false : erase( length -1 ) ; }

    bool pop_front() override { return empty() ? false : erase( 0 ); } 

    void clear() override { while( !empty() ) { erase(0); } } 

    bool erase(size_t index) override {
        Element * toErase = getElementAt(index);
        if(!toErase) { return false; }
        if( index == 0 ) {
            Element * next = toErase->next;
            if(next) {
                next->prev = nullptr;
            } else { // It is the only element
                tail = nullptr;
            }
            head = next;
        } else if ( toErase == tail ) {
            Element * prev = toErase->prev;
            prev->next = nullptr;
            tail = prev;
        } else {
            Element * prev = toErase->prev;
            Element * next = toErase->next;
            prev->next = next;
            next->prev = prev;
        }
        toErase->next = toErase->prev = nullptr; // not needed
        delete toErase;
        --length;
        return true;
    }

    inline size_t size() override { return length; };

    inline bool empty() override { return !length; };

};

#endif