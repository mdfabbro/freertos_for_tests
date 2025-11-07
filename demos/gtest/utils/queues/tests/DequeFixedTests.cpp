#include <gtest/gtest.h>
#include "Queue/DequeFixed.h"
#include "LeakDetector/LeakDetector.h"

struct Item {
    int value;
    explicit Item(int v) : value(v) {}
};


TEST(DequeFixedTest, PushBackShouldInsertAtEnd) {
    DequeFixed<Item, 8> deque;

    Item a(10), b(20), c(30);

    EXPECT_TRUE(deque.push_back(&a));
    EXPECT_TRUE(deque.push_back(&b));
    EXPECT_TRUE(deque.push_back(&c));

    ASSERT_NE(deque.front(), nullptr);
    EXPECT_EQ(deque.front()->value, 10);

    ASSERT_NE(deque.back(), nullptr);
    EXPECT_EQ(deque.back()->value, 30);
}

TEST(DequeFixedTest, PushFrontShouldInsertAtBeginning) {
    DequeFixed<Item, 8> deque;

    Item a(10), b(20), c(30);

    EXPECT_TRUE(deque.push_front(&a));
    EXPECT_TRUE(deque.push_front(&b));
    EXPECT_TRUE(deque.push_front(&c));

    ASSERT_NE(deque.front(), nullptr);
    EXPECT_EQ(deque.front()->value, 30);

    ASSERT_NE(deque.back(), nullptr);
    EXPECT_EQ(deque.back()->value, 10);
}

TEST(DequeFixedTest, PopFrontShouldRemoveFromBeginning) {
    DequeFixed<Item, 8> deque;

    Item a(10), b(20), c(30);
    deque.push_back(&a);
    deque.push_back(&b);
    deque.push_back(&c);

    EXPECT_TRUE(deque.pop_front()); // removes 'a'
    EXPECT_EQ(deque.front()->value, 20);

    EXPECT_TRUE(deque.pop_front()); // removes 'b'
    EXPECT_EQ(deque.front()->value, 30);

    EXPECT_TRUE(deque.pop_front()); // removes 'c'
    EXPECT_EQ(deque.front(), nullptr);
    EXPECT_FALSE(deque.pop_front()); // already empty
}

TEST(DequeFixedTest, PopBackShouldRemoveFromEnd) {
    DequeFixed<Item, 8> deque;

    Item a(10), b(20), c(30);
    deque.push_back(&a);
    deque.push_back(&b);
    deque.push_back(&c);

    EXPECT_TRUE(deque.pop_back());  // removes 'c'
    EXPECT_EQ(deque.back()->value, 20);

    EXPECT_TRUE(deque.pop_back());  // removes 'b'
    EXPECT_EQ(deque.back()->value, 10);

    EXPECT_TRUE(deque.pop_back());  // removes 'a'
    EXPECT_EQ(deque.back(), nullptr);
    EXPECT_FALSE(deque.pop_back()); // already empty
}

TEST(DequeFixedTest, PushFrontAndBackShouldWorkTogether) {
    DequeFixed<Item, 8> deque;

    Item a(10), b(20), c(30);
    deque.push_back(&a);
    deque.push_front(&b);
    deque.push_back(&c);

    // Order: b (front), a, c (back)
    ASSERT_NE(deque.front(), nullptr);
    EXPECT_EQ(deque.front()->value, 20);

    ASSERT_NE(deque.back(), nullptr);
    EXPECT_EQ(deque.back()->value, 30);
}

TEST(DequeFixedTest, PopFrontAndBackShouldLeaveEmpty) {
    DequeFixed<Item, 8> deque;

    Item a(10), b(20);
    deque.push_back(&a);
    deque.push_back(&b);

    EXPECT_TRUE(deque.pop_back());  // remove 'b'
    EXPECT_TRUE(deque.pop_front()); // remove 'a'

    EXPECT_EQ(deque.front(), nullptr);
    EXPECT_EQ(deque.back(), nullptr);
    EXPECT_FALSE(deque.pop_back());
    EXPECT_FALSE(deque.pop_front());
}

// --- Tests de move semantics ---

