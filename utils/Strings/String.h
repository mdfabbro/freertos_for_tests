#ifndef __STRINGS_STRING_H__
#define __STRINGS_STRING_H__

#include "Interfaces.h"

namespace Strings {

template<size_t Size>
class StringFixedSize : public IBasicString {
private:

    char _data[Size + 1] ;
    size_t _length { 0 };
    
public:
    StringFixedSize() {
        _data[Size] = '\0';
        _data[0] = '\0';
    }
    ~StringFixedSize() = default;

    explicit StringFixedSize( const StringFixedSize & other ) {
        _length = other._length;
        memcpy( &_data[0], &other._data[0], _length );
        _data[Size] = '\0';
        _data[_length] = '\0';
    }

    explicit StringFixedSize( const char * s ) : StringFixedSize(s, strlen(s)) { }

    StringFixedSize( const char * s, size_t count ) {
        _data[Size] = '\0';
        insert(0, s, count);
    }

    StringFixedSize & operator=( const StringFixedSize & other ) {
        if( this != &other ) {
            _length = other._length;
            memcpy( &_data[0], &other._data[0], _length );
            _data[Size] = '\0';
            _data[_length] = '\0';
        }
        return *this;
    }

    explicit StringFixedSize( StringFixedSize && other ) {
        _length = other._length;
        memcpy( &_data[0], &other._data[0], _length );
        _data[Size] = '\0';
        _data[_length] = '\0';

        other._data[0] = '\0';
        other._length = '\0';
    }

    StringFixedSize & operator=( StringFixedSize && other ) {
        if( this != &other ) {
            _length = other._length;
            memcpy( &_data[0], &other._data[0], _length );
            _data[Size] = '\0';
            _data[_length] = '\0';

            other._data[0] = '\0';
            other._length = '\0';
        }
        return *this;
    }

    // Element Access

    inline char& at( size_t index) override { return (index < _length) ? _data[index] : _data[Size]; }
    inline const char& at( size_t index) const override { return  (index < _length) ? _data[index] : _data[Size]; }
    inline char& operator[]( size_t index) override { return at(index); }
    inline const char& operator[]( size_t index) const override { return at(index); }
    inline char& front( ) override  { return at(0); }
    inline const char& front( ) const override { return at(0); }
    inline char& back( ) override { return  _length ? _data[_length -1] : _data[Size]; }
    inline const char& back( ) const override { return  _length ? _data[_length -1] : _data[Size]; }

    inline char* data( ) override { return static_cast<char*>(_data) ; }
    inline const char* data( ) const override { return static_cast<const char*>(_data) ; }
    inline const char* c_str( ) const override { return data(); }

    // Capacity

    bool empty() const override { return !_length; } 

    inline size_t size() const   override { return _length; }
    inline size_t length() const override { return _length; }

    inline size_t max_size() const override { return Size; }
    inline size_t capacity() const override { return Size; }

    void clear() override {
        _data[0] = '\0';
        _length = '\0';
    }

    IBasicString& insert(size_t index, size_t count, char ch) override { 
        if( index < _length ) {
            if( (_length + count) <= Size ) {
                memmove( &_data[ index + count ], &_data[ index ], count );
                memset( &_data[ index ], ch, count );
                _length += count;
            } else if( (index + count) < Size ) {
                memmove( &_data[ index + count ], &_data[ index ], _length + count - Size );
                memset( &_data[ index ], ch, count );
                _length = Size;
            } else {
                memset( &_data[ index ], ch, (Size - index) );
                _length = Size;
            }
            _data[ _length ] = '\0'; // It is safe as it is 'Size + 1'.
        }
        return *this;
    }

    IBasicString& insert(size_t index, const char* s) override { 
        const auto count = strlen(s);
        return insert(index, s , count );
    }
    IBasicString& insert(size_t index, const char* s, size_t count) override { 
        if( index <= _length ) {
            if( (_length + count) <= Size ) {
                memmove( &_data[ index + count ], &_data[ index ], _length - index );
                memcpy( &_data[ index ], s, count );
                _length += count;
            } else if( (index + count) < Size ) {
                memmove( &_data[ index + count ], &_data[ index ], _length + count - Size );
                memcpy( &_data[ index ], s, count );
                _length = Size;
            } else {
                memcpy( &_data[ index ], s, (Size - index) );
                _length = Size;
            }
            _data[ _length ] = '\0'; // It is safe as it is 'Size + 1'.
        }
        return *this;
    }

    IBasicString& insert(size_t index, const IBasicString& str) override { 
        return insert(index, str.c_str(), str.length() );
    }

    IBasicString& erase(size_t index, size_t count = npos) override { 
        if( count == npos ) {
            count = 1;
        }
        if( (index + count) >= _length ) {
            _length = index ;
        } else {
            memmove( &_data[ index ], &_data[ index + count ], _length - (index + count) );
        }
        _data[ _length ] = '\0';
        return *this;
    }

    size_t find(const IBasicString& str, size_t pos = 0) const override { 
        return find( str.c_str(), pos, str.length() );
    }
    
