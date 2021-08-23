#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "ITimerService.h"
#include "MockTimerService.h"
using namespace testing;
using namespace EmbeddedIOServices;

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

		Task *task1 = new Task([timerTestClassInstance](){ timerTestClassInstance->testCallback1(); });
		Task *task2 = new Task([timerTestClassInstance](){ timerTestClassInstance->testCallback2(); });

		timerService.ScheduleTask(task1, 100);
		bool scheduled;
		ASSERT_EQ(true, scheduled = task1->Scheduled) << "first callback not set as scheduled";
		ASSERT_EQ(task1, *timerService.GetTaskList().begin()) << "ScheduledTask not set to first";
			
		timerService.ScheduleTask(task2, 150);
		ASSERT_EQ(true, scheduled = task1->Scheduled) << "first callback not still scheduled";
		ASSERT_EQ(true, scheduled = task2->Scheduled) << "second callback not set as scheduled";
		ASSERT_EQ(task1, *timerService.GetTaskList().begin()) << "ScheduledTask not set to first after new later task added";
		ASSERT_EQ(task2, *++timerService.GetTaskList().begin()) << "NextTask not set to second  after new later task added";

		EXPECT_CALL(timerService, GetTick())
			.WillRepeatedly(Return(100));

		timerService.ReturnCallBackPrivateFunction();
		ASSERT_EQ(false, scheduled = task1->Scheduled) << "first callback still set as scheduled";
		ASSERT_EQ(true, scheduled = task2->Scheduled) << "second callback not still scheduled";
		ASSERT_EQ(1, timerTestClassInstance->lastCallBack) << "first callback not called";
		ASSERT_EQ(false, scheduled = (*timerService.GetTaskList().begin())->Scheduled) << "Schedule not set to second after first task called";

		EXPECT_CALL(timerService, GetTick())
			.WillRepeatedly(Return(150));

		timerService.ReturnCallBackPrivateFunction();
		ASSERT_EQ(false, scheduled = task1->Scheduled) << "first callback still set as scheduled";
		ASSERT_EQ(false, scheduled = task2->Scheduled) << "second callback still set as scheduled";
		ASSERT_EQ(2, timerTestClassInstance->lastCallBack) << "second callback not called";
			
		//make sure another callback doesnt mess it up
		timerTestClassInstance->lastCallBack = 0;
		timerService.ReturnCallBackPrivateFunction();
		ASSERT_EQ(false, scheduled = task1->Scheduled) << "first callback still set as scheduled";
		ASSERT_EQ(false, scheduled = task2->Scheduled) << "second callback still set as scheduled";
		ASSERT_EQ(0, timerTestClassInstance->lastCallBack) << "callback was called";

		//overflow tasks
		EXPECT_CALL(timerService, GetTick())
			.WillRepeatedly(Return(2900000000));

		timerService.ScheduleTask(task1, 3000000000);
		ASSERT_EQ(task1, *timerService.GetTaskList().begin()) << "ScheduledTask not set to first";

		timerService.ScheduleTask(task2, 300);
		ASSERT_EQ(task1, *timerService.GetTaskList().begin()) << "ScheduledTask not set to first after new later task added";
		ASSERT_EQ(task2, *++timerService.GetTaskList().begin()) << "NextTask not set to second  after new later task added";

		EXPECT_CALL(timerService, GetTick())
			.WillRepeatedly(Return(3000000000));
		timerService.ReturnCallBackPrivateFunction();
		//ASSERT_EQ(0, task2->TickDeviation) << "second callback not called";
		ASSERT_EQ(1, timerTestClassInstance->lastCallBack) << "tick deviation not set correctly";
		ASSERT_EQ(false, scheduled = (*timerService.GetTaskList().begin())->Scheduled) << "Schedule tick not set to second overflow task after first task called";

		EXPECT_CALL(timerService, GetTick())
			.WillRepeatedly(Return(301));
		timerService.ReturnCallBackPrivateFunction();
		//ASSERT_EQ(1, task2->TickDeviation) << "second callback not called";
		ASSERT_EQ(2, timerTestClassInstance->lastCallBack) << "tick deviation not set correctly";
		ASSERT_EQ(false, scheduled = (*++timerService.GetTaskList().begin())->Scheduled) << "Schedule tick not set to second overflow task after second task called";

		//make sure another callback doesnt mess it up
		timerTestClassInstance->lastCallBack = 0;
		timerService.ReturnCallBackPrivateFunction();
		ASSERT_EQ(0, timerTestClassInstance->lastCallBack) << "callback was called";
	}
}
