#include "stdint.h"
#include <functional>
#include <forward_list>
#include <list>

#ifndef ITIMERSERVICE_H
#define ITIMERSERVICE_H

namespace EmbeddedIOServices
{
	typedef uint32_t tick_t;

	struct Task
	{
		public:
		std::function<void()> CallBack;
		tick_t Tick;
		bool Scheduled : 1;
		bool DeleteAfterExecution : 1;

		Task(std::function<void()> callBack) : 
			CallBack(callBack),
			Tick(0),
			Scheduled(false),
			DeleteAfterExecution(false)
		{ }
	};

#ifdef ALLOW_TASK_TO_SCHEDULE_IN_CALLBACK
	struct ScheduleRequest
	{
		public:
		Task *TaskToSchedule;
		tick_t Tick;

		ScheduleRequest(Task *task, tick_t tick) : TaskToSchedule(task), Tick(tick) { }
	};
#endif

	typedef std::list<Task *> TaskList;

	class ITimerService
	{
	private:
#ifdef ALLOW_TASK_TO_SCHEDULE_IN_CALLBACK
		std::forward_list<ScheduleRequest> _scheduleRequestList;
		std::forward_list<Task *> _removeRequestList;
		bool _scheduleLock = false;
		void FlushScheduleRequests();
#endif
		uint16_t _latency;
		TaskList::iterator RemoveUnscheduledTasksAndReturnBegin();
	protected:
		TaskList _taskList;
		virtual void ScheduleCallBack(const tick_t tick) = 0;
		void ReturnCallBack();
	public:
		virtual const tick_t GetTick() = 0;
		virtual const tick_t GetTicksPerSecond() = 0;

		virtual void Calibrate();

		void ScheduleCallBack(std::function<void()>, tick_t);
		void ScheduleTask(Task *, tick_t);
		void UnScheduleTask(Task *);
	
		constexpr static bool TickLessThanTick(const tick_t i, const tick_t j)
		{
			return i - j > 0x80000000;
		}

		constexpr static bool TickLessThanEqualToTick(const tick_t i, const tick_t j)
		{
			return !TickLessThanTick(j, i);
		}
	};
}
#endif