TEST(DequeFixedTest, MoveConstructorShouldTransferOwnership) {
    DequeFixed<Item, 8> deque1;

    Item a(1), b(2);
    deque1.push_back(&a);
    deque1.push_back(&b);

    DequeFixed<Item, 8> deque2(static_cast<DequeFixed<Item,8>&&>(deque1));

    ASSERT_NE(deque2.front(), nullptr);
    EXPECT_EQ(deque2.front()->value, 1);
    ASSERT_NE(deque2.back(), nullptr);
    EXPECT_EQ(deque2.back()->value, 2);

    EXPECT_EQ(deque1.front(), nullptr);
    EXPECT_EQ(deque1.back(), nullptr);
    EXPECT_FALSE(deque1.pop_front());
    EXPECT_FALSE(deque1.pop_back());
}

TEST(DequeFixedTest, MoveAssignmentShouldTransferOwnership) {
    DequeFixed<Item, 8> deque1;
    DequeFixed<Item, 8> deque2;

    Item a(10), b(20);
    deque1.push_back(&a);
    deque1.push_back(&b);

    deque2 = static_cast<DequeFixed<Item,8>&&>(deque1);

    ASSERT_NE(deque2.front(), nullptr);
    EXPECT_EQ(deque2.front()->value, 10);
    ASSERT_NE(deque2.back(), nullptr);
    EXPECT_EQ(deque2.back()->value, 20);

    EXPECT_EQ(deque1.front(), nullptr);
    EXPECT_EQ(deque1.back(), nullptr);
    EXPECT_FALSE(deque1.pop_front());
    EXPECT_FALSE(deque1.pop_back());
}

TEST(DequeFixedTest, DestructorShouldCleanAllElements) {
    auto* deque = new DequeFixed<Item, 8>();

    Item a(1), b(2), c(3);
    deque->push_back(&a);
    deque->push_back(&b);
    deque->push_back(&c);

    delete deque;

    SUCCEED();
}

TEST(DequeFixedTest, DefaultConstructorShouldInitializeEmpty) {
    DequeFixed<Item, 8> deque;

    EXPECT_EQ(deque.front(), nullptr);
    EXPECT_EQ(deque.back(), nullptr);
    EXPECT_FALSE(deque.pop_front());
    EXPECT_FALSE(deque.pop_back());
}

TEST(DequeFixedTest, LeakDetectionsNoLeakWhenLeavingElements) {
    LeakDetectors::HeapSystem().reset();
    const auto pre = LeakDetectors::HeapSystem().in_use();
    {
        DequeFixed<Item, 8> deque;
        Item a(1), b(2), c(3);
        deque.push_back(&a);
        deque.push_back(&b);
        deque.push_back(&c);
    }
    const auto post = LeakDetectors::HeapSystem().in_use();

    EXPECT_EQ(pre, post);
}

TEST(DequeFixedTest, LeakDetectionsNoLeakWhenEmpty) {
    LeakDetectors::HeapSystem().reset();
    const auto pre = LeakDetectors::HeapSystem().in_use();
    {
        DequeFixed<Item, 8> deque;
        Item a(1), b(2), c(3);
        deque.push_back(&a);
        deque.push_back(&b);
        deque.push_back(&c);
        deque.pop_back();
        deque.pop_front();
        deque.pop_back();
    }
    const auto post = LeakDetectors::HeapSystem().in_use();

    EXPECT_EQ(pre, post);
}

TEST(DequeFixedTest, ShouldNotAllowPushWhenFull) {
    DequeFixed<Item, 5> deque;

    Item a(1), b(2), c(3), d(4), e(5);

    // Fill it!
    EXPECT_TRUE(deque.push_back(&a));
    EXPECT_TRUE(deque.push_back(&b));
    EXPECT_TRUE(deque.push_back(&c));
    EXPECT_TRUE(deque.push_back(&d));
    EXPECT_TRUE(deque.push_back(&e));   

    // It should be full!
    EXPECT_FALSE(deque.push_back(&e));   
    EXPECT_FALSE(deque.push_front(&e)); 

    ASSERT_NE(deque.front(), nullptr);
    EXPECT_EQ(deque.front()->value, 1);
    ASSERT_NE(deque.back(), nullptr);
    EXPECT_EQ(deque.back()->value, 5);

    EXPECT_TRUE(deque.pop_front());     
    EXPECT_TRUE(deque.push_back(&e));  

    EXPECT_EQ(deque.back()->value, 5);
}

