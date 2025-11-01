#include "BaseChannel.h"

#include <unistd.h>
#include <stdexcept>

namespace Serial {

BaseChannel::BaseChannel(int fd) : masterFd(fd) {
    if (fd < 0)
        throw std::runtime_error("Invalid file descriptor for BaseChannel");
}

BaseChannel::~BaseChannel() {
    if (masterFd != -1)
        ::close(masterFd);
}

size_t BaseChannel::transmit(const void* data, size_t size) {
    const auto res = ::write(masterFd, data, size);
    return (res <= 0) ? 0 : static_cast<size_t>(res);
}

size_t BaseChannel::receive(void* buffer, size_t size) {
    const auto res = ::read(masterFd, buffer, size);
    return (res <= 0) ? 0 : static_cast<size_t>(res);
}

}
