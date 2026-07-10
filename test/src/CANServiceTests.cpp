#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "ICommunicationService.h"
#include "MockCANService.h"
#include <vector>
using namespace testing;
using namespace EmbeddedIOServices;

namespace UnitTests
{
	class CANServiceTests : public ::testing::Test 
	{
		protected:
		MockCANService _canService;
		int handler1count = 0;
		int handler2count = 0;
		int handler3count = 0;
		int handler4count = 0;

		CANServiceTests()
		{
			_canService.RegisterReceiveCallBack({0x1, 0}, [this](can_send_callback_t send, const CANData_t data, const uint8_t dataLength) { this->handler1count++; });
			_canService.RegisterReceiveCallBack({0x2, 0}, [this](can_send_callback_t send, const CANData_t data, const uint8_t dataLength) { this->handler2count++; });
			_canService.RegisterReceiveCallBack({0x2, 0}, [this](can_send_callback_t send, const CANData_t data, const uint8_t dataLength) { this->handler3count++; });
			_canService.RegisterReceiveCallBack({0x1, 0}, {0x7F1, 0x7}, [this](can_send_callback_t send, const CANIdentifier_t identifier, const CANData_t data, const uint8_t dataLength) { this->handler4count++; });
		}
	};

	TEST_F(CANServiceTests, CanRegisterAndUnRegisterReceiveCallBack)
	{
		auto id1 = _canService.RegisterReceiveCallBack({0, 0x1}, [](can_send_callback_t send, const CANData_t data, const uint8_t dataLength) {  });
		_canService.RegisterReceiveCallBack({0, 0x2}, [](can_send_callback_t send, const CANData_t data, const uint8_t dataLength) {  });
		auto id3 = _canService.RegisterReceiveCallBack({0, 0x1}, {0, 0x1}, [](can_send_callback_t send, const CANIdentifier_t identifier, const CANData_t data, const uint8_t dataLength) {  });
		_canService.RegisterReceiveCallBack({0, 0x2}, {0, 0x2}, [](can_send_callback_t send, const CANIdentifier_t identifier, const CANData_t data, const uint8_t dataLength) {  });

		_canService.UnRegisterReceiveCallBack(id1);
		_canService.UnRegisterReceiveCallBack({0, 0x2});
		_canService.UnRegisterReceiveCallBack(id3);
		_canService.UnRegisterReceiveCallBack({0, 0x2}, {0, 0x2});
	}

	TEST_F(CANServiceTests, WhenFirstIdentifierReceivedOnlyFirstAndMaskedIdentifierCalled)
	{
		handler1count = 0;
		handler2count = 0;
		handler3count = 0;
		handler4count = 0;

		const CANData_t data = { 0, 1, 2, 3, 4, 5, 6, 7 };
		_canService.Receive({0x1, 0}, data, 8);

		ASSERT_EQ(1, handler1count) << "handler 1 not called";
		ASSERT_EQ(0, handler2count) << "handler 2 called";
		ASSERT_EQ(0, handler2count) << "handler 3 called";
		ASSERT_EQ(1, handler4count) << "handler 4 not called";
	}

	TEST_F(CANServiceTests, WhenThirdIdentifierReceivedOnlyMaskedIdentifierCalled)
	{
		handler1count = 0;
		handler2count = 0;
		handler3count = 0;
		handler4count = 0;

		const CANData_t data = { 0, 1, 2, 3, 4, 5, 6, 7 };
		_canService.Receive({0x3, 0}, data, 8);

		ASSERT_EQ(0, handler1count) << "handler 1 called";
		ASSERT_EQ(0, handler2count) << "handler 2 called";
		ASSERT_EQ(0, handler2count) << "handler 3 called";
		ASSERT_EQ(1, handler4count) << "handler 4 not called";
	}

	TEST_F(CANServiceTests, WhenSecondIdentifierReceivedOnlySecondIdentifiersCalled)
	{
		handler1count = 0;
		handler2count = 0;
		handler3count = 0;
		handler4count = 0;

		const CANData_t data = { 0, 1, 2, 3, 4, 5, 6, 7 };
		_canService.Receive({0x2, 0}, data, 8);

		ASSERT_EQ(0, handler1count) << "handler 1 called";
		ASSERT_EQ(1, handler2count) << "handler 2 not called";
		ASSERT_EQ(1, handler3count) << "handler 3 not called";
		ASSERT_EQ(0, handler4count) << "handler 4 called";
	}

