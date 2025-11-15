#include <gtest/gtest.h>
#include "PriorityBuffer.h"

struct Item {
    int value;
    Item(int v) : value(v) {}
    bool operator<(const Item& other) const {
        return value < other.value;
    }
};

TEST(PriorityCircularBufferTest, AccessWithAtAndOperatorIndex) {
    PriorityCircularBuffer<Item, 4> buffer;

    EXPECT_TRUE(buffer.empty());

    Item a(5), b(15);
    buffer.insert(&a);
    buffer.insert(&b);

    EXPECT_EQ(buffer[0], buffer.at(0));
    EXPECT_EQ(buffer[1], buffer.at(1));

    EXPECT_EQ(buffer[0]->value, buffer.at(0)->value);
    EXPECT_EQ(buffer[1]->value, buffer.at(1)->value);
}

TEST(PriorityCircularBufferTest, InsertShouldPlaceHighestFirstAfterReorder) {
    PriorityCircularBuffer<Item, 4> buffer;

    Item a(10), b(30), c(20);

    buffer.insert(&a);
    buffer.insert(&b);
    buffer.insert(&c);

    ASSERT_NE(buffer.front(), nullptr);
    EXPECT_EQ(buffer.front()->value, 30); // b

    EXPECT_EQ(buffer[1]->value, 20); // c
    EXPECT_EQ(buffer.back()->value, 10); // a

    buffer.pop(); 
    ASSERT_NE(buffer.front(), nullptr);
    EXPECT_EQ(buffer.front()->value, 20); // c
}

TEST(PriorityCircularBufferTest, InsertWithoutReorderKeepsOrder) {
    PriorityCircularBuffer<Item, 4> buffer;

    Item a(10), b(30), c(20);

    buffer.insert(&a, false);
    buffer.insert(&b, false);
    buffer.insert(&c, false);

    buffer.reorder();

    ASSERT_NE(buffer.front(), nullptr);
    EXPECT_EQ(buffer.front()->value, 30);   // b
    EXPECT_EQ(buffer[1]->value, 20);        // c
    EXPECT_EQ(buffer.back()->value, 10);    // a

}

TEST(PriorityCircularBufferTest, PopAndEraseByIndexShouldWork) {
    PriorityCircularBuffer<Item, 4> buffer;

    Item a(10), b(20);

    buffer.insert(&a);
    buffer.insert(&b);

    EXPECT_EQ(buffer.size(), 2);
    buffer.pop();
    EXPECT_EQ(buffer.size(), 1);

    EXPECT_TRUE(buffer.erase(0)); // Deleting the last one!
    EXPECT_TRUE(buffer.empty());
}

TEST(PriorityCircularBufferTest, ClearEmptiesBuffer) {
    PriorityCircularBuffer<Item, 4> buffer;

    Item a(10);
    buffer.insert(&a);

    EXPECT_FALSE(buffer.empty());
    buffer.clear();
    EXPECT_TRUE(buffer.empty());
}
