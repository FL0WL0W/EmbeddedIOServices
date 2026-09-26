#include "CircularBuffer.h"

#include "gtest/gtest.h"

#include <atomic>
#include <thread>

using EmbeddedIOServices::CircularBuffer;

TEST(CircularBuffer, UsesEverySlotAndPreservesOrderAcrossWrap)
{
	CircularBuffer<int, 3U> buffer;
	int ten = 10;
	int twenty = 20;
	int thirty = 30;
	int forty = 40;
	EXPECT_TRUE(buffer.Push(ten));
	EXPECT_TRUE(buffer.Push(twenty));
	EXPECT_TRUE(buffer.Push(thirty));
	EXPECT_FALSE(buffer.Push(forty));

	int value = 0;
	EXPECT_EQ(1U, buffer.Pop(value));
	EXPECT_EQ(10, value);
	EXPECT_TRUE(buffer.Push(forty));
	EXPECT_EQ(1U, buffer.Pop(value));
	EXPECT_EQ(20, value);
	EXPECT_EQ(1U, buffer.Pop(value));
	EXPECT_EQ(30, value);
	EXPECT_EQ(1U, buffer.Pop(value));
	EXPECT_EQ(40, value);
	EXPECT_EQ(0U, buffer.Pop(value));
}

TEST(CircularBuffer, ReturnsTheStatePublishedWithEachValue)
{
	CircularBuffer<int, 3U> buffer;
	int first = 11;
	int second = 22;
	ASSERT_TRUE(buffer.Push(first, 2U));
	ASSERT_TRUE(buffer.Push(second, 0x25U));

	int value = 0;
	EXPECT_EQ(2U, buffer.Pop(value));
	EXPECT_EQ(11, value);
	EXPECT_EQ(0x25U, buffer.Pop(value));
	EXPECT_EQ(22, value);
}

TEST(CircularBuffer, RejectsZeroStateWithoutConsumingCapacity)
{
	CircularBuffer<int, 1U> buffer;
	int rejected = 10;
	int accepted = 20;
	EXPECT_FALSE(buffer.Push(rejected, 0U));
	EXPECT_TRUE(buffer.Push(accepted, 3U));

	int value = 0;
	EXPECT_EQ(3U, buffer.Pop(value));
	EXPECT_EQ(20, value);
}

TEST(CircularBuffer, MultipleConsumersClaimEveryEntryExactlyOnce)
{
	constexpr int entryCount = 64;
	CircularBuffer<int, entryCount> buffer;
	for(int value = 0; value < entryCount; ++value)
	{
		int queuedValue = value;
		ASSERT_TRUE(buffer.Push(queuedValue));
	}

	std::atomic<unsigned int> claims[entryCount];
	for(auto& claim : claims)
		claim.store(0U, std::memory_order_relaxed);

	auto consume = [&buffer, &claims]()
	{
		int value;
		while(buffer.Pop(value) != 0U)
			claims[value].fetch_add(1U, std::memory_order_relaxed);
	};
	std::thread consumers[] = {
		std::thread(consume),
		std::thread(consume),
		std::thread(consume),
		std::thread(consume),
	};
	for(auto& consumer : consumers)
		consumer.join();

	for(const auto& claim : claims)
		EXPECT_EQ(1U, claim.load(std::memory_order_relaxed));
}
