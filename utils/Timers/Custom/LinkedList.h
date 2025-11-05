#ifndef __TIMER_CUSTOM_LINKED_LIST_H__
#define __TIMER_CUSTOM_LINKED_LIST_H__

#include <iostream>

template<class T>
class LinkedList {
public:
    using LessEqual = bool(*)(T*, T*);

    bool contains(T* element) {
        for ( T* current = list; current != nullptr; current = current->next ) {
            if( current == element ) {
                return true;
            }
        }
        return false;
    }

    T* remove(T* element) { // (!) We return T* with the element removed so it can be deleted if needed.
        T* prev {nullptr};
        for ( T* current = list; current != nullptr; current = current->next ) {
            if( current == element ) {
                if(!prev) { 
                    list = current->next;
                } else {
                    prev->next = current->next;
                }
                current->next = nullptr; // To make sure it is clean;
                return current;
            }
            prev = current;
        }
        return nullptr;
    }

    void insertFront(T* element) {
        element->next = list;
        list = element;
    }

    void insertBack(T* element) {
        if (!list) {
            list = element;
            return;                
        }
        T* current = list;
        while(current->next) { current = current->next; }
        current->next = element;
    }

    void insert(T* element, LessEqual isLower) {
        T* prev {nullptr};
        for ( T* current = list ; current != nullptr; current = current->next ) {
            if(isLower(element,current)) {
                element->next = current;
                break;
            }
            prev = current;
        }
        if( !prev ) {
            list = element;
        } else {
            prev->next = element;
        }
    }

    T* begin() {
        return list;
    }

private:
    T* list {nullptr};

};

#endif // __TIMER_CUSTOM_LINKED_LIST_H__