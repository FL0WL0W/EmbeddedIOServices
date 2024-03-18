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

		CANServiceTests()
		{
			_canService.RegisterReceiveCallBack(0x1, [this](can_send_callback_t send, const CANData_t data) { this->handler1count++; });
			_canService.RegisterReceiveCallBack(0x2, [this](can_send_callback_t send, const CANData_t data) { this->handler2count++; });
			_canService.RegisterReceiveCallBack(0x2, [this](can_send_callback_t send, const CANData_t data) { this->handler3count++; });
		}
	};

	TEST_F(CANServiceTests, CanRegisterAndUnRegisterReceiveCallBack)
	{
		can_receive_callback_map_t::iterator iterator = _canService.RegisterReceiveCallBack(0x1, [](can_send_callback_t send, const CANData_t data) {  });
		_canService.UnRegisterReceiveCallBack(iterator);
	}

	TEST_F(CANServiceTests, WhenFirstIdentifierReceivedOnlyFirstIdentifierCalled)
	{
		handler1count = 0;
		handler2count = 0;
		handler3count = 0;

		const CANData_t data = { 0, 1, 2, 3, 4, 5, 6, 7 };
		_canService.Receive(0x1, data);

		ASSERT_EQ(1, handler1count) << "handler 1 not called";
		ASSERT_EQ(0, handler2count) << "handler 2 called";
	}

	TEST_F(CANServiceTests, WhenSecondIdentifierReceivedSecondAndThirdIdentifierCalled)
	{
		handler1count = 0;
		handler2count = 0;
		handler3count = 0;

		const CANData_t data = { 0, 1, 2, 3, 4, 5, 6, 7 };
		_canService.Receive(0x2, data);

		ASSERT_EQ(0, handler1count) << "handler 1 called";
		ASSERT_EQ(1, handler2count) << "handler 2 not called";
		ASSERT_EQ(1, handler3count) << "handler 3 not called";
	}
}
