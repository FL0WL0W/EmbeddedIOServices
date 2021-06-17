#include "stdint.h"
#include <functional>
#include <forward_list>

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

		Task(std::function<void()> callBack, bool deleteAfterExecution)
		{
			CallBack = callBack;
			DeleteAfterExecution = deleteAfterExecution;
		}
	};

	class ITimerService
	{
	private:
		void ScheduleFirstTaskInList();
		void RemoveExecutedTasksFromList();
	protected:
		std::forward_list<Task *> _taskList;
		virtual void ScheduleCallBack(const uint32_t tick) = 0;
	public:
		virtual const uint32_t GetTick() = 0;
		virtual const uint32_t GetTicksPerSecond() = 0;

		void ReturnCallBack(void);
		Task *ScheduleTask(std::function<void()>, uint32_t, bool);
		void ScheduleTask(Task *, uint32_t);
		void UnScheduleTask(Task *);
		
		const uint32_t GetElapsedTick(const uint32_t);
		const float GetElapsedTime(const uint32_t);

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
