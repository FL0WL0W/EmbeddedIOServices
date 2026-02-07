#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "MockCANService.h"
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
}
