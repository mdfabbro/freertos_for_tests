#ifndef RESPONDER_H
#define RESPONDER_H

#include "Http/Interfaces.h"
#include <cstring>
#include <cstdio>

class Responder : public Http::IResponse {
public:
    explicit Responder(bool printEnable)
        : enablePrint(printEnable) {}
    
    void setClientSession(Http::IClientSession* session_) override {
        session = session_;
    }
    
    void respond() override;
private:
    Http::IClientSession* session {nullptr};
    bool enablePrint{false};
};

#endif // RESPONDER_H
