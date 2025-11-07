#include <gtest/gtest.h>
#include "Queue/QueueFixed.h"
#include "LeakDetector/LeakDetector.h"

struct Item {
    int value;
    explicit Item(int v) : value(v) {}
};

TEST(QueueFixedTest, PushShouldInsertInFIFOOrder) {
    QueueFixed<Item, 8> queue;

    Item a(10), b(20), c(30);

    EXPECT_TRUE(queue.push(&a));
    EXPECT_TRUE(queue.push(&b));
    EXPECT_TRUE(queue.push(&c));

    ASSERT_NE(queue.front(), nullptr);
    EXPECT_EQ(queue.front()->value, 10);

    ASSERT_NE(queue.back(), nullptr);
    EXPECT_EQ(queue.back()->value, 30);
}

TEST(QueueFixedTest, PopShouldRemoveInFIFOOrder) {
    QueueFixed<Item, 8> queue;

    Item a(10), b(20), c(30);
    queue.push(&a);
    queue.push(&b);
    queue.push(&c);

    EXPECT_TRUE(queue.pop()); // removes 'a'
    EXPECT_EQ(queue.front()->value, 20);

    EXPECT_TRUE(queue.pop()); // removes 'b'
    EXPECT_EQ(queue.front()->value, 30);

    EXPECT_TRUE(queue.pop()); // removes 'c'
    EXPECT_EQ(queue.front(), nullptr);
    EXPECT_FALSE(queue.pop()); // already empty
}

TEST(QueueFixedTest, MoveConstructorShouldTransferOwnership) {
    QueueFixed<Item, 8> q1;

    Item a(1), b(2);
    q1.push(&a);
    q1.push(&b);

    QueueFixed<Item, 8> q2(static_cast<QueueFixed<Item,8>&&>(q1));

    ASSERT_NE(q2.front(), nullptr);
    EXPECT_EQ(q2.front()->value, 1);
    ASSERT_NE(q2.back(), nullptr);
    EXPECT_EQ(q2.back()->value, 2);

    EXPECT_EQ(q1.front(), nullptr);
    EXPECT_EQ(q1.back(), nullptr);
    EXPECT_FALSE(q1.pop());
}

TEST(QueueFixedTest, MoveAssignmentShouldTransferOwnership) {
    QueueFixed<Item, 8> q1;
    QueueFixed<Item, 8> q2;

    Item a(10), b(20);
    q1.push(&a);
    q1.push(&b);

    q2 = static_cast<QueueFixed<Item,8>&&>(q1);

    ASSERT_NE(q2.front(), nullptr);
    EXPECT_EQ(q2.front()->value, 10);
    ASSERT_NE(q2.back(), nullptr);
    EXPECT_EQ(q2.back()->value, 20);

    EXPECT_EQ(q1.front(), nullptr);
    EXPECT_EQ(q1.back(), nullptr);
    EXPECT_FALSE(q1.pop());
}

TEST(QueueFixedTest, DefaultConstructorShouldInitializeEmpty) {
    QueueFixed<Item, 8> queue;

    EXPECT_EQ(queue.front(), nullptr);
    EXPECT_EQ(queue.back(), nullptr);
    EXPECT_FALSE(queue.pop());
}

TEST(QueueFixedTest, ShouldNotAllowPushWhenFull) {
    QueueFixed<Item, 5> queue;

    Item a(1), b(2), c(3), d(4), e(5);

    EXPECT_TRUE(queue.push(&a));
    EXPECT_TRUE(queue.push(&b));
    EXPECT_TRUE(queue.push(&c));
    EXPECT_TRUE(queue.push(&d));
    EXPECT_TRUE(queue.push(&e));

    EXPECT_FALSE(queue.push(&e)); // full

    ASSERT_NE(queue.front(), nullptr);
    EXPECT_EQ(queue.front()->value, 1);
    ASSERT_NE(queue.back(), nullptr);
    EXPECT_EQ(queue.back()->value, 5);

    EXPECT_TRUE(queue.pop());     // remove 'a'
    EXPECT_TRUE(queue.push(&e));  // can push again
}

TEST(QueueFixedTest, VoidQueueBasic) {
    QueueFixed<void,5> queue;

    EXPECT_FALSE(queue.pop()); // Empty

    EXPECT_TRUE(queue.push());
    EXPECT_TRUE(queue.push());
    EXPECT_TRUE(queue.push());
    EXPECT_TRUE(queue.push());
    EXPECT_TRUE(queue.push());

    EXPECT_FALSE(queue.push()); // Full

    EXPECT_TRUE(queue.pop());
    EXPECT_TRUE(queue.pop());
    EXPECT_TRUE(queue.pop());
    EXPECT_TRUE(queue.pop());
    EXPECT_TRUE(queue.pop());
    EXPECT_FALSE(queue.pop()); // Empty again
}

TEST(QueueFixedTest, LeakDetectionsNoLeakWhenLeavingElements) {
    LeakDetectors::HeapSystem().reset();
    const auto pre = LeakDetectors::HeapSystem().in_use();
    {
        QueueFixed<Item, 8> queue;
        Item a(1), b(2), c(3);
        queue.push(&a);
        queue.push(&b);
        queue.push(&c);
    }
    const auto post = LeakDetectors::HeapSystem().in_use();

    EXPECT_EQ(pre, post);
}

TEST(QueueFixedTest, LeakDetectionsNoLeakWhenEmpty) {
    LeakDetectors::HeapSystem().reset();
    const auto pre = LeakDetectors::HeapSystem().in_use();
    {
        QueueFixed<Item, 8> queue;
        Item a(1), b(2), c(3);
        queue.push(&a);
        queue.push(&b);
        queue.push(&c);
        queue.pop();
        queue.pop();
        queue.pop();
    }
    const auto post = LeakDetectors::HeapSystem().in_use();

    EXPECT_EQ(pre, post);
}
