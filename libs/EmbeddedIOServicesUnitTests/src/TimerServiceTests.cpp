#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "ITimerService.h"
#include "MockTimerService.h"
using ::testing::Return;
using EmbeddedIOServices::MockTimerService;
using EmbeddedIOServices::Task;
using EmbeddedIOServices::CallBack;

namespace UnitTests
{
    class timerTestClass
    {
        public:
        int lastCallBack;

        void testCallback1()
        {
            lastCallBack = 1;
        }
        void testCallback2()
        {
            lastCallBack = 2;
        }
    };

	TEST(TimerService, WhenAddingLaterTask_ThenFirstTaskIsCalledFirst)
	{
        timerTestClass *timerTestClassInstance = new timerTestClass();

		MockTimerService timerService;
		EXPECT_CALL(timerService, GetTick())
			.WillRepeatedly(Return(0));

		Task *task1 = new Task(new CallBack<timerTestClass>(timerTestClassInstance, &timerTestClass::testCallback1), true);
		timerService.ScheduleTask(task1, 100);
		//ASSERT_EQ(true, task1->Scheduled) << "first callback not set as scheduled";
		ASSERT_EQ((void *)task1, (void *)timerService.FirstTask) << "ScheduledTask not set to first";
			
		Task *task2 = timerService.ScheduleTask(new CallBack<timerTestClass>(timerTestClassInstance, &timerTestClass::testCallback2), 150, true);
		// ASSERT_EQ(true, task1->Scheduled) << "first callback not still scheduled";
		// ASSERT_EQ(true, task2->Scheduled) << "second callback not set as scheduled";
		ASSERT_EQ((void *)task1, (void *)timerService.FirstTask) << "ScheduledTask not set to first after new later task added";
		ASSERT_EQ((void *)task2, (void *)timerService.FirstTask->NextTask) << "NextTask not set to second  after new later task added";

		EXPECT_CALL(timerService, GetTick())
			.WillRepeatedly(Return(100));

		timerService.ReturnCallBack();
		// ASSERT_EQ(false, task1->Scheduled) << "first callback still set as scheduled";
		// ASSERT_EQ(true, task2->Scheduled) << "second callback not still scheduled";
		ASSERT_EQ(1, timerTestClassInstance->lastCallBack) << "first callback not called";
		ASSERT_EQ(false, timerService.FirstTask->Scheduled) << "Schedule not set to second after first task called";

		EXPECT_CALL(timerService, GetTick())
			.WillRepeatedly(Return(150));

		timerService.ReturnCallBack();
		// ASSERT_EQ(false, task1->Scheduled) << "first callback still set as scheduled";
		// ASSERT_EQ(false, task2->Scheduled) << "second callback still set as scheduled";
		ASSERT_EQ(2, timerTestClassInstance->lastCallBack) << "second callback not called";
			
		//make sure another callback doesnt mess it up
		timerTestClassInstance->lastCallBack = 0;
		timerService.ReturnCallBack();
		// ASSERT_EQ(false, task1->Scheduled) << "first callback still set as scheduled";
		// ASSERT_EQ(false, task2->Scheduled) << "second callback still set as scheduled";
		ASSERT_EQ(0, timerTestClassInstance->lastCallBack) << "callback was called";

		//overflow tasks
		EXPECT_CALL(timerService, GetTick())
			.WillRepeatedly(Return(2900000000));

		task1 = timerService.ScheduleTask(new CallBack<timerTestClass>(timerTestClassInstance, &timerTestClass::testCallback1), 3000000000, true);
		ASSERT_EQ((void *)task1, (void *)timerService.FirstTask) << "ScheduledTask not set to first";

		task2 = timerService.ScheduleTask(new CallBack<timerTestClass>(timerTestClassInstance, &timerTestClass::testCallback2), 300, true);
		ASSERT_EQ((void *)task1, (void *)timerService.FirstTask) << "ScheduledTask not set to first after new later task added";
		ASSERT_EQ((void *)task2, (void *)timerService.FirstTask->NextTask) << "NextTask not set to second  after new later task added";

		EXPECT_CALL(timerService, GetTick())
			.WillRepeatedly(Return(3000000000));
		timerService.ReturnCallBack();
		ASSERT_EQ(1, timerTestClassInstance->lastCallBack) << "first callback not called";
		ASSERT_EQ(false, timerService.FirstTask->Scheduled) << "Schedule tick not set to second overflow task after first task called";

		EXPECT_CALL(timerService, GetTick())
			.WillRepeatedly(Return(300));
		timerService.ReturnCallBack();
		ASSERT_EQ(2, timerTestClassInstance->lastCallBack) << "second callback not called";
		ASSERT_EQ(false, timerService.FirstTask->NextTask->Scheduled) << "Schedule tick not set to second overflow task after second task called";

		//make sure another callback doesnt mess it up
		timerTestClassInstance->lastCallBack = 0;
		timerService.ReturnCallBack();
		ASSERT_EQ(0, timerTestClassInstance->lastCallBack) << "callback was called";
	}
}
