#include <gtest/gtest.h>
#include "Queue/FifoFixed.h"

struct Item {
    int value;
    explicit Item(int v) : value(v) {}
};

TEST(FifoFixedTest, InitiallyEmpty) {
    FifoFixed<Item, 3> fifo;
    EXPECT_EQ(fifo.begin(), nullptr);
    EXPECT_FALSE(fifo.pop());
}

TEST(FifoFixedTest, PushAndBeginReturnFirstElement) {
    FifoFixed<Item, 3> fifo;
    Item a(10);
    Item b(20);

    EXPECT_TRUE(fifo.push(&a));
    EXPECT_EQ(fifo.begin()->value, 10);

    EXPECT_TRUE(fifo.push(&b));
    EXPECT_EQ(fifo.begin()->value, 10);  // still the first
}

TEST(FifoFixedTest, PopRemovesFirstElement) {
    FifoFixed<Item, 3> fifo;
    Item a(1), b(2), c(3);

    fifo.push(&a);
    fifo.push(&b);
    fifo.push(&c);

    EXPECT_EQ(fifo.begin()->value, 1);
    EXPECT_TRUE(fifo.pop());
    EXPECT_EQ(fifo.begin()->value, 2);
    EXPECT_TRUE(fifo.pop());
    EXPECT_EQ(fifo.begin()->value, 3);
    EXPECT_TRUE(fifo.pop());
    EXPECT_EQ(fifo.begin(), nullptr);
    EXPECT_FALSE(fifo.pop()); // already empty
}

TEST(FifoFixedTest, CannotPushBeyondCapacity) {
    FifoFixed<Item, 2> fifo;
    Item a(1), b(2), c(3);

    EXPECT_TRUE(fifo.push(&a));
    EXPECT_TRUE(fifo.push(&b));
    EXPECT_FALSE(fifo.push(&c)); // full
}

TEST(FifoFixedTest, WrapAroundWorksCorrectly) {
    FifoFixed<Item, 3> fifo;
    Item a(10), b(20), c(30), d(40);

    fifo.push(&a);
    fifo.push(&b);
    fifo.push(&c);

    fifo.pop();           // remove 'a'
    EXPECT_TRUE(fifo.push(&d));  // should wrap and occupy freed slot

    // After one pop and one push, order should be: b, c, d
    EXPECT_EQ(fifo.begin()->value, 20);
    fifo.pop();
    EXPECT_EQ(fifo.begin()->value, 30);
    fifo.pop();
    EXPECT_EQ(fifo.begin()->value, 40);
}

