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

	TEST(TimerService, WhenAddingLaterTask_ThenFirstTaskIsCalledFirst)
	{
		HardwareAbstraction::MockTimerService timerService;
		EXPECT_CALL(timerService, GetTick())
			.WillRepeatedly(Return(0));

		HardwareAbstraction::Task *task1 = new HardwareAbstraction::Task(testCallback1, 0, true);
		timerService.ScheduleTask(task1, 100);
		//ASSERT_EQ(true, task1->Scheduled) << "first callback not set as scheduled";
		ASSERT_EQ((void *)task1, (void *)timerService.FirstTask) << "ScheduledTask not set to first";
			
		HardwareAbstraction::Task *task2 = timerService.ScheduleTask(testCallback2, 0, 150, true);
		// ASSERT_EQ(true, task1->Scheduled) << "first callback not still scheduled";
		// ASSERT_EQ(true, task2->Scheduled) << "second callback not set as scheduled";
		ASSERT_EQ((void *)task1, (void *)timerService.FirstTask) << "ScheduledTask not set to first after new later task added";
		ASSERT_EQ((void *)task2, (void *)timerService.FirstTask->NextTask) << "NextTask not set to second  after new later task added";

		EXPECT_CALL(timerService, GetTick())
			.WillRepeatedly(Return(100));

		timerService.ReturnCallBack();
		// ASSERT_EQ(false, task1->Scheduled) << "first callback still set as scheduled";
		// ASSERT_EQ(true, task2->Scheduled) << "second callback not still scheduled";
		ASSERT_EQ(1, lastCallBack) << "first callback not called";
		ASSERT_EQ((void *)task2, (void *)timerService.FirstTask) << "Schedule not set to second after first task called";

		EXPECT_CALL(timerService, GetTick())
			.WillRepeatedly(Return(150));

		timerService.ReturnCallBack();
		// ASSERT_EQ(false, task1->Scheduled) << "first callback still set as scheduled";
		// ASSERT_EQ(false, task2->Scheduled) << "second callback still set as scheduled";
		ASSERT_EQ(2, lastCallBack) << "second callback not called";
			
		//make sure another callback doesnt mess it up
		lastCallBack = 0;
		timerService.ReturnCallBack();
		// ASSERT_EQ(false, task1->Scheduled) << "first callback still set as scheduled";
		// ASSERT_EQ(false, task2->Scheduled) << "second callback still set as scheduled";
		ASSERT_EQ(0, lastCallBack) << "callback was called";

		//overflow tasks
		EXPECT_CALL(timerService, GetTick())
			.WillRepeatedly(Return(2900000000));

		task1 = timerService.ScheduleTask(testCallback1, 0, 3000000000, true);
		ASSERT_EQ((void *)task1, (void *)timerService.FirstTask) << "ScheduledTask not set to first";

		task2 = timerService.ScheduleTask(testCallback2, 0, 300, true);
		ASSERT_EQ((void *)task1, (void *)timerService.FirstTask) << "ScheduledTask not set to first after new later task added";
		ASSERT_EQ((void *)task2, (void *)timerService.FirstTask->NextTask) << "NextTask not set to second  after new later task added";

		EXPECT_CALL(timerService, GetTick())
			.WillRepeatedly(Return(3000000000));
		timerService.ReturnCallBack();
		ASSERT_EQ(1, lastCallBack) << "first callback not called";
		ASSERT_EQ((void *)task2, (void *)timerService.FirstTask) << "Schedule tick not set to second overflow task after first task called";

		EXPECT_CALL(timerService, GetTick())
			.WillRepeatedly(Return(300));
		timerService.ReturnCallBack();
		ASSERT_EQ(2, lastCallBack) << "second callback not called";
		ASSERT_EQ(0, timerService.FirstTask) << "Schedule tick not set to second overflow task after second task called";

		//make sure another callback doesnt mess it up
		lastCallBack = 0;
		timerService.ReturnCallBack();
		ASSERT_EQ(0, lastCallBack) << "callback was called";
	}
}
