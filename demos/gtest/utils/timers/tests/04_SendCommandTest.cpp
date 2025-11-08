#include <gtest/gtest.h>
#include "Timers/Lists.h"
#include "Timers/Timer.h"
#include "Timers/SendCommand.h"

// Simple test fixture for Timers namespace
class SendCommandTest : public ::testing::Test {
protected:
    void SetUp() override {
        Timers::EnqueuedCommands::GetList().clear();
    }
};

TEST_F(SendCommandTest, SendingStart) {
    auto& list = Timers::EnqueuedCommands::GetList();

    // Ensure list starts empty
    list.clear();
    EXPECT_TRUE(list.empty());

    Timers::Timer t1("T1", 100, nullptr, false );
    Timers::SendCommand::Start(&t1);

    EXPECT_EQ(list.size(), 1u);
    
}

TEST_F(SendCommandTest, SendingReset) {
    auto& list = Timers::EnqueuedCommands::GetList();

    list.clear(); // Ensure list starts empty
    EXPECT_TRUE(list.empty());

    Timers::Timer t1("T1", 100, nullptr, false );
    Timers::SendCommand::Reset(&t1);

    EXPECT_EQ(list.size(), 1u);
}


TEST_F(SendCommandTest, SendingStop) {
    auto& list = Timers::EnqueuedCommands::GetList();

    list.clear(); // Ensure list starts empty
    EXPECT_TRUE(list.empty());

    Timers::Timer t1("T1", 100, nullptr, false );
    Timers::SendCommand::Stop(&t1);

    EXPECT_EQ(list.size(), 1u);
}

TEST_F(SendCommandTest, SendingSingleShot) {
    auto& list = Timers::EnqueuedCommands::GetList();

    list.clear(); // Ensure list starts empty
    EXPECT_TRUE(list.empty());

    Timers::Timer t1("T1", 100, nullptr, false );
    Timers::SendCommand::SingleShot(&t1);

    EXPECT_EQ(list.size(), 1u);
}

TEST_F(SendCommandTest, SendingAutoReload) {
    auto& list = Timers::EnqueuedCommands::GetList();

    list.clear(); // Ensure list starts empty
    EXPECT_TRUE(list.empty());

    Timers::Timer t1("T1", 100, nullptr, false );
    Timers::SendCommand::AutoReload(&t1);

    EXPECT_EQ(list.size(), 1u);
}