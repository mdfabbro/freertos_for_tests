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

ssize_t BaseChannel::transmit(const void* data, size_t size) {
    return ::write(masterFd, data, size);
}

ssize_t BaseChannel::receive(void* buffer, size_t size) {
    return ::read(masterFd, buffer, size);
}

}
