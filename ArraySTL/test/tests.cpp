#include "NDArray.h"

#include <gtest/gtest.h>

#include <stdexcept>
#include <vector>

TEST(NDArrayTest, Construct1DFromInitializerList)
{
	NDArray< int, 1 > arr({ 1, 2, 3, 4 });
	EXPECT_EQ(arr.dim(), 1);
	EXPECT_EQ(arr.total_count(), 4);
	EXPECT_EQ(arr.at({ 0 }), 1);
	EXPECT_EQ(arr.at({ 1 }), 2);
	EXPECT_EQ(arr.at({ 2 }), 3);
	EXPECT_EQ(arr.at({ 3 }), 4);
}

TEST(NDArrayTest, Construct1DFromIterators)
{
	std::vector< int > vec = { 10, 20, 30 };
	NDArray< int, 1 > arr(vec.begin(), vec.end());
	EXPECT_EQ(arr.dim(), 1);
	EXPECT_EQ(arr.total_count(), 3);
	EXPECT_EQ(arr.at({ 0 }), 10);
	EXPECT_EQ(arr.at({ 1 }), 20);
	EXPECT_EQ(arr.at({ 2 }), 30);
}

TEST(NDArrayTest, CopyConstructor1D)
{
	NDArray< int, 1 > arr({ 1, 2, 3 });
	NDArray< int, 1 > copy(arr);
	EXPECT_EQ(copy.total_count(), arr.total_count());
	EXPECT_EQ(copy.at({ 0 }), 1);
	EXPECT_EQ(copy.at({ 1 }), 2);
	EXPECT_EQ(copy.at({ 2 }), 3);
	copy.at({ 0 }) = 100;
	EXPECT_EQ(arr.at({ 0 }), 1);
	EXPECT_EQ(copy.at({ 0 }), 100);
}

TEST(NDArrayTest, MoveConstructor1D)
{
	NDArray< int, 1 > arr({ 1, 2, 3 });
	NDArray< int, 1 > moved(std::move(arr));
	EXPECT_EQ(moved.total_count(), 3);
	EXPECT_EQ(moved.at({ 0 }), 1);
	EXPECT_EQ(moved.at({ 1 }), 2);
	EXPECT_EQ(moved.at({ 2 }), 3);
	EXPECT_EQ(arr.total_count(), 0);
}

TEST(NDArrayTest, AtAccess1D)
{
	NDArray< int, 1 > arr({ 1, 2, 3 });
	EXPECT_EQ(arr.at({ 0 }), 1);
	EXPECT_EQ(arr.at({ 1 }), 2);
	EXPECT_EQ(arr.at({ 2 }), 3);
	EXPECT_THROW(arr.at({ 3 }), std::out_of_range);
	EXPECT_THROW(arr.at({ 0, 0 }), std::out_of_range);
}

TEST(NDArrayTest, Construct2DFromInitializerList)
{
	NDArray< int, 2 > arr({ { 1, 2, 3 }, { 4, 5, 6 } });
	EXPECT_EQ(arr.dim(), 2);
	EXPECT_EQ(arr.count(), 2);
	EXPECT_EQ(arr.total_count(), 6);
	EXPECT_EQ(arr.at({ 0, 0 }), 1);
	EXPECT_EQ(arr.at({ 0, 1 }), 2);
	EXPECT_EQ(arr.at({ 0, 2 }), 3);
	EXPECT_EQ(arr.at({ 1, 0 }), 4);
	EXPECT_EQ(arr.at({ 1, 1 }), 5);
	EXPECT_EQ(arr.at({ 1, 2 }), 6);
	EXPECT_THROW((NDArray< int, 2 >({ { 1, 2 }, { 3, 4, 5 } })), std::invalid_argument);
}

TEST(NDArrayTest, ViewFromArray1D)
{
	NDArray< int, 1 > arr({ 1, 2, 3 });
	NDArrayView< int, 1 > view(arr.data_, arr.dimensions_, arr.strides_);
	EXPECT_EQ(view.total_count(), 3);
	EXPECT_EQ(view.at({ 0 }), 1);
	view.at({ 0 }) = 100;
	EXPECT_EQ(arr.at({ 0 }), 100);
}

