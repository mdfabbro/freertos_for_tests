#include <gtest/gtest.h>
#include "Timers/Lists.h"
#include "Timers/Timer.h"
#include "Timers/Command.h"

TEST(TimersSingletonTest, RunningListBasicOperations) {
    auto& list = Timers::Running::GetList();

    // Ensure list starts empty
    list.clear();
    EXPECT_TRUE(list.empty());

    // Add some timers
    Timers::Timer t1("T1", 100, nullptr, false );
    Timers::Timer t2("T2", 200, nullptr, false );
    list.push(&t1);
    list.push(&t2);

    EXPECT_EQ(list.size(), 2u);
    EXPECT_EQ(list.front()->periodInTicks(), 100);
    EXPECT_EQ(list[0]->periodInTicks(), 100);
    EXPECT_EQ(list[1]->periodInTicks(), 200);

    // Clear list
    list.clear();
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0u);
}

TEST(TimersSingletonTest, EnqueuedCommandsBasicOperations) {
    auto& list = Timers::EnqueuedCommands::GetList();

    // Ensure list starts empty
    list.clear();
    EXPECT_TRUE(list.empty());

    // Add some commands
    Timers::Timer t1("T1", 100, nullptr, false );
    Timers::Command c1(Timers::Command::TypeSet::START,&t1);
    Timers::Command c2(Timers::Command::TypeSet::STOP,&t1);
    list.push(&c1);
    list.push(&c2);

    EXPECT_EQ(list.size(), 2u);
    EXPECT_EQ(list.front()->type , Timers::Command::TypeSet::START);
    EXPECT_TRUE(list.pop());
    EXPECT_EQ(list.front()->type , Timers::Command::TypeSet::STOP);

    // Clear list
    list.clear();
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0u);
}
