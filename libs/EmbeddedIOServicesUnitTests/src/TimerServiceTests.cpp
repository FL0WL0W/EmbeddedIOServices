#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "HardwareAbstraction/ITimerService.h"
#include "MockTimerService.h"
using ::testing::Return;

namespace UnitTests
{
	static int lastCallBack;

	static void testCallback1(void *parameters)
	{
		lastCallBack = 1;
	}
	static void testCallback2(void *parameters)
	{
		lastCallBack = 2;
	}

	TEST(IndependentMethod, TimerService_WhenAddingLaterTaskFirstTaskIsCalledFirst)
	{
		HardwareAbstraction::MockTimerService timerService;
		EXPECT_CALL(timerService, GetTick())
			.WillRepeatedly(Return(0));

		HardwareAbstraction::Task *task1 = timerService.ScheduleTask(testCallback1, 0, 100, true);
		ASSERT_EQ((void *)task1, (void *)timerService.CallBackStackPointer[timerService.StackSize - 1]) << "CallBackStackPointer not set to first";
			
		HardwareAbstraction::Task *task2 = timerService.ScheduleTask(testCallback2, 0, 150, true);
		ASSERT_EQ((void *)task1, (void *)timerService.CallBackStackPointer[timerService.StackSize - 1]) << "CallBackStackPointer not set to first after new later task added";
		ASSERT_EQ((void *)task2, (void *)timerService.CallBackStackPointer[timerService.StackSize - 2]) << "CallBackStackPointer-1 not set to second  after new later task added";

		EXPECT_CALL(timerService, GetTick())
			.WillRepeatedly(Return(100));

		timerService.ReturnCallBack();
		ASSERT_EQ(1, lastCallBack) << "first callback not called";
		ASSERT_EQ((void *)task2, (void *)timerService.CallBackStackPointer[timerService.StackSize - 1]) << "Schedule not set to second after first task called";

		EXPECT_CALL(timerService, GetTick())
			.WillRepeatedly(Return(150));

		timerService.ReturnCallBack();
		ASSERT_EQ(2, lastCallBack) << "second callback not called";
			
		//make sure another callback doesnt mess it up
		lastCallBack = 0;
		timerService.ReturnCallBack();
		ASSERT_EQ(0, lastCallBack) << "callback was called";

		//overflow tasks
		EXPECT_CALL(timerService, GetTick())
			.WillRepeatedly(Return(2900000000));

		task1 = timerService.ScheduleTask(testCallback1, 0, 3000000000, true);
		ASSERT_EQ((void *)task1, (void *)timerService.CallBackStackPointer[timerService.StackSize - 1]) << "CallBackStackPointer not set to first";

		task2 = timerService.ScheduleTask(testCallback2, 0, 300, true);
		ASSERT_EQ((void *)task1, (void *)timerService.CallBackStackPointer[timerService.StackSize - 1]) << "CallBackStackPointer not set to first after new later task added";
		ASSERT_EQ((void *)task2, (void *)timerService.CallBackStackPointer[timerService.StackSize - 2]) << "CallBackStackPointer-1 not set to second  after new later task added";

		EXPECT_CALL(timerService, GetTick())
			.WillRepeatedly(Return(3000000000));
		timerService.ReturnCallBack();
		ASSERT_EQ(1, lastCallBack) << "first callback not called";
		ASSERT_EQ((void *)task2, (void *)timerService.CallBackStackPointer[timerService.StackSize - 1]) << "Schedule tick not set to second overflow task after first task called";

		EXPECT_CALL(timerService, GetTick())
			.WillRepeatedly(Return(300));
		timerService.ReturnCallBack();
		ASSERT_EQ(2, lastCallBack) << "second callback not called";
		ASSERT_EQ((unsigned char)0, timerService.StackSize) << "Schedule tick not set to second overflow task after second task called";

		//make sure another callback doesnt mess it up
		lastCallBack = 0;
		timerService.ReturnCallBack();
		ASSERT_EQ(0, lastCallBack) << "callback was called";
	}
}