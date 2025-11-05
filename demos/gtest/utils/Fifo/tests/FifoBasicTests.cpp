#include <gtest/gtest.h>
#include "Fifo/FifoBasic.h"

struct Item {
    int value;
    explicit Item(int v) : value(v) {}
};

TEST(FifoBasicTest, PushAndBeginShouldWork) {
    FifoBasic<Item> fifo;

    Item a(10);
    Item b(20);
    fifo.push(&a);
    fifo.push(&b);

    Item* first = fifo.begin();
    ASSERT_EQ(first->value, 10);
}

TEST(FifoBasicTest, PopShouldRemoveFirstElement) {
    FifoBasic<Item> fifo;

    Item a(10);
    Item b(20);
    fifo.push(&a);
    fifo.push(&b);

    ASSERT_TRUE(fifo.pop()); 
    Item* newFirst = fifo.begin();
    EXPECT_EQ(newFirst->value, 20);
}

TEST(FifoBasicTest, PopOnEmptyReturnsFalse) {
    FifoBasic<Item> fifo;
    EXPECT_FALSE(fifo.pop());
}

TEST(FifoBasicTest, MultiplePushAndPopSequence) {
    FifoBasic<Item> fifo;
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
    EXPECT_FALSE(fifo.pop()); 
}
