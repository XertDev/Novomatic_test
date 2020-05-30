#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "CircularBuffer.hpp"

TEST(CircularBuffer, default_constructor_info)
{
	CircularBuffer<int> buffer;

	ASSERT_EQ(0, buffer.size());
	ASSERT_TRUE(buffer.empty());
}

TEST(CircularBuffer, constructor_capacity_validation)
{
	ASSERT_THROW(CircularBuffer<int> a(0), std::length_error);
}

TEST(CircularBuffer, capacity_match)
{
	CircularBuffer<int> buff1(100);
	CircularBuffer<int> buff2(121);

	ASSERT_EQ(100, buff1.capacity());
	ASSERT_EQ(121, buff2.capacity());
}

TEST(CircularBuffer, push_back)
{
	CircularBuffer<int> buffer;
	buffer.push_back(2);

	ASSERT_EQ(2, buffer.front());
	ASSERT_EQ(2, buffer.back());
	ASSERT_EQ(1, buffer.size());
	ASSERT_FALSE(buffer.empty());
}

TEST(CircularBuffer, many_push_backs)
{
	CircularBuffer<int> buffer;
	buffer.push_back(2);
	buffer.push_back(3);

	ASSERT_EQ(2, buffer.front());
	ASSERT_EQ(3, buffer.back());
	ASSERT_EQ(2, buffer.size());
}

TEST(CircularBuffer, clear_buffer)
{
	CircularBuffer<int> buffer;
	buffer.push_back(5);

	buffer.clear();

	ASSERT_EQ(0, buffer.size());
	ASSERT_TRUE(buffer.empty());
}

TEST(CircularBuffer, pop_front)
{
	CircularBuffer<int> buffer;
	buffer.push_back(3);
	buffer.push_back(1);
	buffer.pop_front();

	ASSERT_EQ(1, buffer.size());
	ASSERT_EQ(1, buffer.front());
}

TEST(CircularBuffer, push_back_overwrite_front)
{
	CircularBuffer<int> buffer(2);
	buffer.push_back(3);
	buffer.push_back(1);
	buffer.push_back(2);

	ASSERT_EQ(2, buffer.size());
	ASSERT_EQ(1, buffer.front());
	ASSERT_EQ(2, buffer.back());
}

TEST(CircularBuffer, swap)
{
	CircularBuffer<int> buffer1(2);
	CircularBuffer<int> buffer2(3);
	buffer1.push_back(1);
	buffer1.push_back(2);
	buffer2.push_back(3);

	std::swap(buffer1, buffer2);

	ASSERT_EQ(1, buffer1.size());
	ASSERT_EQ(2, buffer2.size());

	ASSERT_EQ(1, buffer2.front());
	ASSERT_EQ(2, buffer2.back());
	ASSERT_EQ(3, buffer1.front());

	ASSERT_EQ(2, buffer2.capacity());
	ASSERT_EQ(3, buffer1.capacity());
}

TEST(CircularBuffer, index_operator)
{
	CircularBuffer<int> buffer;
	buffer.push_back(3);
	buffer.push_back(1);

	ASSERT_EQ(3, buffer[0]);
	ASSERT_EQ(1, buffer[1]);
}

TEST(CircularBuffer, at)
{
	CircularBuffer<int> buffer;
	buffer.push_back(3);
	buffer.push_back(1);

	ASSERT_EQ(3, buffer.at(0));
	ASSERT_EQ(1, buffer.at(1));
	ASSERT_THROW(buffer.at(2), std::out_of_range);
}