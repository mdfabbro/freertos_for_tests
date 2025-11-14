#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <stdint.h>
#include <cstring>
#include <cstddef>
#include <iostream>

enum class MessageID : uint8_t {
    EMERGENCY = 255,  // HIGHEST!
    VERSION = 1,
    ALIVE = 0         // LOWEST
};

struct __attribute__((packed)) Device {
    uint8_t address;
    
    constexpr explicit Device(uint8_t address) : address(address) {}

    bool operator<(const Device& other) {
        return (address > other.address); // The address 0 is the one with the most priority!
    }

    bool operator==(const Device& other) {
        return (address == other.address);
    }

};

struct MessageHeader {

    constexpr MessageHeader(Device origin, Device destination, MessageID id) 
        : origin(origin), destination(destination), id(id) {};

    virtual ~MessageHeader() = default;

    Device origin;         
    Device destination ;
    MessageID id; 

    bool operator<(const MessageHeader& other) {
        if(other.id == MessageID::EMERGENCY && id != MessageID::EMERGENCY) {  // Emergency message is the exception!
            return true; 
        } else if (id == MessageID::EMERGENCY && other.id != MessageID::EMERGENCY) {
            return false;
        }
        if (origin < other.origin) { 
            return true; 
        } else if (origin == other.origin && destination < other.destination) {
            return true;
        } 
        return (origin == other.origin && destination == other.destination && id < other.id);
    }

};

struct  __attribute__((packed)) EmergencyMessage : public MessageHeader {
    char ERROR[8] ;
    EmergencyMessage( Device org, Device dst,  const char * msg )
        : MessageHeader(org, dst, MessageID::EMERGENCY) {
        const auto ErrorLength = (sizeof(ERROR)/sizeof(ERROR[0]));
        const auto msgLength = strlen(msg);
        auto toCopy = ( msgLength < ErrorLength ) ? (msgLength + 1) : ErrorLength;
        for(size_t i = 0; i < toCopy ; ++i ) {
            ERROR[i] = msg[i];
        }
    }

    ~EmergencyMessage() = default;
};  

struct  __attribute__((packed)) VersionMessage : public MessageHeader {
    uint8_t MAYOR ;
    uint8_t MINOR ;
    uint8_t REVISION ;

    VersionMessage( Device org, Device dst, const uint8_t version[3] ) 
        :   MessageHeader(org, dst, MessageID::VERSION), 
            MAYOR(version[0]), MINOR(version[1]), REVISION(version[2]) { }
};  

struct  __attribute__((packed)) AliveMessage : public MessageHeader {
    uint32_t ticks ;

    AliveMessage( Device org, Device dst, uint32_t ticks  ) 
        : MessageHeader(org, dst, MessageID::ALIVE),  ticks(ticks) { }
};  

#endif // __MESSAGE_H__