#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "AggregateCommunicationService.h"
#include "MockCommunicationService.h"
#include <vector>
using namespace testing;
using namespace EmbeddedIOServices;

namespace UnitTests
{
	TEST(AggregateCommunicationServiceTests, SendsDataToAllServices)
	{
		MockCommunicationService service1;
		MockCommunicationService service2;
		ICommunicationService *services[2] = { &service1, &service2 };
		AggregateCommunicationService aggregate(services, 2);
		const uint8_t data[3] = { 1, 2, 3 };

		EXPECT_CALL(service1, Send(data, sizeof(data))).Times(1);
		EXPECT_CALL(service2, Send(data, sizeof(data))).Times(1);

		aggregate.Send(data, sizeof(data));
	}

	TEST(AggregateCommunicationServiceTests, CallsAttachedSendCallbacksWhenSending)
	{
		MockCommunicationService service;
		ICommunicationService *services[1] = { &service };
		AggregateCommunicationService aggregate(services, 1);
		const uint8_t data[3] = { 1, 2, 3 };
		std::vector<uint8_t> observed;

		aggregate.RegisterSendCallBack([&observed](const void *data, size_t length) {
			const uint8_t * const bytes = reinterpret_cast<const uint8_t *>(data);
			observed.assign(bytes, bytes + length);
		});

		EXPECT_CALL(service, Send(data, sizeof(data))).Times(1);

		aggregate.Send(data, sizeof(data));

		ASSERT_EQ(sizeof(data), observed.size());
		EXPECT_EQ(1, observed[0]);
		EXPECT_EQ(2, observed[1]);
		EXPECT_EQ(3, observed[2]);
	}

	TEST(AggregateCommunicationServiceTests, UnRegistersAttachedSendCallbacks)
	{
		MockCommunicationService service;
		AggregateCommunicationService aggregate;
		aggregate.AddService(&service);
		const uint8_t data[1] = { 1 };
		int sendCallBackCount = 0;

		const auto id = aggregate.RegisterSendCallBack([&sendCallBackCount](const void *data, size_t length) {
			sendCallBackCount++;
		});
		aggregate.UnRegisterSendCallBack(id);

		EXPECT_CALL(service, Send(data, sizeof(data))).Times(1);

		aggregate.Send(data, sizeof(data));

		EXPECT_EQ(0, sendCallBackCount);
	}

	TEST(AggregateCommunicationServiceTests, ReceivesDataFromChildServices)
	{
		MockCommunicationService service;
		AggregateCommunicationService aggregate;
		aggregate.AddService(&service);
		const uint8_t data[3] = { 4, 5, 6 };
		std::vector<uint8_t> received;

		aggregate.RegisterReceiveCallBack([&received](communication_send_callback_t send, const void *data, size_t length) {
			const uint8_t * const bytes = reinterpret_cast<const uint8_t *>(data);
			received.assign(bytes, bytes + length);
			return length;
		});

		EXPECT_EQ(0u, service.Receive(data, sizeof(data)));

		ASSERT_EQ(sizeof(data), received.size());
		EXPECT_EQ(4, received[0]);
		EXPECT_EQ(5, received[1]);
		EXPECT_EQ(6, received[2]);
	}

	TEST(AggregateCommunicationServiceTests, DestructorUnRegistersChildReceiveCallbacks)
	{
		MockCommunicationService service;
		const uint8_t data[1] = { 1 };
		int receiveCallBackCount = 0;

		{
			AggregateCommunicationService aggregate;
			aggregate.AddService(&service);
			aggregate.RegisterReceiveCallBack([&receiveCallBackCount](communication_send_callback_t send, const void *data, size_t length) {
				receiveCallBackCount++;
				return length;
			});

			EXPECT_EQ(0u, service.Receive(data, sizeof(data)));
		}

		EXPECT_EQ(0u, service.Receive(data, sizeof(data)));
		EXPECT_EQ(1, receiveCallBackCount);
	}
}
