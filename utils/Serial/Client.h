#ifndef __SERIAL_CLIENT_H__
#define __SERIAL_CLIENT_H__

#include "BaseChannel.h"
#include <string>

namespace Serial {

struct Client : public BaseChannel {
    explicit Client(const std::string& path) ;
};

}
#endif