TEST(DequeFixedTest, VoidQueueBasic) {
    DequeFixed<void,5> deque;

    EXPECT_FALSE(deque.pop_front()); // Empty!

    EXPECT_TRUE(deque.push_back());
    EXPECT_TRUE(deque.push_back());
    EXPECT_TRUE(deque.push_back());
    EXPECT_TRUE(deque.push_back());
    EXPECT_TRUE(deque.push_back());   

    // It should be full!
    EXPECT_FALSE(deque.push_back());   
    EXPECT_FALSE(deque.push_front()); 

    EXPECT_TRUE(deque.pop_front());
    EXPECT_TRUE(deque.pop_front());
    EXPECT_TRUE(deque.pop_front());
    EXPECT_TRUE(deque.pop_front());
    EXPECT_TRUE(deque.pop_front());
    EXPECT_FALSE(deque.pop_front()); // Empty!

}

TEST(DequeFixedTest, AtShouldReturnCorrectElementByIndex) {
    DequeFixed<Item, 5> deque;

    Item a(10), b(20), c(30);
    deque.push_back(&a);
    deque.push_back(&b);
    deque.push_back(&c);

    ASSERT_NE(deque.at(0), nullptr);
    EXPECT_EQ(deque.at(0)->value, 10);

    ASSERT_NE(deque.at(1), nullptr);
    EXPECT_EQ(deque.at(1)->value, 20);

    ASSERT_NE(deque.at(2), nullptr);
    EXPECT_EQ(deque.at(2)->value, 30);

    // Out of range indices should return nullptr
    EXPECT_EQ(deque.at(3), nullptr);
    EXPECT_EQ(deque.at(99), nullptr);
}

TEST(DequeFixedTest, OperatorIndexShouldBehaveLikeAt) {
    DequeFixed<Item, 5> deque;

    Item a(1), b(2), c(3);
    deque.push_back(&a);
    deque.push_back(&b);
    deque.push_back(&c);

    // Operator[] should behave the same as at()
    EXPECT_EQ((*deque[0]).value, 1);
    EXPECT_EQ((*deque[1]).value, 2);
    EXPECT_EQ((*deque[2]).value, 3);

    // Out of range should return nullptr
    EXPECT_EQ(deque[3], nullptr);
}

TEST(DequeFixedTest, AtShouldHandleWrapAroundCorrectly) {
    DequeFixed<Item, 5> deque;

    Item a(1), b(2), c(3), d(4), e(5);

    deque.push_back(&a);
    deque.push_back(&b);
    deque.push_back(&c);
    deque.push_back(&d);
    deque.push_back(&e);

    // Remove two elements from the front to move the start pointer
    deque.pop_front(); // removes 'a'
    deque.pop_front(); // removes 'b'

    Item f(6), g(7);
    deque.push_back(&f);
    deque.push_back(&g);

    // The circular buffer now contains: c, d, e, f, g
    EXPECT_EQ(deque.at(0)->value, 3);
    EXPECT_EQ(deque.at(1)->value, 4);
    EXPECT_EQ(deque.at(2)->value, 5);
    EXPECT_EQ(deque.at(3)->value, 6);
    EXPECT_EQ(deque.at(4)->value, 7);

    // Out of range
    EXPECT_EQ(deque.at(5), nullptr);
}

TEST(DequeFixedTest, OperatorIndexShouldHandleWrapAroundCorrectly) {
    DequeFixed<Item, 5> deque;

    Item a(10), b(20), c(30), d(40), e(50);

    deque.push_back(&a);
    deque.push_back(&b);
    deque.push_back(&c);
    deque.push_back(&d);
    deque.push_back(&e);

    // Remove two from the front to force circular indexing
    deque.pop_front();
    deque.pop_front();

    Item f(60), g(70);
    deque.push_back(&f);
    deque.push_back(&g);

    // Circular order: c, d, e, f, g
    EXPECT_EQ(deque[0]->value, 30);
    EXPECT_EQ(deque[1]->value, 40);
    EXPECT_EQ(deque[2]->value, 50);
    EXPECT_EQ(deque[3]->value, 60);
    EXPECT_EQ(deque[4]->value, 70);

    // Out of range
    EXPECT_EQ(deque[5], nullptr);
}

