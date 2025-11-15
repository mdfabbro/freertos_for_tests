#include <gtest/gtest.h>
#include "Queue/Vector.h"   // tu implementación

struct Item {
    int value;
    Item(int v = 0) : value(v) {}
};

TEST(VectorTest, PushBackAndAccess) {
    Vector<Item, 5> vec;

    EXPECT_EQ(vec.size(), 0);

    vec.push_back(Item(10));
    vec.push_back(Item(20));

    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0]->value, 10);
    EXPECT_EQ(vec[1]->value, 20);
}

TEST(VectorTest, FrontAndBack) {
    Vector<Item, 5> vec;

    vec.push_back(Item(5));
    vec.push_back(Item(15));
    vec.push_back(Item(25));

    EXPECT_EQ(vec.front()->value, 5);
    EXPECT_EQ(vec.back()->value, 25);
}

TEST(VectorTest, ModifyElements) {
    Vector<Item, 5> vec;

    vec.push_back(Item(1));
    vec.push_back(Item(2));

    vec[0]->value = 100;
    vec[1]->value = 200;

    EXPECT_EQ(vec[0]->value, 100);
    EXPECT_EQ(vec[1]->value, 200);
}

TEST(VectorTest, CapacityLimit) {
    Vector<Item, 5> vec;

    for (int i = 0; i < 5; i++) {
        vec.push_back(Item(i));
    }

    EXPECT_EQ(vec.size(), 5);

    // Dependiendo de tu implementación:
    // Si debe lanzar excepción:
    // EXPECT_THROW(vec.push_back(Item(99)), std::out_of_range);

    // Si no debería permitirlo y simplemente ignorar:
    // EXPECT_EQ(vec.size(), 5);
}

TEST(VectorTest, Iteration) {
    Vector<Item, 5> vec;

    vec.push_back(Item(1));
    vec.push_back(Item(2));
    vec.push_back(Item(3));

    int sum = 0;
    for (size_t i = 0; i < vec.size(); i++) {
        sum += vec[i]->value;
    }

    EXPECT_EQ(sum, 6);
}

TEST(VectorTest, PopBackReducesSize) {
    Vector<Item, 5> vec;

    vec.push_back(Item(10));
    vec.push_back(Item(20));
    vec.push_back(Item(30));

    EXPECT_EQ(vec.size(), 3);

    vec.pop_back();
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec.back()->value, 20);
}

TEST(VectorTest, ClearEmptiesVector) {
    Vector<Item, 5> vec;

    vec.push_back(Item(1));
    vec.push_back(Item(2));
    vec.push_back(Item(3));

    vec.clear();

    EXPECT_EQ(vec.size(), 0);
}

TEST(VectorTest, OverwriteElements) {
    Vector<Item, 5> vec;

    vec.push_back(Item(5));
    vec.push_back(Item(10));

    *vec[0] = Item(50);
    *vec[1] = Item(100);

    EXPECT_EQ(vec[0]->value, 50);
    EXPECT_EQ(vec[1]->value, 100);
}

TEST(VectorTest, FillMaxAndModify) {
    Vector<Item, 5> vec;

    for (size_t i = 0; i < 5; i++)
        vec.push_back(Item(i));

    EXPECT_EQ(vec.size(), 5);

    vec[4]->value = 999;
    EXPECT_EQ(vec[4]->value, 999);
}

TEST(VectorTest, AssignAfterCreation) {
    Vector<Item, 5> vec;

    vec.push_back(Item());
    vec[0]->value = 777;

    EXPECT_EQ(vec[0]->value, 777);
}

TEST(VectorTest, MixedOperations) {
    Vector<Item, 5> vec;

    vec.push_back(Item(1));
    vec.push_back(Item(2));
    vec.push_back(Item(3));

    vec.pop_back();            // remove 3
    vec.push_back(Item(4));    // add 4

    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0]->value, 1);
    EXPECT_EQ(vec[1]->value, 2);
    EXPECT_EQ(vec[2]->value, 4);
}

TEST(VectorTest, EraseAllElements) {
    Vector<Item, 5> vec;

    vec.push_back(Item(1));
    vec.push_back(Item(2));
    vec.push_back(Item(3));

    ASSERT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0]->value, 1);
    EXPECT_EQ(vec[1]->value, 2);
    EXPECT_EQ(vec[2]->value, 3);

    vec.erase(0);

    ASSERT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0]->value, 2);
    EXPECT_EQ(vec[1]->value, 3);

    vec.erase(0);

    ASSERT_EQ(vec.size(), 1);
    EXPECT_EQ(vec[0]->value, 3);

    vec.erase(0);

    ASSERT_EQ(vec.size(), 0);
}

TEST(VectorTest, InsertVariousPositions) {
    Vector<Item, 5> vec;

    // Insertar en posición 0 (vector vacío)
    Item a(10);
    vec.insert(0, a);

    ASSERT_EQ(vec.size(), 1u);
    EXPECT_EQ(vec[0]->value, 10);

    // Insertar al final
    Item b(20);
    vec.insert(1, b);

    ASSERT_EQ(vec.size(), 2u);
    EXPECT_EQ(vec[0]->value, 10);
    EXPECT_EQ(vec[1]->value, 20);

    // Insertar al medio
    Item c(30);
    vec.insert(1, c); // Entre 10 y 20

    ASSERT_EQ(vec.size(), 3u);
    EXPECT_EQ(vec[0]->value, 10);
    EXPECT_EQ(vec[1]->value, 30);  // nuevo elemento en el medio
    EXPECT_EQ(vec[2]->value, 20);

    // Insertar al principio nuevamente
    Item d(40);
    vec.insert(0, d);

    ASSERT_EQ(vec.size(), 4u);
    EXPECT_EQ(vec[0]->value, 40);
    EXPECT_EQ(vec[1]->value, 10);
    EXPECT_EQ(vec[2]->value, 30);
    EXPECT_EQ(vec[3]->value, 20);

    // Insertar último para llenar el vector
    Item e(50);
    vec.insert(4, e); // al final

    ASSERT_EQ(vec.size(), 5u);
    EXPECT_EQ(vec[0]->value, 40);
    EXPECT_EQ(vec[1]->value, 10);
    EXPECT_EQ(vec[2]->value, 30);
    EXPECT_EQ(vec[3]->value, 20);
    EXPECT_EQ(vec[4]->value, 50);
}
