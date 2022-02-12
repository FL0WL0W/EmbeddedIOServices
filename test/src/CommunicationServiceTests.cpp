#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "ICommunicationService.h"
#include "MockCommunicationService.h"
using namespace testing;
using namespace EmbeddedIOServices;

namespace UnitTests
{
	class CommunicationServiceTests : public ::testing::Test 
	{
		protected:
		MockCommunicationService _communicationService;
		communication_receive_callback_t handler1;
		communication_receive_callback_t handler2;
		size_t handler1return = 0;
		int handler1count = 0;
		size_t handler2return = 0;
		int handler2count = 0;

		CommunicationServiceTests()
		{
			handler1 = [this](void *data, size_t len) { this->handler1count++; return this->handler1return != 0? this->handler1return-- : 0; };
			handler2 = [this](void *data, size_t len) { this->handler2count++; return this->handler2return != 0? this->handler2return-- : 0; };
			_communicationService.RegisterReceiveCallBack(&handler1);
			_communicationService.RegisterReceiveCallBack(&handler2);
		}
	};

	TEST_F(CommunicationServiceTests, CanRegisterAndUnRegisterReceiveCallBack)
	{
		communication_receive_callback_t handler = [](void *data, size_t len) { return 1; };

		_communicationService.RegisterReceiveCallBack(&handler);
		_communicationService.UnRegisterReceiveCallBack(&handler);
	}

	TEST_F(CommunicationServiceTests, WhenFirstHandlerHandlesAllThenSecondHandleNotCalled)
	{
		handler1return = 1;
		handler1count = 0;
		handler2count = 0;

		ASSERT_EQ(1, _communicationService.Receive(0, 1)) << "Correct Handle Count Not Returned";

		ASSERT_EQ(1, handler1count) << "handler 1 not called";
		ASSERT_EQ(0, handler2count) << "handler 2 called";
	}

	TEST_F(CommunicationServiceTests, WhenFirstHandlerDoesNotHandleAllThenSecondHandleCalled)
	{
		handler1return = 1;
		handler2return = 2;
		handler1count = 0;
		handler2count = 0;

		ASSERT_EQ(4, _communicationService.Receive(0, 4)) << "Correct Handle Count Not Returned";

		ASSERT_EQ(3, handler1count) << "handler 1 not called";
		ASSERT_EQ(2, handler2count) << "handler 2 not called twice";
	}
}
