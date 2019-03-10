#include <algorithm>
#include <stdint.h>
#include "HardwareAbstraction/ITimerService.h"

#ifdef ITIMERSERVICE_H
namespace HardwareAbstraction
{	
	static bool TickLessThanTick(const uint32_t i, const uint32_t j)
	{
		return (j > 2147483647 && i + 2147483647 < j + 2147483647) || (j <= 2147483647 && i < j);
	}

	static bool TickLessThanEqualToTick(const uint32_t i, const uint32_t j)
	{
		return (j > 2147483647 && i + 2147483647 <= j + 2147483647) || (j <= 2147483647 && i <= j);
	}

	static uint32_t TickMinusTick(const uint32_t i, const uint32_t j)
	{
		if(j > 2147483647)
		{
			return (i + 2147483647) - (j + 2147483647);
		}
		else
		{
			return i - j;
		}
	}
	
	void CallBackGroup::Execute()
	{
		for (std::list<ICallBack *>::const_iterator iterator = _callBackList.begin(), end = _callBackList.end(); iterator != end; ++iterator)
		{
			(*iterator)->Execute();
		}
	}
	
	void CallBackGroup::Add(void(*callBackPointer)(void *), void *parameters)
	{
		Add(new CallBack(callBackPointer, parameters));
	}
	
	void CallBackGroup::Add(ICallBack *callBack)
	{
		_callBackList.push_back(callBack);
	}
	
	void CallBackGroup::Remove(ICallBack *callBack)
	{
		_callBackList.remove(callBack);
	}
	
	void CallBackGroup::Clear()
	{
		_callBackList.clear();
	}

	void ITimerService::ReturnCallBack(void)
	{
		uint32_t tick;
		while (ScheduledTask != 0 && TickLessThanEqualToTick(ScheduledTask->Tick, GetTick()))
		{
			Task *callTask = ScheduledTask;
			ScheduledTask = callTask->NextTask;
			callTask->Execute();
			if (callTask->DeleteOnExecution)
				delete callTask;
		}

		if (ScheduledTask == 0)
			return;

		ScheduleCallBack(ScheduledTask->Tick);
	}

	Task *ITimerService::ScheduleTask(void(*callBack)(void *), void *parameters, uint32_t tick, bool deleteOnExecution)
	{
		Task *taskToSchedule = new Task(callBack, parameters, deleteOnExecution);

		ScheduleTask(taskToSchedule, tick);

		return taskToSchedule;
	}

	const bool ITimerService::ScheduleTask(Task *task, const uint32_t tick)
	{
		task->Tick = tick;
		if(ScheduledTask == 0)
		{
			ScheduledTask = task;
		}
		else
		{
			Task *iterator = ScheduledTask;
			while(iterator->NextTask != 0 && TickLessThanTick(iterator->NextTask->Tick, tick)) iterator = iterator->NextTask;

			task->NextTask = iterator->NextTask;
			iterator->NextTask = task;			
		}
		
		if (ScheduledTask == task)
		{
			ScheduleCallBack(tick);
		}
		return true;
	}

	const bool ITimerService::ReScheduleTask(Task *task, const uint32_t tick)
	{
		//if next scheduled task and not moving
		if(ScheduledTask == task && (ScheduledTask->NextTask == 0 || TickLessThanTick(tick, ScheduledTask->NextTask->Tick)))
		{
			task->Tick = tick;
			ScheduleCallBack(tick);
			return true;
		}
		else
		{
			UnScheduleTask(task);
			return ScheduleTask(task, tick);
		}
	}

	const bool ITimerService::UnScheduleTask(Task *task)
	{
		//if is next scheduled task
		if(ScheduledTask == task)
		{
			ScheduledTask = ScheduledTask->NextTask;
			task->NextTask = 0;
			ScheduleCallBack(ScheduledTask->Tick);
		}
		else if(ScheduledTask != 0)
		{
			Task *iterator = ScheduledTask;
			while(iterator->NextTask != 0 && iterator->NextTask != task) iterator = iterator->NextTask;
			if(iterator->NextTask != 0)
				iterator->NextTask = iterator->NextTask->NextTask;
			task->NextTask = 0;
		}
		
		return true;
	}
	
	const uint32_t ITimerService::GetElapsedTick(const uint32_t lastTick)
	{
		return TickMinusTick(GetTick(), lastTick);
	}
	
	const float ITimerService::GetElapsedTime(const uint32_t lastTick)
	{
		return (GetElapsedTick(lastTick) / (float)GetTicksPerSecond());
	}
}
#endif
