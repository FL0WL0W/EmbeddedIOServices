#include "ITimerService.h"

#ifndef MOCKTIMERSERVICE_H
#define MOCKTIMERSERVICE_H
namespace EmbeddedIOServices
{
	class MockTimerService : public ITimerService
	{
	public:
		uint32_t Tick;
		MOCK_METHOD1(ScheduleCallBack, void(uint32_t tick));
		MOCK_METHOD0(GetTick, const uint32_t());
		MOCK_METHOD0(GetTicksPerSecond, const uint32_t());
		MOCK_METHOD0(DisableCallBack, void());
		MOCK_METHOD0(EnableCallBack, void());
		void ReturnCallBackPrivateFunction() { ReturnCallBack(); }
		std::forward_list<Task *> GetTaskList() { return _taskList; }
	};
}
#endif