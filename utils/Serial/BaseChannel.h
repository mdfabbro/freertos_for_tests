#ifndef __SERIAL_BASE_CHANNEL_H__
#define __SERIAL_BASE_CHANNEL_H__

#include "Interfaces.h"

namespace Serial {

class BaseChannel : public ITransceiver {
protected:
    int masterFd;

    // Protected constructor: fd se pasa desde Server/Client
    explicit BaseChannel(int fd);

    ~BaseChannel() override ;

public:
    size_t transmit(const void* data, size_t size) override;
    size_t receive(void* buffer, size_t size) override;
};

} 

#endif // __SERIAL_BASE_CHANNEL_H__
