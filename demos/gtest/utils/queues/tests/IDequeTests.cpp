#include <gtest/gtest.h>
#include "LeakDetector/LeakDetector.h"
#include "Queue/DequeFixed.h"
#include "Queue/DequeLinked.h"

template <typename TDeque>
class DequeInterfaceTest : public ::testing::Test {};

using DequeImplementations = ::testing::Types<
    DequeFixed<int, 8>,
    DequeLinked<int>
>;

TYPED_TEST_SUITE(DequeInterfaceTest, DequeImplementations);

TYPED_TEST(DequeInterfaceTest, PushBackStoresPointersCorrectly) {
    TypeParam deque;

    int a = 10;
    int b = 20;

    EXPECT_TRUE(deque.push_back(&a));
    EXPECT_TRUE(deque.push_back(&b));

    EXPECT_EQ(deque.size(), 2u);
    EXPECT_EQ(deque.back(), &b);
    EXPECT_EQ(deque.front(), &a);
}

TYPED_TEST(DequeInterfaceTest, PushFrontPrependsCorrectly) {
    TypeParam deque;

    int a = 1;
    int b = 2;

    EXPECT_TRUE(deque.push_front(&a));
    EXPECT_TRUE(deque.push_front(&b));

    EXPECT_EQ(deque.size(), 2u);
    EXPECT_EQ(deque.front(), &b);
    EXPECT_EQ(deque.back(), &a);
}

TYPED_TEST(DequeInterfaceTest, PopFrontAndPopBackWorkCorrectly) {
    TypeParam deque;
    int a = 5, b = 6, c = 7;

    deque.push_back(&a);
    deque.push_back(&b);
    deque.push_back(&c);

    EXPECT_EQ(deque.size(), 3u);

    EXPECT_TRUE(deque.pop_front()); // remove 'a'
    EXPECT_EQ(deque.front(), &b);

    EXPECT_TRUE(deque.pop_back());  // remove 'c'
    EXPECT_EQ(deque.back(), &b);

    EXPECT_EQ(deque.size(), 1u);
}

TYPED_TEST(DequeInterfaceTest, ClearEmptiesDeque) {
    TypeParam deque;
    int a = 42;

    deque.push_back(&a);
    EXPECT_FALSE(deque.empty());

    deque.clear();
    EXPECT_TRUE(deque.empty());
    EXPECT_EQ(deque.size(), 0u);
}

template <typename TVoidDeque>
class DequeVoidInterfaceTest : public ::testing::Test {};

using DequeVoidImplementations = ::testing::Types<
    DequeFixed<void, 8>,
    DequeLinked<void>
>;

TYPED_TEST_SUITE(DequeVoidInterfaceTest, DequeVoidImplementations);

TYPED_TEST(DequeVoidInterfaceTest, PushIncreasesSize) {
    TypeParam deque;

    EXPECT_TRUE(deque.push_back());
    EXPECT_EQ(deque.size(), 1u);

    EXPECT_TRUE(deque.push_front());
    EXPECT_EQ(deque.size(), 2u);
}

TYPED_TEST(DequeVoidInterfaceTest, PopDecreasesSize) {
    TypeParam deque;

    deque.push_back();
    deque.push_back();
    EXPECT_EQ(deque.size(), 2u);

    EXPECT_TRUE(deque.pop_front());
    EXPECT_EQ(deque.size(), 1u);

    EXPECT_TRUE(deque.pop_back());
    EXPECT_EQ(deque.size(), 0u);
    EXPECT_TRUE(deque.empty());
}

TYPED_TEST(DequeVoidInterfaceTest, ClearEmptiesDeque) {
    TypeParam deque;

    for (int i = 0; i < 5; ++i)
        EXPECT_TRUE(deque.push_back());

    EXPECT_EQ(deque.size(), 5u);

    deque.clear();

    EXPECT_TRUE(deque.empty());
    EXPECT_EQ(deque.size(), 0u);
}