TEST(NDArrayTest, ViewFromArray2D)
{
	NDArray< int, 2 > arr({ { 1, 2, 3 }, { 4, 5, 6 } });
	NDArrayView< int, 2 > view(arr.data_, arr.dimensions_, arr.strides_);
	EXPECT_EQ(view.total_count(), 6);
	EXPECT_EQ(view.at({ 0, 1 }), 2);
	view.at({ 1, 2 }) = 42;
	EXPECT_EQ(arr.at({ 1, 2 }), 42);
}

TEST(NDArrayTest, ConstViewFromArray1D)
{
	NDArray< int, 1 > arr({ 1, 2, 3 });
	NDArrayConstView< int, 1 > view(arr.data_, arr.dimensions_, arr.strides_);
	EXPECT_EQ(view.total_count(), 3);
	EXPECT_EQ(view.at({ 0 }), 1);
	EXPECT_EQ(view.at({ 1 }), 2);
}

TEST(NDArrayTest, ConstViewFromView)
{
	NDArray< int, 2 > arr({ { 1, 2, 3 }, { 4, 5, 6 } });
	NDArrayView< int, 2 > view(arr.data_, arr.dimensions_, arr.strides_);
	NDArrayConstView< int, 2 > const_view(view);
	EXPECT_EQ(const_view.total_count(), 6);
	EXPECT_EQ(const_view.at({ 0, 1 }), 2);
}

TEST(NDArrayTest, Reshape2DTo1D)
{
	NDArray< int, 2 > arr({ { 1, 2, 3 }, { 4, 5, 6 } });
	std::vector< size_t > new_dims = { 6 };
	auto view = arr.reshape< 1 >(new_dims);
	EXPECT_EQ(view.dim(), 1);
	EXPECT_EQ(view.total_count(), 6);
	EXPECT_EQ(view.at({ 0 }), 1);
	EXPECT_EQ(view.at({ 5 }), 6);
	view.at({ 0 }) = 100;
	EXPECT_EQ(arr.at({ 0, 0 }), 100);
}

TEST(NDArrayTest, ReshapeInvalid)
{
	NDArray< int, 2 > arr({ { 1, 2, 3 }, { 4, 5, 6 } });
	std::vector< size_t > new_dims = { 7 };
	EXPECT_THROW(arr.reshape< 1 >(new_dims), std::invalid_argument);
}

TEST(NDArrayTest, Swap1D)
{
	NDArray< int, 1 > arr1({ 1, 2, 3 });
	NDArray< int, 1 > arr2({ 4, 5, 6 });
	arr1.swap(arr2);
	EXPECT_EQ(arr1.at({ 0 }), 4);
	EXPECT_EQ(arr2.at({ 0 }), 1);

	NDArray< int, 1 > arr3({ 1, 2 });
	EXPECT_THROW(arr1.swap(arr3), std::invalid_argument);
}

TEST(NDArrayTest, IsEqual)
{
	NDArray< int, 1 > arr1({ 1, 2, 3 });
	NDArray< int, 1 > arr2({ 1, 2, 3 });
	NDArray< int, 1 > arr3({ 1, 2, 4 });
	EXPECT_TRUE(arr1.is_equal(arr2));
	EXPECT_FALSE(arr1.is_equal(arr3));
}

TEST(NDArrayTest, Iterator1D)
{
	NDArray< int, 1 > arr({ 1, 2, 3 });
	std::vector< int > result;
	for (auto it = arr.begin(); it != arr.end(); ++it)
	{
		result.push_back(*it);
	}
	EXPECT_EQ(result, std::vector< int >({ 1, 2, 3 }));
}

TEST(NDArrayTest, ConstIterator1D)
{
	NDArray< int, 1 > arr({ 1, 2, 3 });
	std::vector< int > result;
	for (auto it = arr.cbegin(); it != arr.cend(); ++it)
	{
		result.push_back(*it);
	}
	EXPECT_EQ(result, std::vector< int >({ 1, 2, 3 }));
}

TEST(NDArrayTest, IteratorView1D)
{
	NDArray< int, 1 > arr({ 1, 2, 3 });
	NDArrayView< int, 1 > view(arr.data_, arr.dimensions_, arr.strides_);
	std::vector< int > result;
	for (auto it = view.begin(); it != view.end(); ++it)
	{
		result.push_back(*it);
	}
	EXPECT_EQ(result, std::vector< int >({ 1, 2, 3 }));
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
