#ifndef __SERIAL_SERVER_H__
#define __SERIAL_SERVER_H__

#include "BaseChannel.h"
#include "ChannelHandle.h"
#include <string>

namespace Serial {

class Server : public BaseChannel {
private:
    ChannelHandle handler;
public:
    explicit Server(ChannelHandle && handler_): BaseChannel(handler_.getFd()), handler(std::move(handler_)) {
    }
};

}

#endif