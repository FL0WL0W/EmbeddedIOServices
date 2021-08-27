#include "CallBack.h"
#include "stdint.h"
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
		callback_t CallBack;
		//only valid when scheduled or during callback
		tick_t ScheduledTick;
		volatile tick_t ExecutedTick;
		volatile bool Scheduled : 1;
		const bool DeleteAfterExecution : 1;

		Task(callback_t callBack, bool deleteAfterExecution) : 
			CallBack(callBack),
			ScheduledTick(0),
			ExecutedTick(0),
			Scheduled(false),
			DeleteAfterExecution(deleteAfterExecution)
		{ }

		Task(callback_t callBack) : 
			Task(callBack, false)
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

	typedef std::forward_list<ScheduleRequest> ScheduleRequestList;
	typedef std::forward_list<Task *> RemoveRequestList;
#endif

	typedef std::list<Task *> TaskList;

	class ITimerService
	{
	private:
#ifdef ALLOW_TASK_TO_SCHEDULE_IN_CALLBACK
		ScheduleRequestList _scheduleRequestList;
		RemoveRequestList _removeRequestList;
		bool _scheduleLock = false;
#endif
		uint16_t _latency;
		TaskList::iterator RemoveUnscheduledTasksAndReturnBegin();
	protected:
		TaskList _taskList;
		virtual void ScheduleCallBack(const tick_t tick) = 0;
		void ReturnCallBack();
	public:
		virtual tick_t GetTick() = 0;
		virtual tick_t GetTicksPerSecond() = 0;

		virtual void Calibrate();

		void ScheduleCallBack(callback_t, tick_t);
		void ScheduleTask(Task *, tick_t);
		void UnScheduleTask(Task *);
#ifdef ALLOW_TASK_TO_SCHEDULE_IN_CALLBACK
		void FlushScheduleRequests();
#endif
	
		constexpr static bool TickLessThanTick(const tick_t i, const tick_t j)
		{
			return (i - j) & (1<<(sizeof(tick_t) * 8 - 1));
		}

		constexpr static bool TickLessThanEqualToTick(const tick_t i, const tick_t j)
		{
			return !TickLessThanTick(j, i);
		}
	};
}
#endif
