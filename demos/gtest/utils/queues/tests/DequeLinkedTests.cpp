#include <gtest/gtest.h>
#include "Queue/DequeLinked.h"
#include "LeakDetector/LeakDetector.h"

struct Item {
    int value;
    explicit Item(int v) : value(v) {}
};

TEST(DequeLinkedTest, PushBackShouldInsertAtEnd) {
    DequeLinked<Item> deque;

    Item a(10), b(20), c(30);

    EXPECT_TRUE(deque.push_back(&a));
    EXPECT_TRUE(deque.push_back(&b));
    EXPECT_TRUE(deque.push_back(&c));

    // 'begin' should point to the first inserted element
    ASSERT_NE(deque.front(), nullptr);
    EXPECT_EQ(deque.front()->value, 10);

    // 'end' should point to the last inserted element
    ASSERT_NE(deque.back(), nullptr);
    EXPECT_EQ(deque.back()->value, 30);
}

TEST(DequeLinkedTest, PushFrontShouldInsertAtBeginning) {
    DequeLinked<Item> deque;

    Item a(10), b(20), c(30);

    EXPECT_TRUE(deque.push_front(&a));
    EXPECT_TRUE(deque.push_front(&b));
    EXPECT_TRUE(deque.push_front(&c));

    // After multiple push_front calls, 'begin' should be the last pushed
    ASSERT_NE(deque.front(), nullptr);
    EXPECT_EQ(deque.front()->value, 30);

    // The last pushed_back element (which was first inserted) should be at the end
    ASSERT_NE(deque.back(), nullptr);
    EXPECT_EQ(deque.back()->value, 10);
}

