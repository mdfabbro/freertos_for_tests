#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

#include "Interfaces.h"
#include "Socket.h"
#include <map>
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>

namespace Http {

class Server : public IClientSessionController {
public:
    Server(int port, int maxClients);
    ~Server();

    bool start();
    void stop();

    int getNextClientSessionId();
    IClientSession* getClientSession(int id) override;

    struct ClientSession : public IClientSession {
        explicit ClientSession(Socket* sock);
        ~ClientSession() override;

        size_t read(char* out, size_t maxLen) override;
        size_t write(const char* data, size_t len) override;
        size_t available() const override;
        void close() override;
        bool isClosed() const override;
        int id() const override;

    private:
        Socket* sock;
        bool closed;
        std::vector<char> buffer;
        mutable std::mutex bufferMutex;
    };

private:
    void acceptLoop();

    int port;
    int maxClients;
    Socket serverSocket;
    std::atomic<bool> running{false};

    std::map<int, ClientSession*> clients;
    std::mutex clientsMutex;
    std::thread acceptThread;
};

}

#endif // __HTTP_SERVER_H__
