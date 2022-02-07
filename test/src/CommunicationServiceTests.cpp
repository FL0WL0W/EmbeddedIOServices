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
		bool handler1return = false;
		int handler1count = 0;
		bool handler2return = false;
		int handler2count = 0;

		CommunicationServiceTests()
		{
			_communicationService.RegisterHandler(new CommunicationHandler([this](void *data, size_t len) { this->handler1count++; return this->handler1return; }));
			_communicationService.RegisterHandler(new CommunicationHandler([this](void *data, size_t len) { this->handler2count++; return this->handler2return; }));
		}
	};

	TEST_F(CommunicationServiceTests, CanRegisterAndUnRegisterHandler)
	{
		CommunicationHandler handler(0);

		_communicationService.RegisterHandler(&handler);
		_communicationService.UnRegisterHandler(&handler);
	}

	TEST_F(CommunicationServiceTests, WhenFirstHandlerHandlesThenSecondHandleNotCalled)
	{
		handler1return = true;
		handler1count = 0;
		handler2count = 0;

		_communicationService.Receive(0, 0);

		ASSERT_EQ(1, handler1count) << "handler 1 not called";
		ASSERT_EQ(0, handler2count) << "handler 2 called";
	}

	TEST_F(CommunicationServiceTests, WhenFirstHandlerDoesntHandleThenSecondHandleHandles)
	{
		handler1return = false;
		handler1count = 0;
		handler2count = 0;

		_communicationService.Receive(0, 0);

		ASSERT_EQ(1, handler1count) << "handler 1 not called";
		ASSERT_EQ(1, handler2count) << "handler 2 not called";
	}
}
