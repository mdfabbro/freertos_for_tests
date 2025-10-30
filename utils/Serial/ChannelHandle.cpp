#include "ChannelHandle.h"
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <pty.h>

namespace Serial {

ChannelHandle::ChannelHandle(const std::string& linkPath) 
    : link(linkPath)
{
    fd = ::posix_openpt(O_RDWR | O_NOCTTY);
    if (fd < 0)
        throw std::runtime_error("Failed to open PTY master");

    if (::grantpt(fd) < 0 || ::unlockpt(fd) < 0)
        throw std::runtime_error("Failed to initialize PTY");

    char* slaveName = ::ptsname(fd);
    if (!slaveName)
        throw std::runtime_error("Failed to get PTY slave name");

    path = slaveName;

    if (!link.empty()) {
        ::unlink(link.c_str());  // remove if exists
        if (::symlink(path.c_str(), link.c_str()) != 0)
            throw std::runtime_error("Failed to create symlink " + link);
    }
}

ChannelHandle::~ChannelHandle() {
    if (fd >= 0) {
        ::close(fd);
    }
    if (!link.empty()) {
        ::unlink(link.c_str());
    }
}

ChannelHandle::ChannelHandle(ChannelHandle&& other) noexcept
    : link(std::move(other.link)), path(std::move(other.path)), fd(other.fd) {
    other.fd = -1; // evitar que el destructor cierre el fd movido
}

ChannelHandle& ChannelHandle::operator=(ChannelHandle&& other) noexcept {
    if (this != &other) {
        if (fd >= 0) ::close(fd);
        fd = other.fd;
        link = std::move(other.link);
        path = std::move(other.path);
        other.fd = -1;
    }
    return *this;
}

int ChannelHandle::getFd() const { return fd; }
std::string ChannelHandle::getLink() const { return link; }
std::string ChannelHandle::getPath() const { return path; }
bool ChannelHandle::valid() const { return fd >= 0; }

} // namespace Serial
