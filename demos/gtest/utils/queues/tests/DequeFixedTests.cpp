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