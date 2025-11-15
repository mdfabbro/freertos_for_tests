#ifndef __STRINGS_INTERFACES_H__
#define __STRINGS_INTERFACES_H__

#include <cstddef>

namespace Strings {

constexpr size_t npos = static_cast<size_t>(-1) ;

struct IBasicString {
    virtual ~IBasicString() {} ;

    virtual char& at( size_t index) = 0 ;
    virtual const char& at( size_t index) const = 0 ;
    virtual char& operator[]( size_t index) = 0 ;
    virtual const char& operator[]( size_t index) const = 0 ;
    virtual char& front( ) = 0 ;
    virtual const char& front( ) const = 0 ;
    virtual char& back( ) = 0 ;
    virtual const char& back( ) const = 0 ;

    virtual char* data( ) = 0 ;
    virtual const char* data( ) const = 0 ;
    virtual const char* c_str( ) const = 0 ;

    // Capacity

    virtual bool empty() const = 0 ;

    virtual size_t size() const = 0 ;
    virtual size_t length() const = 0 ;

    virtual size_t max_size() const = 0 ;
    // virtual void reserve(size_t ) const = 0 ;
    virtual size_t capacity() const = 0 ;

    // Mofifiers

    virtual void clear() = 0 ;

    virtual IBasicString& insert(size_t index, size_t count, char ch) = 0 ;
    virtual IBasicString& insert(size_t index, const char* s) = 0 ;
    virtual IBasicString& insert(size_t index, const char* s, size_t count) = 0 ;
    virtual IBasicString& insert(size_t index, const IBasicString& str) = 0 ;

    virtual IBasicString& erase(size_t index, size_t count = npos)      = 0 ;

    virtual size_t find(const IBasicString& str, size_t pos = 0) const = 0 ;
    virtual size_t find(const char* s, size_t pos , size_t count) const = 0 ;
    virtual size_t find(const char* s, size_t pos = 0) const = 0 ;
    virtual size_t find(char ch, size_t pos = 0) const = 0 ;

    virtual size_t rfind(const IBasicString& str, size_t pos = npos) const = 0 ;
    virtual size_t rfind(const char* s, size_t pos , size_t count) const = 0 ;
    virtual size_t rfind(const char* s, size_t pos = npos) const = 0 ;
    virtual size_t rfind(char ch, size_t pos = npos) const = 0 ;

    virtual size_t find_first_of(const IBasicString& str, size_t pos = 0) const = 0 ;
    virtual size_t find_first_of(const char* s, size_t pos , size_t count) const = 0 ;
    virtual size_t find_first_of(const char* s, size_t pos = 0) const = 0 ;
    virtual size_t find_first_of(char ch, size_t pos = 0) const = 0 ;

    virtual size_t find_first_not_of(const IBasicString& str, size_t pos = 0) const = 0 ;
    virtual size_t find_first_not_of(const char* s, size_t pos , size_t count) const = 0 ;
    virtual size_t find_first_not_of(const char* s, size_t pos = 0) const = 0 ;
    virtual size_t find_first_not_of(char ch, size_t pos = 0) const = 0 ;

    virtual size_t find_last_of(const IBasicString& str, size_t pos = npos) const = 0 ;
    virtual size_t find_last_of(const char* s, size_t pos , size_t count) const = 0 ;
    virtual size_t find_last_of(const char* s, size_t pos = npos) const = 0 ;
    virtual size_t find_last_of(char ch, size_t pos = npos) const = 0 ;

    virtual size_t find_last_not_of(const IBasicString& str, size_t pos = npos) const = 0 ;
    virtual size_t find_last_not_of(const char* s, size_t pos , size_t count) const = 0 ;
    virtual size_t find_last_not_of(const char* s, size_t pos = npos) const = 0 ;
    virtual size_t find_last_not_of(char ch, size_t pos = npos) const = 0 ;

};

}

#endif
