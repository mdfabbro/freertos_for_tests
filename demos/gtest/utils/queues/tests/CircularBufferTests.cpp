#include <gtest/gtest.h>
#include "Queue/CircularBuffer.h"
#include "LeakDetector/LeakDetector.h"

struct Item {
    int value;
    explicit Item(int v) : value(v) {}
};

TEST(CircularBufferTest, PushShouldInsertAtEnd) {
    CircularBuffer<Item, 4> queue;

    Item a(10), b(20), c(30);

    EXPECT_TRUE(queue.push(&a));
    EXPECT_TRUE(queue.push(&b));
    EXPECT_TRUE(queue.push(&c));

    ASSERT_NE(queue.front(), nullptr);
    EXPECT_EQ(queue.front()->value, 10);

    ASSERT_NE(queue.back(), nullptr);
    EXPECT_EQ(queue.back()->value, 30);
}

TEST(CircularBufferTest, PopShouldRemoveFromFront) {
    CircularBuffer<Item, 4> queue;

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

TEST(CircularBufferTest, PushAndPopShouldWorkInFIFOOrder) {
    CircularBuffer<Item, 4> queue;

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

TEST(CircularBufferTest, ShouldOverwriteOldestWhenFull) {
    CircularBuffer<Item, 3> queue;

    Item a(1), b(2), c(3), d(4);
    queue.push(&a);
    queue.push(&b);
    queue.push(&c);

    // now full, next push should remove oldest ('a')
    EXPECT_TRUE(queue.push(&d));

    ASSERT_NE(queue.front(), nullptr);
    EXPECT_EQ(queue.front()->value, 2);
    ASSERT_NE(queue.back(), nullptr);
    EXPECT_EQ(queue.back()->value, 4);
    EXPECT_EQ(queue.size(), 3);
}

TEST(CircularBufferTest, MoveConstructorShouldTransferOwnership) {
    CircularBuffer<Item, 4> queue1;

    Item a(1), b(2);
    queue1.push(&a);
    queue1.push(&b);

    CircularBuffer<Item, 4> queue2(static_cast<CircularBuffer<Item,4>&&>(queue1));

    ASSERT_NE(queue2.front(), nullptr);
    EXPECT_EQ(queue2.front()->value, 1);
    ASSERT_NE(queue2.back(), nullptr);
    EXPECT_EQ(queue2.back()->value, 2);

    EXPECT_EQ(queue1.front(), nullptr);
    EXPECT_EQ(queue1.back(), nullptr);
    EXPECT_FALSE(queue1.pop());
}

TEST(CircularBufferTest, MoveAssignmentShouldTransferOwnership) {
    CircularBuffer<Item, 4> queue1;
    CircularBuffer<Item, 4> queue2;

    Item a(10), b(20);
    queue1.push(&a);
    queue1.push(&b);

    queue2 = static_cast<CircularBuffer<Item,4>&&>(queue1);

    ASSERT_NE(queue2.front(), nullptr);
    EXPECT_EQ(queue2.front()->value, 10);
    ASSERT_NE(queue2.back(), nullptr);
    EXPECT_EQ(queue2.back()->value, 20);

    EXPECT_EQ(queue1.front(), nullptr);
    EXPECT_EQ(queue1.back(), nullptr);
    EXPECT_FALSE(queue1.pop());
}

TEST(CircularBufferTest, DestructorShouldCleanAllElements) {
    auto* queue = new CircularBuffer<Item, 4>();

    Item a(1), b(2), c(3);
    queue->push(&a);
    queue->push(&b);
    queue->push(&c);

    delete queue;

    SUCCEED();
}

TEST(CircularBufferTest, DefaultConstructorShouldInitializeEmpty) {
    CircularBuffer<Item, 4> queue;

    EXPECT_EQ(queue.front(), nullptr);
    EXPECT_EQ(queue.back(), nullptr);
    EXPECT_FALSE(queue.pop());
}

// --- Leak detector integration ---

TEST(CircularBufferTest, LeakDetectionsNoLeakWhenLeavingElements) {
    LeakDetectors::HeapSystem().reset();
    const auto pre = LeakDetectors::HeapSystem().in_use();
    {
        CircularBuffer<Item, 4> queue;
        Item a(1), b(2), c(3);
        queue.push(&a);
        queue.push(&b);
        queue.push(&c);
    }
    const auto post = LeakDetectors::HeapSystem().in_use();
    EXPECT_EQ(pre, post);
}

TEST(CircularBufferTest, LeakDetectionsNoLeakWhenEmpty) {
    LeakDetectors::HeapSystem().reset();
    const auto pre = LeakDetectors::HeapSystem().in_use();
    {
        CircularBuffer<Item, 4> queue;
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

TEST(CircularBufferTest, OverflowShouldRemoveOldest) {
    CircularBuffer<Item, 3> queue;

    Item a(1), b(2), c(3), d(4);

    // Llenamos el buffer
    EXPECT_TRUE(queue.push(&a));
    EXPECT_TRUE(queue.push(&b));
    EXPECT_TRUE(queue.push(&c));

    ASSERT_EQ(queue.size(), 3);
    EXPECT_EQ(queue.front()->value, 1);
    EXPECT_EQ(queue.back()->value, 3);

    // Push sobre buffer lleno: debería eliminar 'a' y agregar 'd'
    EXPECT_TRUE(queue.push(&d));

    ASSERT_EQ(queue.size(), 3);       // sigue siendo tamaño máximo
    EXPECT_EQ(queue.front()->value, 2); // 'a' se perdió
    EXPECT_EQ(queue.back()->value, 4);  // 'd' al final

    // Pop sucesivos para verificar orden FIFO restante
    EXPECT_TRUE(queue.pop());
    EXPECT_EQ(queue.front()->value, 3);
    EXPECT_TRUE(queue.pop());
    EXPECT_EQ(queue.front()->value, 4);
    EXPECT_TRUE(queue.pop());
    EXPECT_EQ(queue.front(), nullptr); // ya vacío
}
