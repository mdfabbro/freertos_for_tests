#include <gtest/gtest.h>
#include "Queue/FifoBasic.h"

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

TEST(FifoBasicTest, MoveConstructorShouldTransferOwnership) {
    FifoBasic<Item> fifo1;

    Item a(10), b(20);
    fifo1.push(&a);
    fifo1.push(&b);

    // As we dont have the move we cast it 
    FifoBasic<Item> fifo2(static_cast<FifoBasic<Item>&&>(fifo1));

    // fifo2 debería tener los datos
    ASSERT_NE(fifo2.begin(), nullptr);
    EXPECT_EQ(fifo2.begin()->value, 10);

    // fifo1 debería quedar vacío
    EXPECT_EQ(fifo1.begin(), nullptr);
    EXPECT_FALSE(fifo1.pop());
}

TEST(FifoBasicTest, MoveAssignmentShouldTransferOwnership) {
    FifoBasic<Item> fifo1;
    FifoBasic<Item> fifo2;

    Item a(100), b(200);
    fifo1.push(&a);
    fifo1.push(&b);

    // Asignación por movimiento sin std::move
    fifo2 = static_cast<FifoBasic<Item>&&>(fifo1);

    // fifo2 debería contener los elementos
    ASSERT_NE(fifo2.begin(), nullptr);
    EXPECT_EQ(fifo2.begin()->value, 100);
    EXPECT_TRUE(fifo2.pop());
    EXPECT_EQ(fifo2.begin()->value, 200);

    // fifo1 debería quedar vacío
    EXPECT_EQ(fifo1.begin(), nullptr);
    EXPECT_FALSE(fifo1.pop());
}
