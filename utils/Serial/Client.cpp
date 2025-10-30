#include "Client.h"

#include <stdexcept>     // std::runtime_error
#include <fcntl.h>       // open, O_RDWR, O_NOCTTY
#include <unistd.h>      // close (inherited from BaseChannel cleanup)

Serial::Client::Client(const std::string& path)
    : BaseChannel(::open(path.c_str(), O_RDWR | O_NOCTTY)) {
}