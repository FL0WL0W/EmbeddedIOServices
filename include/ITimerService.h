#include "stdint.h"
#include <functional>
#include <forward_list>
#include <list>

#ifndef ITIMERSERVICE_H
#define ITIMERSERVICE_H

namespace EmbeddedIOServices
{
	struct Task
	{
		public:
		std::function<void()> CallBack;
		uint32_t Tick;
		bool Scheduled : 1;
		bool DeleteAfterExecution : 1;

		Task(std::function<void()> callBack)
		{
			CallBack = callBack;
			DeleteAfterExecution = false;
		}
	};

#ifdef ALLOW_TASK_TO_SCHEDULE_IN_CALLBACK
	struct ScheduleRequest
	{
		public:
		Task *TaskToSchedule;
		uint32_t Tick;

		ScheduleRequest(Task *task, uint32_t tick) : TaskToSchedule(task), Tick(tick) { }
	};
#endif

	class ITimerService
	{
	private:
#ifdef ALLOW_TASK_TO_SCHEDULE_IN_CALLBACK
		std::list<ScheduleRequest> _scheduleRequestList;
		bool _scheduleLock = false;
		void FlushScheduleRequests();
#endif
		void ScheduleFirstTaskInList();
	protected:
		std::forward_list<Task *> _taskList;
		virtual void ScheduleCallBack(const uint32_t tick) = 0;
		void ReturnCallBack();
	public:
		virtual const uint32_t GetTick() = 0;
		virtual const uint32_t GetTicksPerSecond() = 0;

		void ScheduleCallBack(std::function<void()>, uint32_t);
		void ScheduleTask(Task *, uint32_t);
		void UnScheduleTask(Task *);
		
		constexpr static bool TickLessThanTick(const uint32_t i, const uint32_t j)
		{
			return i - j > 0x80000000;
		}

		constexpr static bool TickLessThanEqualToTick(const uint32_t i, const uint32_t j)
		{
			return !TickLessThanTick(j, i);
		}
	};
}
#endif
