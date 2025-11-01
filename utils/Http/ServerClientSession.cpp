#include "Server.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>

using namespace Http;

Server::ClientSession::ClientSession(Socket* s)
    : sock(s), closed(false) {}

Server::ClientSession::~ClientSession() {
    close();
    delete sock;
}

size_t Server::ClientSession::read(char* out, size_t maxLen) {
    if (closed) return 0;
    const ssize_t bytes = ::recv(sock->fd(), out, maxLen, MSG_DONTWAIT);
    if (bytes <= 0) { closed = true; return 0; }

    std::lock_guard<std::mutex> lock(bufferMutex);
    buffer.insert(buffer.end(), out, out + bytes);
    return static_cast<size_t>(bytes);
}

size_t Server::ClientSession::write(const char* data, size_t len) {
    if (closed) return 0;
    const ssize_t res = ::send(sock->fd(), data, len, 0);
    if (res <= 0) { closed = true; return 0; }
    return static_cast<size_t>(res);
}

size_t Server::ClientSession::available() const {
    std::lock_guard<std::mutex> lock(bufferMutex);
    return buffer.size();
}

void Server::ClientSession::close() {
    if (!closed) {
        sock->close();
        closed = true;
    }
}

bool Server::ClientSession::isClosed() const {
    return closed;
}

int Server::ClientSession::id() const {
    return sock->fd();
}