    size_t find(const char* s, size_t pos , size_t count) const override {
        if( count > _length ) { return npos; }
        for(size_t i = pos ; i <= (_length - count); ++i) {
            bool equal = true;
            for( size_t j = 0; j < count ; ++j ) {
                if( _data[i+j] != s[j] ) { 
                    equal = false;
                    break;
                }
            }
            if(equal) { return i; }
        }
        return npos;
    }
    size_t find(const char* s, size_t pos = 0) const override { 
        return find( s, pos, strlen(s) );
    }
    size_t find(char ch, size_t pos = 0) const override { 
        if( pos >= _length ) { return npos; }

        // memchr as al alternative
        for( size_t i = pos ; i < _length; ++i ) {
            if( _data[i] == ch ) { 
                return i; 
            }
        }
        return npos;
    }

    size_t rfind(const IBasicString& str, size_t pos = npos) const override { 
        return rfind( str.c_str(), pos, str.length() );
    }
    size_t rfind(const char* s, size_t pos , size_t count) const override { 
        if( count > _length ) { return npos; }
        if( pos > (_length-count) ) { pos = _length-count; }
        for (size_t i = pos+1 ; i-- > 0; ) {
            bool equal = true;
            for (size_t j = 0; j < count; ++j) {
                if (_data[i + j] != s[j]) {
                    equal = false;
                    break;
                }
            }
            if (equal) { return i; }
        }
        return npos;
    }
    size_t rfind(const char* s, size_t pos = npos) const override { 
        return rfind( s, pos, strlen(s) );
     }
    size_t rfind(char ch, size_t pos = npos) const override {  
        return rfind(&ch, pos, 1);
    }

    size_t find_first_of(const IBasicString& str, size_t pos = 0) const override { 
        return find_first_of(str.c_str(), pos, str.length());
    }
    size_t find_first_of(const char* s, size_t pos , size_t count) const override { 
        for( size_t i = pos; i < _length ; ++i ) {
            const char toFind = _data[i];
            for( size_t j = 0; j < count ; ++j ) {
                if( s[j] == toFind ) {
                    return i;
                }
            }
        }
        return npos;
    }
    size_t find_first_of(const char* s, size_t pos = 0) const override { 
        return find_first_of(s, pos, strlen(s));
    }
    size_t find_first_of(char ch, size_t pos = 0) const override { 
        return find(ch, pos);
    }

    size_t find_first_not_of(const IBasicString& str, size_t pos = 0) const override { 
        return find_first_not_of( str.c_str() , pos, str.length() );
    }
    size_t find_first_not_of(const char* s, size_t pos , size_t count) const override { 
        for( size_t i = pos; i < _length ; ++i ) {
            const char toFind = _data[i];
            bool notFound {true};
            for( size_t j = 0; j < count ; ++j ) {
                if( s[j] == toFind ) {
                    notFound = false;
                    break;
                }
            }
            if( notFound ) { return i; }
        }
        return npos;
    }
    size_t find_first_not_of(const char* s, size_t pos = 0) const override {
        return find_first_not_of( s , pos, strlen(s) );
    }
    size_t find_first_not_of(char ch, size_t pos = 0) const override { 
        return find_first_not_of( &ch , pos, 1 );
    }

    size_t find_last_of(const IBasicString& str, size_t pos = npos) const override {
        return find_last_of( str.c_str(), pos, str.length() );
    }

    size_t find_last_of(const char* s, size_t pos , size_t count) const override {
        if( pos >= _length ) { pos = _length-1; }
        for( size_t i = pos+1; i-- > 0 ; ) {
            const char toFind = _data[i];
            for( size_t j = 0; j < count ; ++j ) {
                if( s[j] == toFind ) {
                    return i;
                }
            }
        }
        return npos;
    }

    size_t find_last_of(const char* s, size_t pos = npos) const override { 
        return find_last_of( s, pos, strlen(s) );
    }

    size_t find_last_of(char ch, size_t pos = npos) const override { 
        return find_last_of( &ch, pos, 1 );
    }

    size_t find_last_not_of(const IBasicString& str, size_t pos = npos) const override { 
        return find_last_not_of( str.c_str(), pos, str.length() );
    }
    size_t find_last_not_of(const char* s, size_t pos , size_t count) const override { 
        if( pos >= _length ) { pos = _length-1; }
        for( size_t i = pos+1; i-- > 0 ; ) {
            const char toFind = _data[i];
            bool notFound {true};
            for( size_t j = 0; j < count ; ++j ) {
                if( s[j] == toFind ) {
                    notFound = false;
                    break;
                }
            }
            if(notFound) { return i; }
        }
        return npos;
    }
    size_t find_last_not_of(const char* s, size_t pos = npos) const override { 
        return find_last_not_of( s, pos, strlen(s) );
    }
    size_t find_last_not_of(char ch, size_t pos = npos) const override { 
        return find_last_not_of( &ch, pos, 1 );
    }
};

}


#endif 
