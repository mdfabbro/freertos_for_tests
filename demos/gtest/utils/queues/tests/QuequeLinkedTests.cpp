#include <gtest/gtest.h>
#include "Queue/QueueLinked.h"
#include "LeakDetector/LeakDetector.h"

struct Item {
    int value;
    explicit Item(int v) : value(v) {}
};

// --- Basic behavior ---

TEST(QueueLinkedTest, PushShouldInsertAtEnd) {
    QueueLinked<Item> queue;

    Item a(10), b(20), c(30);

    EXPECT_TRUE(queue.push(&a));
    EXPECT_TRUE(queue.push(&b));
    EXPECT_TRUE(queue.push(&c));

    ASSERT_NE(queue.front(), nullptr);
    EXPECT_EQ(queue.front()->value, 10);

    ASSERT_NE(queue.back(), nullptr);
    EXPECT_EQ(queue.back()->value, 30);
}

TEST(QueueLinkedTest, PopShouldRemoveFromFront) {
    QueueLinked<Item> queue;

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

TEST(QueueLinkedTest, PushAndPopShouldWorkInFIFOOrder) {
    QueueLinked<Item> queue;

    Item a(10), b(20), c(30);
    queue.push(&a);
    queue.push(&b);
    queue.push(&c);

    ASSERT_EQ(queue.front()->value, 10);
    ASSERT_EQ(queue.back()->value, 30);

    EXPECT_TRUE(queue.pop());
    EXPECT_EQ(queue.front()->value, 20);

    EXPECT_TRUE(queue.pop());
    EXPECT_EQ(queue.front()->value, 30);

    EXPECT_TRUE(queue.pop());
    EXPECT_EQ(queue.front(), nullptr);
}

// --- Move semantics ---

TEST(QueueLinkedTest, MoveConstructorShouldTransferOwnership) {
    QueueLinked<Item> queue1;

    Item a(1), b(2);
    queue1.push(&a);
    queue1.push(&b);

    QueueLinked<Item> queue2(static_cast<QueueLinked<Item>&&>(queue1));

    ASSERT_NE(queue2.front(), nullptr);
    EXPECT_EQ(queue2.front()->value, 1);
    ASSERT_NE(queue2.back(), nullptr);
    EXPECT_EQ(queue2.back()->value, 2);

    EXPECT_EQ(queue1.front(), nullptr);
    EXPECT_EQ(queue1.back(), nullptr);
    EXPECT_FALSE(queue1.pop());
}

TEST(QueueLinkedTest, MoveAssignmentShouldTransferOwnership) {
    QueueLinked<Item> queue1;
    QueueLinked<Item> queue2;

    Item a(10), b(20);
    queue1.push(&a);
    queue1.push(&b);

    queue2 = static_cast<QueueLinked<Item>&&>(queue1);

    ASSERT_NE(queue2.front(), nullptr);
    EXPECT_EQ(queue2.front()->value, 10);
    ASSERT_NE(queue2.back(), nullptr);
    EXPECT_EQ(queue2.back()->value, 20);

    EXPECT_EQ(queue1.front(), nullptr);
    EXPECT_EQ(queue1.back(), nullptr);
    EXPECT_FALSE(queue1.pop());
}

// --- Constructor / Destructor ---

TEST(QueueLinkedTest, DestructorShouldCleanAllElements) {
    auto* queue = new QueueLinked<Item>();

    Item a(1), b(2), c(3);
    queue->push(&a);
    queue->push(&b);
    queue->push(&c);

    delete queue;

    SUCCEED();
}

TEST(QueueLinkedTest, DefaultConstructorShouldInitializeEmpty) {
    QueueLinked<Item> queue;

    EXPECT_EQ(queue.front(), nullptr);
    EXPECT_EQ(queue.back(), nullptr);
    EXPECT_FALSE(queue.pop());
}

// --- Leak detector integration ---

TEST(QueueLinkedTest, LeakDetectionsNoLeakWhenLeavingElements) {
    LeakDetectors::HeapSystem().reset();
    const auto pre = LeakDetectors::HeapSystem().in_use();
    {
        QueueLinked<Item> queue;
        Item a(1), b(2), c(3);
        queue.push(&a);
        queue.push(&b);
        queue.push(&c);
    }
    const auto post = LeakDetectors::HeapSystem().in_use();
    EXPECT_EQ(pre, post);
}

TEST(QueueLinkedTest, LeakDetectionsNoLeakWhenEmpty) {
    LeakDetectors::HeapSystem().reset();
    const auto pre = LeakDetectors::HeapSystem().in_use();
    {
        QueueLinked<Item> queue;
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