	TEST_F(CANServiceTests, GetISOTPServiceReturnsSameServiceForSameListenAndTransmitIds)
	{
		const CANIdentifier_t listenIds[2] = { { 0x700, 0 }, { 0x701, 0 } };
		const CANIdentifier_t transmitIds[2] = { { 0x708, 0 }, { 0x709, 0 } };

		ICommunicationService * const service1 = _canService.GetISOTPService(listenIds, 2, transmitIds, 2);
		ICommunicationService * const service2 = _canService.GetISOTPService(listenIds, 2, transmitIds, 2);

		EXPECT_EQ(service1, service2);
	}

	TEST_F(CANServiceTests, GetISOTPServiceSendsUsingTransmitIds)
	{
		const CANIdentifier_t listenIds[1] = { { 0x700, 0 } };
		const CANIdentifier_t transmitIds[2] = { { 0x708, 0 }, { 0x709, 0 } };
		ICommunicationService * const service = _canService.GetISOTPService(listenIds, 1, transmitIds, 2);
		const uint8_t payload[3] = { 0x22, 0xF1, 0x90 };

		EXPECT_CALL(_canService, Send(transmitIds[0], _, 4))
			.WillOnce([](const CANIdentifier_t identifier, const CANData_t data, const uint8_t dataLength) {
				EXPECT_EQ(0x03, data.Data[0]);
				EXPECT_EQ(0x22, data.Data[1]);
				EXPECT_EQ(0xF1, data.Data[2]);
				EXPECT_EQ(0x90, data.Data[3]);
			});
		EXPECT_CALL(_canService, Send(transmitIds[1], _, 4))
			.WillOnce([](const CANIdentifier_t identifier, const CANData_t data, const uint8_t dataLength) {
				EXPECT_EQ(0x03, data.Data[0]);
				EXPECT_EQ(0x22, data.Data[1]);
				EXPECT_EQ(0xF1, data.Data[2]);
				EXPECT_EQ(0x90, data.Data[3]);
			});

		service->Send(payload, sizeof(payload));
	}

	TEST_F(CANServiceTests, GetISOTPServiceReceivesFromEveryListenId)
	{
		const CANIdentifier_t listenIds[2] = { { 0x700, 0 }, { 0x701, 0 } };
		const CANIdentifier_t transmitIds[1] = { { 0x708, 0 } };
		ICommunicationService * const service = _canService.GetISOTPService(listenIds, 2, transmitIds, 1);
		std::vector<uint8_t> received;

		service->RegisterReceiveCallBack([&received](communication_send_callback_t send, const void *data, size_t length) {
			const uint8_t * const bytes = reinterpret_cast<const uint8_t *>(data);
			received.insert(received.end(), bytes, bytes + length);
			return length;
		});

		const CANData_t frame1 = { 0x01, 0xAA, 0, 0, 0, 0, 0, 0 };
		const CANData_t frame2 = { 0x01, 0xBB, 0, 0, 0, 0, 0, 0 };
		_canService.Receive(listenIds[0], frame1, 2);
		_canService.Receive(listenIds[1], frame2, 2);

		ASSERT_EQ(2u, received.size());
		EXPECT_EQ(0xAA, received[0]);
		EXPECT_EQ(0xBB, received[1]);
	}

	TEST_F(CANServiceTests, GetISOTPServiceReusesExistingISOTPServiceForSameListenId)
	{
		const CANIdentifier_t listenIds[1] = { { 0x700, 0 } };
		const CANIdentifier_t transmitIds1[1] = { { 0x708, 0 } };
		const CANIdentifier_t transmitIds2[1] = { { 0x709, 0 } };
		ICommunicationService * const service1 = _canService.GetISOTPService(listenIds, 1, transmitIds1, 1);
		ICommunicationService * const service2 = _canService.GetISOTPService(listenIds, 1, transmitIds2, 1);
		int service1ReceiveCount = 0;
		int service2ReceiveCount = 0;

		service1->RegisterReceiveCallBack([&service1ReceiveCount](communication_send_callback_t send, const void *data, size_t length) {
			service1ReceiveCount++;
			return length;
		});
		service2->RegisterReceiveCallBack([&service2ReceiveCount](communication_send_callback_t send, const void *data, size_t length) {
			service2ReceiveCount++;
			return length;
		});

		const CANData_t frame = { 0x01, 0xAA, 0, 0, 0, 0, 0, 0 };
		_canService.Receive(listenIds[0], frame, 2);

		EXPECT_NE(service1, service2);
		EXPECT_EQ(1, service1ReceiveCount);
		EXPECT_EQ(1, service2ReceiveCount);
	}
}
