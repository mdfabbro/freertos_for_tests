#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <netinet/in.h>

class Socket {
public:
    Socket();
    explicit Socket(int fd); // wrap existing socket
    ~Socket();

    bool create();                     // TCP socket
    bool bind(unsigned short port);    // bind to port
    bool listen(int backlog = 5);      // listen
    Socket* accept(struct sockaddr_in* addr = nullptr); // return new Socket
    void close();
    int fd() const { return sockFd; }

private:
    int sockFd{-1};
};

#endif // __SOCKET_H__