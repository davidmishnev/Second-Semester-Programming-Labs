#include "quat.h"

#include <gtest/gtest.h>

TEST(CtorQuatTests, CtorWithNoArgs)
{
    // Проверка конструктора без аргументов
    Quat test;
    const double *data_ = test.data();
    ASSERT_EQ(data_[0], 0);
    ASSERT_EQ(data_[1], 0);
    ASSERT_EQ(data_[2], 0);
    ASSERT_EQ(data_[3], 0);
    delete[] data_;
}

TEST(CtorQuatTests, CtorWithArgs)
{
    // Проверка что метод data возвращает компоненты кватерниона в порядке x y z w
    double x, y, z, w;
    x = 3.22;
    y = 2.39;
    z = 31.18;
    w = 1.23;
    Quat test{ w, x, y, z };
    const double *data_ = test.data();
    ASSERT_EQ(data_[0], x);
    ASSERT_EQ(data_[1], y);
    ASSERT_EQ(data_[2], z);
    ASSERT_EQ(data_[3], w);
    delete[] data_;
}

TEST(CtorQuatTests, CtorAngleAndRadian)
{
    // проверка 2х одинаковых кватернионов заданных через градусы/радианы
    double xyz[3] = { 3.22, 2.39, 31.18 };
    double angle = 30.0;
    double angle_from_radian = angle / M_PI * 180;
    Quat test{ angle_from_radian, false, xyz };
    Quat test1{ 30.0, true, xyz };
    const double *data_ = test.data();
    const double *data1_ = test1.data();
    ASSERT_NEAR(data_[0], data1_[0], 1e-6);
    ASSERT_NEAR(data_[1], data1_[1], 1e-6);
    ASSERT_NEAR(data_[2], data1_[2], 1e-6);
    ASSERT_NEAR(data_[3], data1_[3], 1e-6);
    delete[] data_;
    delete[] data1_;
}

TEST(OperatorQuatTests, AddTest0)
{
    Quat test1{ 1.0, 1.0, 1.0, 1.0 };
    Quat test2{ 1.0, 1.0, 1.0, 1.0 };
    Quat addVal = test1 + test2;
    Quat expected{ 2.0, 2.0, 2.0, 2.0 };
    ASSERT_EQ(addVal, expected);
}

TEST(OperatorQuatTests, AddTest1)
{
    Quat test1{ 1.0, 1.0, 1.0, 1.0 };
    Quat test2{ 1.0, 1.0, 1.0, 1.0 };
    test1 += test2;
    Quat expected{ 2.0, 2.0, 2.0, 2.0 };
    ASSERT_EQ(test1, expected);
}

TEST(OperatorQuatTests, AddTest2)
{
    Quat test1{ 1.0, 1.0, 1.0, 1.0 };
    Quat test2{ -1.0, -1.0, -1.0, -1.0 };
    test1 += test2;
    Quat expected{ 0, 0, 0, 0 };
    ASSERT_EQ(test1, expected);
}

TEST(OperatorQuatTests, AddTest3)
{
    Quat test1{ 0, 0, 0, 0 };
    Quat test2{ -1.0, -1.0, -1.0, -1.0 };
    test1 += test2;
    Quat expected{ -1.0, -1.0, -1.0, -1.0 };
    ASSERT_EQ(test1, expected);
}

TEST(OperatorQuatTests, AddTest4)
{
    Quat test1{ 0, 0, 0, 0 };
    Quat test2 = test1;
    test1 += test2;
    ASSERT_EQ(test1, test2);
}

TEST(OperatorQuatTests, SubTest0)
{
    Quat test1{ 0, 0, 0, 0 };
    Quat test2{ 0, 0, 0, 0 };
    Quat test3 = test1 - test2;
    ASSERT_EQ(test2, test3);
}

TEST(OperatorQuatTests, SubTest1)
{
    Quat test1{ 1.1, 2.39, 3.3, 4.4 };
    Quat test2{ -1.0, -0.25, -0, 0 };
    test2 = test1 - test2;
    Quat expected = { 1.1 - (-1.0), 2.39 - (-0.25), 3.3 - (-0), 4.4 - 0 };
    const double *data_actual = test2.data();
    const double *data_expected = expected.data();
    ASSERT_EQ(data_actual[0], data_expected[0]);
    ASSERT_EQ(data_actual[1], data_expected[1]);
    ASSERT_EQ(data_actual[2], data_expected[2]);
    ASSERT_EQ(data_actual[3], data_expected[3]);
    delete[] data_actual;
    delete[] data_expected;
}

TEST(OperatorQuatTests, MultTest0)
{
    Quat test1{ 1, 2, 3, 4 };
    Quat test2{ 5, 6, 7, 8 };
    Quat test3 = test1 * test2;
    Quat expected{ -60, 12, 30, 24 };
    const double *data_expected = expected.data();
    const double *data_test3 = test3.data();
    EXPECT_EQ(data_expected[3], data_test3[3]);
    EXPECT_EQ(data_expected[0], data_test3[0]);
    EXPECT_EQ(data_expected[1], data_test3[1]);
    EXPECT_EQ(data_expected[2], data_test3[2]);
    delete[] data_expected;
    delete[] data_test3;
}

TEST(OperatorQuatTests, MultTest1)
{
    Quat test1{ 1, 2, 3, 4 };
    Quat test2{ 5, 6, 7, 8 };
    test1 *= test2;
    Quat expected{ -60, 12, 30, 24 };
    EXPECT_EQ(expected, test1);
}

TEST(OperatorQuatTests, MultTest2)
{
    Quat test1{ 1, 2, 3, 4 };
    Quat test2{ 1, 0, 0, 0 };
    Quat expected = test1 * test2;
    EXPECT_EQ(expected, test1);
}

TEST(OperatorQuatTests, MultTest3)
{
    Quat test1{ 1, 2, 3, 4 };
    Quat test2{ 0, 0, 0, 0 };
    Quat test3 = test1 * test2;
    Quat expected{ 0, 0, 0, 0 };
    ASSERT_EQ(test3, expected);
}

//int main(int argc, char **argv)
//{
//    testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//}