TEST(DequeFixedTest, SizeAndEmptyShouldReflectElementCount) {
    DequeFixed<Item, 5> deque;

    EXPECT_TRUE(deque.empty());
    EXPECT_EQ(deque.size(), 0u);

    Item a(10), b(20);
    deque.push_back(&a);
    EXPECT_FALSE(deque.empty());
    EXPECT_EQ(deque.size(), 1u);

    deque.push_back(&b);
    EXPECT_EQ(deque.size(), 2u);

    deque.pop_front();
    EXPECT_EQ(deque.size(), 1u);

    deque.pop_front();
    EXPECT_TRUE(deque.empty());
    EXPECT_EQ(deque.size(), 0u);
}

TEST(DequeFixedTest, EraseShouldRemoveMiddleElementAndShiftCorrectly) {
    DequeFixed<Item, 8> deque;

    Item a(1), b(2), c(3), d(4), e(5);
    deque.push_back(&a);
    deque.push_back(&b);
    deque.push_back(&c);
    deque.push_back(&d);
    deque.push_back(&e);

    // Erase element at index 2 (the one with value 3)
    EXPECT_TRUE(deque.erase(2));

    // The order should now be: a, b, d, e
    ASSERT_NE(deque.front(), nullptr);
    EXPECT_EQ(deque.at(0)->value, 1);
    EXPECT_EQ(deque.at(1)->value, 2);
    EXPECT_EQ(deque.at(2)->value, 4);
    EXPECT_EQ(deque.at(3)->value, 5);
    EXPECT_EQ(deque.size(), 4u);
}

TEST(DequeFixedTest, EraseShouldHandleFrontAndBackIndices) {
    DequeFixed<Item, 5> deque;

    Item a(10), b(20), c(30);
    deque.push_back(&a);
    deque.push_back(&b);
    deque.push_back(&c);

    // Erase front (index 0)
    EXPECT_TRUE(deque.erase(0));
    ASSERT_NE(deque.front(), nullptr);
    EXPECT_EQ(deque.front()->value, 20);
    EXPECT_EQ(deque.size(), 2u);

    // Erase last (current index 1)
    EXPECT_TRUE(deque.erase(1));
    ASSERT_NE(deque.front(), nullptr);
    EXPECT_EQ(deque.front()->value, 20);
    EXPECT_EQ(deque.size(), 1u);

    // Erase the last remaining element
    EXPECT_TRUE(deque.erase(0));
    EXPECT_TRUE(deque.empty());
    EXPECT_EQ(deque.size(), 0u);
}

TEST(DequeFixedTest, EraseShouldFailWhenIndexIsOutOfRange) {
    DequeFixed<Item, 4> deque;

    Item a(1), b(2);
    deque.push_back(&a);
    deque.push_back(&b);

    EXPECT_FALSE(deque.erase(2)); // out of range
    EXPECT_FALSE(deque.erase(999)); // far out of range

    // Data should remain unchanged
    EXPECT_EQ(deque.size(), 2u);
    EXPECT_EQ(deque.at(0)->value, 1);
    EXPECT_EQ(deque.at(1)->value, 2);
}

TEST(DequeFixedTest, EraseShouldHandleWrapAroundCorrectly) {
    DequeFixed<Item, 5> deque;

    Item a(10), b(20), c(30), d(40), e(50);
    deque.push_back(&a);
    deque.push_back(&b);
    deque.push_back(&c);
    deque.push_back(&d);
    deque.push_back(&e);

    // Pop two from front to move 'start' forward
    deque.pop_front(); // remove a
    deque.pop_front(); // remove b

    // Now order (circular): c, d, e
    Item f(60), g(70);
    deque.push_back(&f);
    deque.push_back(&g);
    // Now buffer is full again (c, d, e, f, g), with wrap-around indexing

    // Remove middle element (e)
    EXPECT_TRUE(deque.erase(2));

    // New order should be: c, d, f, g
    EXPECT_EQ(deque.size(), 4u);
    EXPECT_EQ(deque.at(0)->value, 30);
    EXPECT_EQ(deque.at(1)->value, 40);
    EXPECT_EQ(deque.at(2)->value, 60);
    EXPECT_EQ(deque.at(3)->value, 70);
}

