#include "Server.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>

using namespace Http;

Server::Server(int port, int maxClients)
    : port(port), maxClients(maxClients) {}

Server::~Server() {
    stop();
}

bool Server::start() {
    if (!serverSocket.create() || !serverSocket.bind(port) || !serverSocket.listen(maxClients)) {
        return false;
    } 
    running = true;
    std::cout << "[HTTP][Server] Listening on port: " << port << std::endl;
    acceptThread = std::thread(&Server::acceptLoop, this);
    return true;
}

void Server::stop() {
    running = false;
    serverSocket.close();
    if (acceptThread.joinable())
        acceptThread.join();
    std::lock_guard<std::mutex> lock(clientsMutex);
    for (auto& [id, client] : clients) {
        client->close();
        delete client;
    }
    clients.clear();
}

void Server::acceptLoop() {
    while (running) {
        Socket* clientSock = serverSocket.accept();
        if (!clientSock) continue;
        std::lock_guard<std::mutex> lock(clientsMutex);
        if ((int)clients.size() < maxClients) {
            auto* c = new ClientSession(clientSock);
            const auto clientFd = clientSock->fd();
            clients[clientFd] = c;
            std::cout << "[HTTP][Server] Client '" << clientFd << "' connected\n";
        } else {
            std::cout << "[HTTP][Server] Max clients reached, rejecting '" << clientSock->fd() << "'\n";
            clientSock->close();
            delete clientSock;
        }
    }
}

int Server::getNextClientSessionId() {
    std::lock_guard<std::mutex> lock(clientsMutex);
    for (auto& [id, client] : clients) {
        if (client && !client->isClosed()) return id;
    }
    return -1;
}

IClientSession* Server::getClientSession(int id) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    auto it = clients.find(id);
    return (it != clients.end()) ? it->second : nullptr;
}