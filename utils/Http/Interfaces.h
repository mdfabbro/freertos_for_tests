#ifndef __HTTP_ICLIENTSESSION_H__
#define __HTTP_ICLIENTSESSION_H__

#include <cstddef>

namespace Http {

class IClientSession {
public:
    virtual ~IClientSession() = default;

    virtual size_t read(char* buffer, size_t bufferSize) = 0;
    virtual size_t write(const char* buffer, size_t len) = 0;
    virtual size_t available() const = 0;
    virtual void close() = 0;
    virtual bool isClosed() const = 0;
    virtual int id() const = 0;
};

class IClientSessionController {
public:
    virtual ~IClientSessionController() = default;

    virtual int getNextClientSessionId() = 0;
    virtual IClientSession* getClientSession(int id) = 0;
};

class IResponse {
public:
    virtual ~IResponse() = default;

    virtual void setClientSession(IClientSession* session) = 0;
    virtual void respond() = 0;
};

}
#endif // __HTTP_ICLIENTSESSION_H__