TEST(DequeLinkedTest, PopFrontShouldRemoveFromBeginning) {
    DequeLinked<Item> deque;

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

TEST(DequeLinkedTest, PopBackShouldRemoveFromEnd) {
    DequeLinked<Item> deque;

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

TEST(DequeLinkedTest, PushFrontAndBackShouldWorkTogether) {
    DequeLinked<Item> deque;

    Item a(10), b(20), c(30);
    deque.push_back(&a);
    deque.push_front(&b);
    deque.push_back(&c);

    // Order should now be: b (front), a, c (back)
    ASSERT_NE(deque.front(), nullptr);
    EXPECT_EQ(deque.front()->value, 20);

    ASSERT_NE(deque.back(), nullptr);
    EXPECT_EQ(deque.back()->value, 30);
}

TEST(DequeLinkedTest, PopFrontAndBackShouldLeaveEmpty) {
    DequeLinked<Item> deque;

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

TEST(DequeLinkedTest, MoveConstructorShouldTransferOwnership) {
    DequeLinked<Item> deque1;

    Item a(1), b(2);
    deque1.push_back(&a);
    deque1.push_back(&b);

    // Move construct deque2 from deque1
    DequeLinked<Item> deque2(static_cast<DequeLinked<Item>&&>(deque1));

    // deque2 should now own the nodes
    ASSERT_NE(deque2.front(), nullptr);
    EXPECT_EQ(deque2.front()->value, 1);
    ASSERT_NE(deque2.back(), nullptr);
    EXPECT_EQ(deque2.back()->value, 2);

    // deque1 should be empty after move
    EXPECT_EQ(deque1.front(), nullptr);
    EXPECT_EQ(deque1.back(), nullptr);
    EXPECT_FALSE(deque1.pop_front());
    EXPECT_FALSE(deque1.pop_back());
}

TEST(DequeLinkedTest, MoveAssignmentShouldTransferOwnership) {
    DequeLinked<Item> deque1;
    DequeLinked<Item> deque2;

    Item a(10), b(20);
    deque1.push_back(&a);
    deque1.push_back(&b);

    // Move assign
    deque2 = static_cast<DequeLinked<Item>&&>(deque1);

    // deque2 should now contain elements from deque1
    ASSERT_NE(deque2.front(), nullptr);
    EXPECT_EQ(deque2.front()->value, 10);
    ASSERT_NE(deque2.back(), nullptr);
    EXPECT_EQ(deque2.back()->value, 20);

    // deque1 should now be empty
    EXPECT_EQ(deque1.front(), nullptr);
    EXPECT_EQ(deque1.back(), nullptr);
    EXPECT_FALSE(deque1.pop_front());
    EXPECT_FALSE(deque1.pop_back());
}

TEST(DequeLinkedTest, DestructorShouldCleanAllElements) {
    auto* deque = new DequeLinked<Item>();

    Item a(1), b(2), c(3);
    deque->push_back(&a);
    deque->push_back(&b);
    deque->push_back(&c);

    // The destructor should release all allocated nodes without crash
    delete deque;

    // If the class correctly deletes its elements, Valgrind / ASan should show no leaks
    SUCCEED();
}

TEST(DequeLinkedTest, DefaultConstructorShouldInitializeEmpty) {
    DequeLinked<Item> deque;

    EXPECT_EQ(deque.front(), nullptr);
    EXPECT_EQ(deque.back(), nullptr);
    EXPECT_FALSE(deque.pop_front());
    EXPECT_FALSE(deque.pop_back());
}

TEST(DequeLinkedTest, LeakDetectionsNoLeakWhenLeavingElements) {
    LeakDetectors::HeapSystem().reset();
    const auto pre = LeakDetectors::HeapSystem().in_use();
    {
        DequeLinked<Item> deque;
        Item a(1), b(2), c(3);
        deque.push_back(&a);
        deque.push_back(&b);
        deque.push_back(&c);
    }
    const auto post = LeakDetectors::HeapSystem().in_use();

    EXPECT_EQ(pre, post);
}

TEST(DequeLinkedTest, LeakDetectionsNoLeakWhenEmpty) {
    LeakDetectors::HeapSystem().reset();
    const auto pre = LeakDetectors::HeapSystem().in_use();
    {
        DequeLinked<Item> deque;
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

TEST(DequeLinkedTest, SizeAndEmptyShouldReflectContent) {
    DequeLinked<Item> deque;
    EXPECT_TRUE(deque.empty());
    EXPECT_EQ(deque.size(), 0u);

    Item a(1), b(2);
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

TEST(DequeLinkedTest, AtAndOperatorIndexShouldReturnCorrectElements) {
    DequeLinked<Item> deque;

    Item a(10), b(20), c(30);
    deque.push_back(&a);
    deque.push_back(&b);
    deque.push_back(&c);

    // Using at()
    EXPECT_EQ(deque.at(0)->value, 10);
    EXPECT_EQ(deque.at(1)->value, 20);
    EXPECT_EQ(deque.at(2)->value, 30);
    EXPECT_EQ(deque.at(99), nullptr); // out of range

    // Using operator[]
    EXPECT_EQ((*deque[0]).value, 10);
    EXPECT_EQ(deque[1]->value, 20);
    EXPECT_EQ(deque[2]->value, 30);
    EXPECT_EQ(deque[3], nullptr); // out of range
}

TEST(DequeLinkedTest, EraseShouldRemoveMiddleElementAndMaintainLinks) {
    DequeLinked<Item> deque;

    Item a(1), b(2), c(3), d(4);
    deque.push_back(&a);
    deque.push_back(&b);
    deque.push_back(&c);
    deque.push_back(&d);

    // Remove index 1 (b)
    EXPECT_TRUE(deque.erase(1));

    EXPECT_EQ(deque.size(), 3u);
    EXPECT_EQ(deque.at(0)->value, 1);
    EXPECT_EQ(deque.at(1)->value, 3);
    EXPECT_EQ(deque.at(2)->value, 4);

    // Ensure internal links are still valid by sequential access
    EXPECT_NE(deque.front(), nullptr);
    EXPECT_NE(deque.back(), nullptr);
}

TEST(DequeLinkedTest, EraseShouldHandleFirstAndLastElementsProperly) {
    DequeLinked<Item> deque;

    Item a(10), b(20), c(30);
    deque.push_back(&a);
    deque.push_back(&b);
    deque.push_back(&c);

    // Remove front (index 0)
    EXPECT_TRUE(deque.erase(0));
    EXPECT_EQ(deque.front()->value, 20);
    EXPECT_EQ(deque.size(), 2u);

    // Remove back (last index)
    EXPECT_TRUE(deque.erase(1));
    EXPECT_EQ(deque.front()->value, 20);
    EXPECT_EQ(deque.size(), 1u);

    // Remove remaining element
    EXPECT_TRUE(deque.erase(0));
    EXPECT_TRUE(deque.empty());
}

TEST(DequeLinkedTest, EraseShouldReturnFalseWhenIndexOutOfRange) {
    DequeLinked<Item> deque;

    Item a(1), b(2);
    deque.push_back(&a);
    deque.push_back(&b);

    EXPECT_FALSE(deque.erase(2)); // invalid index
    EXPECT_FALSE(deque.erase(10)); // far out of range

    // Data should remain intact
    EXPECT_EQ(deque.size(), 2u);
    EXPECT_EQ(deque.at(0)->value, 1);
    EXPECT_EQ(deque.at(1)->value, 2);
}

TEST(DequeLinkedTest, EraseShouldWorkAfterSeveralPushAndPopOperations) {
    DequeLinked<Item> deque;

    Item a(5), b(6), c(7), d(8), e(9);
    deque.push_back(&a);
    deque.push_back(&b);
    deque.push_back(&c);
    deque.push_back(&d);
    deque.push_back(&e);

    // Pop some from the front to simulate usage
    deque.pop_front(); // remove a
    deque.pop_front(); // remove b

    // Now: c, d, e
    EXPECT_EQ(deque.front()->value, 7);
    EXPECT_EQ(deque.back()->value, 9);

    // Erase middle element (d)
    EXPECT_TRUE(deque.erase(1));
    EXPECT_EQ(deque.size(), 2u);
    EXPECT_EQ(deque.at(0)->value, 7);
    EXPECT_EQ(deque.at(1)->value, 9);
}
