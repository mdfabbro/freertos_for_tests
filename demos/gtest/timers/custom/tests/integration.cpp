#include <gtest/gtest.h>

#include "Timers/Custom/LinkedList.h"
#include "Timers/Custom/Handler.h"

struct Node {
    int value;
    Node* next;
};

bool lessEqual(Node* a, Node* b) {
    return a->value <= b->value;
}

class LinkedListTest : public ::testing::Test {
protected:
    LinkedList<Node> list;
    Node n1{1, nullptr};
    Node n2{2, nullptr};
    Node n3{3, nullptr};

    void TearDown() override {
        // Clean up to avoid dangling pointers
        n1.next = n2.next = n3.next = nullptr;
    }
};

// --- Test: Insert Front ---
TEST_F(LinkedListTest, InsertFrontAddsToHead) {
    list.insertFront(&n1);
    EXPECT_EQ(list.begin(), &n1);
    EXPECT_EQ(n1.next, nullptr);

    list.insertFront(&n2);
    EXPECT_EQ(list.begin(), &n2);
    EXPECT_EQ(n2.next, &n1);
}

// --- Test: Insert Back ---
TEST_F(LinkedListTest, InsertBackAppendsToEnd) {
    list.insertBack(&n1);
    list.insertBack(&n2);
    EXPECT_EQ(list.begin(), &n1);
    EXPECT_EQ(n1.next, &n2);
    EXPECT_EQ(n2.next, nullptr);
}

// --- Test: Contains ---
TEST_F(LinkedListTest, ContainsFindsElements) {
    list.insertBack(&n1);
    list.insertBack(&n2);
    EXPECT_TRUE(list.contains(&n1));
    EXPECT_TRUE(list.contains(&n2));
    EXPECT_FALSE(list.contains(&n3));
}

// --- Test: Remove ---
TEST_F(LinkedListTest, RemoveElementRemovesCorrectly) {
    list.insertBack(&n1);
    list.insertBack(&n2);
    list.insertBack(&n3);

    Node* removed = list.remove(&n2);
    EXPECT_EQ(removed, &n2);
    EXPECT_EQ(n2.next, nullptr); // next was cleared
    EXPECT_FALSE(list.contains(&n2));
}

// --- Test: Insert with Comparator ---
TEST_F(LinkedListTest, InsertWithComparatorPlacesCorrectly) {
    // Start with n2 -> n3
    list.insertBack(&n2);
    list.insertBack(&n3);

    // Insert n1 in order using lessEqual
    auto isLower = [](Node *a, Node* b) -> bool { return a->value <= b->value; };
    list.insert(&n1, isLower);

    EXPECT_EQ(list.begin(), &n1);
    EXPECT_EQ(n1.next, &n2);
    EXPECT_EQ(n2.next, &n3);
}

// --- Test: Begin returns head ---
TEST_F(LinkedListTest, BeginReturnsHeadPointer) {
    list.insertBack(&n1);
    EXPECT_EQ(list.begin(), &n1);
}


TEST(TimerTest, BeginTimerTest) {
    CustomTimer::Timer t {"TimerA", 100, nullptr, true};
    t.start();
    t.reset();
    t.stop();
    
    EXPECT_EQ(CustomTimer::Command::popFront()->type, CustomTimer::Command::Type::START);
    EXPECT_EQ(CustomTimer::Command::popFront()->type, CustomTimer::Command::Type::RESET);
    EXPECT_EQ(CustomTimer::Command::popFront()->type, CustomTimer::Command::Type::STOP);
}
