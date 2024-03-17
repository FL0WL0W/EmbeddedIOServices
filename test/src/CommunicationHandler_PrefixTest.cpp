#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "MockCommunicationService.h"
#include "CommunicationHandler_Prefix.h"
using namespace testing;
using namespace EmbeddedIOServices;

namespace UnitTests
{
	class CommunicationHandler_PrefixTest : public ::testing::Test 
	{
		protected:
		MockCommunicationService _communicationService;
		CommunicationHandler_Prefix _communicationHandler_Prefix;
		size_t handler1return = 0;
		int handler1count = 0;
		size_t handler2return = 0;
		int handler2count = 0;
		size_t handler3return = 0;
		int handler3count = 0;

		CommunicationHandler_PrefixTest()
		{
			_communicationService.RegisterReceiveCallBack([this](communication_send_callback_t send, const void *data, size_t len) { return this->_communicationHandler_Prefix.Receive(send, data, len); });
			_communicationHandler_Prefix.RegisterReceiveCallBack([this](communication_send_callback_t send, const void *data, size_t len) { this->handler1count++; return this->handler1return != 0? this->handler1return-- : 0; }, "a", 1);
			_communicationHandler_Prefix.RegisterReceiveCallBack([this](communication_send_callback_t send, const void *data, size_t len) { this->handler2count++; return this->handler2return != 0? this->handler2return-- : 0; }, "b", 1);
			_communicationHandler_Prefix.RegisterReceiveCallBack([this](communication_send_callback_t send, const void *data, size_t len) { this->handler3count++; return this->handler3return != 0? this->handler3return-- : 0; }, "b", 1);
		}
	};

	TEST_F(CommunicationHandler_PrefixTest, CanRegisterAndUnRegisterReceiveCallBack)
	{
		communication_prefix_receive_callback_list_t::iterator iterator = _communicationHandler_Prefix.RegisterReceiveCallBack([](communication_send_callback_t send, const void *data, size_t len) { return 1; }, "c", 1);
		_communicationHandler_Prefix.UnRegisterReceiveCallBack(iterator);
	}

	TEST_F(CommunicationHandler_PrefixTest, WhenSecondCommandHandlesAllThenFirstandThirdCommandNotCalled)
	{
		handler1return = 0;
		handler2return = 1;
		handler3return = 0;
		handler1count = 0;
		handler2count = 0;
		handler3count = 0;

		ASSERT_EQ(2, _communicationService.Receive(reinterpret_cast<const void *>("b "), 2)) << "Correct Handle Count Not Returned";

		ASSERT_EQ(0, handler1count) << "handler 1 called";
		ASSERT_EQ(1, handler2count) << "handler 2 not called";
		ASSERT_EQ(0, handler3count) << "handler 3 called";
	}

	TEST_F(CommunicationHandler_PrefixTest, WhenFirstCommandDoesNotHandleAllThenSecondAndThirdCommandCalled)
	{
		handler1return = 1;
		handler2return = 2;
		handler3return = 1;
		handler1count = 0;
		handler2count = 0;
		handler3count = 0;

		ASSERT_EQ(9, _communicationService.Receive(reinterpret_cast<const void *>("a b  b b "), 9)) << "Correct Handle Count Not Returned";

		ASSERT_EQ(1, handler1count) << "handler 1 not called";
		ASSERT_EQ(3, handler2count) << "handler 2 not called 3 times";
		ASSERT_EQ(1, handler3count) << "handler 3 not called once";
	}
}
