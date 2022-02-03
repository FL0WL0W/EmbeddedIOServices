#include "ITimerService.h"

#ifndef MOCKTIMERSERVICE_H
#define MOCKTIMERSERVICE_H
namespace EmbeddedIOServices
{
	class MockTimerService : public ITimerService
	{
	public:
		tick_t Tick;
		MOCK_METHOD1(ScheduleCallBack, void(tick_t tick));
		MOCK_METHOD0(GetTick, tick_t());
		MOCK_METHOD0(GetTicksPerSecond, tick_t());
		MOCK_METHOD0(DisableCallBack, void());
		MOCK_METHOD0(EnableCallBack, void());
		void ReturnCallBackPrivateFunction() { ReturnCallBack(); }
		TaskList GetTaskList() { return _taskList; }
	};
}
#endif