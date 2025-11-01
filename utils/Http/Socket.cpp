#include "Socket.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>

Socket::Socket() : sockFd(-1) {}
Socket::Socket(int fd) : sockFd(fd) {}

Socket::~Socket() {
    close();
}

bool Socket::create() {
    sockFd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0) {
        perror("socket");
        return false;
    }
    int opt = 1;
    setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    return true;
}

bool Socket::bind(unsigned short port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    if (::bind(sockFd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return false;
    }
    return true;
}

bool Socket::listen(int backlog) {
    if (::listen(sockFd, backlog) < 0) {
        perror("listen");
        return false;
    }
    return true;
}

Socket* Socket::accept(struct sockaddr_in* addr) {
    socklen_t len = addr ? sizeof(sockaddr_in) : 0;
    int clientFd = ::accept(sockFd, addr ? (struct sockaddr*)addr : nullptr, &len);
    if (clientFd < 0) return nullptr;
    return new Socket(clientFd);
}

void Socket::close() {
    if (sockFd >= 0) {
        ::close(sockFd);
        sockFd = -1;
    }
}