TEST(DequeFixedVoidTest, ShouldStartEmptyAndReportCorrectSize) {
    DequeFixed<void, 5> deque;

    EXPECT_TRUE(deque.empty());
    EXPECT_EQ(deque.size(), 0u);

    EXPECT_TRUE(deque.push_back());
    EXPECT_FALSE(deque.empty());
    EXPECT_EQ(deque.size(), 1u);

    EXPECT_TRUE(deque.push_front());
    EXPECT_EQ(deque.size(), 2u);

    EXPECT_TRUE(deque.pop_back());
    EXPECT_EQ(deque.size(), 1u);

    EXPECT_TRUE(deque.pop_front());
    EXPECT_TRUE(deque.empty());
    EXPECT_EQ(deque.size(), 0u);
}

TEST(DequeFixedVoidTest, ShouldHandleFullBufferCorrectly) {
    DequeFixed<void, 5> deque;

    // Fill it completely
    for (int i = 0; i < 5; ++i) {
        EXPECT_TRUE(deque.push_back());
    }

    EXPECT_EQ(deque.size(), 5u);
    EXPECT_FALSE(deque.push_back()); // should fail, full

    EXPECT_TRUE(deque.pop_front());
    EXPECT_EQ(deque.size(), 4u);

    EXPECT_TRUE(deque.push_back());
    EXPECT_EQ(deque.size(), 5u);
}

TEST(DequeFixedVoidTest, EraseShouldRemoveCorrectElements) {
    DequeFixed<void, 5> deque;

    for (int i = 0; i < 5; ++i) {
        EXPECT_TRUE(deque.push_back());
    }

    EXPECT_EQ(deque.size(), 5u);

    // Remove first element
    EXPECT_TRUE(deque.erase(0));
    EXPECT_EQ(deque.size(), 4u);

    // Remove last element (index 3 after one removal)
    EXPECT_TRUE(deque.erase(3));
    EXPECT_EQ(deque.size(), 3u);

    // Remove middle element
    EXPECT_TRUE(deque.erase(1));
    EXPECT_EQ(deque.size(), 2u);

    // Out-of-range index
    EXPECT_FALSE(deque.erase(10));
    EXPECT_EQ(deque.size(), 2u);
}

TEST(DequeFixedVoidTest, ShouldBehaveCorrectlyAfterPushAndPopMix) {
    DequeFixed<void, 5> deque;

    EXPECT_TRUE(deque.push_back());
    EXPECT_TRUE(deque.push_back());
    EXPECT_TRUE(deque.push_front());
    EXPECT_TRUE(deque.push_back());
    EXPECT_EQ(deque.size(), 4u);

    EXPECT_TRUE(deque.pop_front());
    EXPECT_EQ(deque.size(), 3u);

    EXPECT_TRUE(deque.erase(1));
    EXPECT_EQ(deque.size(), 2u);

    EXPECT_TRUE(deque.pop_back());
    EXPECT_TRUE(deque.pop_front());
    EXPECT_TRUE(deque.empty());
}

TEST(DequeFixedVoidTest, AtAndOperatorIndexShouldHandleBoundsProperly) {
    DequeFixed<void, 5> deque;

    EXPECT_EQ(deque.at(0), nullptr);
    EXPECT_EQ(deque[0], nullptr);

    EXPECT_TRUE(deque.push_back());
    EXPECT_TRUE(deque.push_back());
    EXPECT_TRUE(deque.push_back());

    EXPECT_EQ(deque.at(0), nullptr);
    EXPECT_EQ(deque[1], nullptr);
    EXPECT_EQ(deque.at(2), nullptr);

    EXPECT_EQ(deque.at(3), nullptr); // out of range
    EXPECT_EQ(deque[4], nullptr);
}

TEST(DequeFixedVoidTest, ShouldAllowFullEraseCycle) {
    DequeFixed<void, 5> deque;

    for (int i = 0; i < 5; ++i)
        EXPECT_TRUE(deque.push_back());

    EXPECT_EQ(deque.size(), 5u);

    for (int i = 0; i < 5; ++i)
        EXPECT_TRUE(deque.erase(0));  // remove from front each time

    EXPECT_TRUE(deque.empty());
    EXPECT_EQ(deque.size(), 0u);

    EXPECT_FALSE(deque.erase(0)); // nothing left to erase
}