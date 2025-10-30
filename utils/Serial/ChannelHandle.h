#ifndef __SERIAL_CHANNEL_HANDLE_H__
#define __SERIAL_CHANNEL_HANDLE_H__

#include <string>

namespace Serial {

/**
 * Represents a shared descriptor for a serial channel (PTY master/slave).
 * Owns the file descriptor and optionally creates a symlink to the slave.
 */
class ChannelHandle {
    std::string link;
    std::string path;
    int fd = -1;

public:
    explicit ChannelHandle(const std::string& linkPath = "");

    ~ChannelHandle();
    ChannelHandle(ChannelHandle&& other) noexcept ;
    ChannelHandle& operator=(ChannelHandle&& other) noexcept ;
    ChannelHandle(const ChannelHandle&) = delete;
    ChannelHandle& operator=(const ChannelHandle&) = delete;

    int getFd() const;
    std::string getLink() const;
    std::string getPath() const;  // symlink if exists, otherwise real slave
    bool valid() const;
};

}

#endif // __SERIAL_CHANNEL_HANDLE_H__
