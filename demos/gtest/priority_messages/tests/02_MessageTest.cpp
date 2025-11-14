#include <gtest/gtest.h>
#include "PriorityBuffer.h"
#include "Message.h"

TEST(MessagesTest, AccessWithAtAndOperatorIndex) {
    PriorityCircularBuffer<MessageHeader, 4> messages;
    const uint8_t versionData[] = { 1, 200, 5 };

    EXPECT_TRUE(messages.empty());

    AliveMessage alive{Device{3}, Device{0}, 100};                 // Second most important as Device{3} is the origin
    EmergencyMessage emergency( Device{10}, Device{0} ,"timeout"); // This is the highst priority
    VersionMessage version(Device{10}, Device{5}, versionData );   // Lowest priority

    messages.insert(&alive);
    messages.insert(&emergency);
    messages.insert(&version);

    // Comparing at with operator[]
    EXPECT_EQ(messages[0], messages.at(0));
    EXPECT_EQ(messages[1], messages.at(1));
    EXPECT_EQ(messages[2], messages.at(2));

    // Checking the priorities
    EXPECT_EQ(messages[0]->id, MessageID::EMERGENCY);
    EXPECT_EQ(messages[1]->id, MessageID::ALIVE);
    EXPECT_EQ(messages[2]->id, MessageID::VERSION);

}

TEST(MessagesTest, TwoEmergenciesDifferentOrigin) {
    PriorityCircularBuffer<MessageHeader, 4> messages;
    EXPECT_TRUE(messages.empty());

    EmergencyMessage emergencyA( Device{10}, Device{0} ,"DevA"); 
    EmergencyMessage emergencyB( Device{5}, Device{0} ,"DevB"); 

    messages.insert(&emergencyA);
    messages.insert(&emergencyB);

    // Checking the priorities
    EXPECT_EQ(messages.size(), 2);
    EXPECT_EQ(messages[0]->id, MessageID::EMERGENCY);
    EXPECT_EQ(messages[1]->id, MessageID::EMERGENCY);

    // As both are emergency msgs, the lower address should go first.
    EXPECT_EQ(messages.front()->origin.address, 5);
    EXPECT_EQ(messages.back()->origin.address, 10);

    // Changing the insert order mut keep the priority
    messages.clear();
    messages.insert(&emergencyB);
    messages.insert(&emergencyA);
    
    // As both are emergency msgs, the lower address should go first.
    EXPECT_EQ(messages.front()->origin.address, 5);
    EXPECT_EQ(messages.back()->origin.address, 10);
}

TEST(MessagesTest, TwoEmergenciesDifferentDestination) {
    PriorityCircularBuffer<MessageHeader, 4> messages;
    EXPECT_TRUE(messages.empty());

    EmergencyMessage emergencyA( Device{3}, Device{10} ,"DevA"); 
    EmergencyMessage emergencyB( Device{3}, Device{5} ,"DevB"); 

    messages.insert(&emergencyA);
    messages.insert(&emergencyB);

    // Checking the priorities
    EXPECT_EQ(messages.size(), 2);
    EXPECT_EQ(messages[0]->id, MessageID::EMERGENCY);
    EXPECT_EQ(messages[1]->id, MessageID::EMERGENCY);

    // As both are emergency msgs, the lower address should go first.
    EXPECT_EQ(messages.front()->destination.address, 5);
    EXPECT_EQ(messages.back()->destination.address, 10);

    // Changing the insert order mut keep the priority
    messages.clear();
    messages.insert(&emergencyB);
    messages.insert(&emergencyA);
    
    // As both are emergency msgs, the lower address should go first.
    EXPECT_EQ(messages.front()->destination.address, 5);
    EXPECT_EQ(messages.back()->destination.address, 10);
}
