#ifndef __SERIAL_INTERFACES_H__
#define __SERIAL_INTERFACES_H__

#include <unistd.h>
#include <cstddef>

namespace Serial {

// Interface for transmitting data
struct ITx {
    virtual ~ITx() = default;
    virtual ssize_t transmit(const void* data, size_t size) = 0;
};

// Interface for receiving data
struct IRx {
    virtual ~IRx() = default;
    virtual ssize_t receive(void* buffer, size_t size) = 0;
};

struct ITransceiver: public ITx, public IRx {
};

}

#endif // __SERIAL_INTERFACES_H__