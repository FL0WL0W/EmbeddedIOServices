#include <algorithm>
#include <stdint.h>
#include "HardwareAbstraction/ITimerService.h"

#ifdef ITIMERSERVICE_H
namespace HardwareAbstraction
{		
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
	
	bool ITimerService::TickLessThanTick(const uint32_t i, const uint32_t j)
	{
		return (j > 2147483647 && i + 2147483647 < j + 2147483647) || (j <= 2147483647 && i < j);
	}

	bool ITimerService::TickLessThanEqualToTick(const uint32_t i, const uint32_t j)
	{
		return (j > 2147483647 && i + 2147483647 <= j + 2147483647) || (j <= 2147483647 && i <= j);
	}

	uint32_t ITimerService::TickMinusTick(const uint32_t i, const uint32_t j)
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

	void ITimerService::ReturnCallBack(void)
	{
		if(_disableCallBack)
		{
			_callBackCalledWhileDisabled = true;
			return;
		}
		_callBackCalledWhileDisabled = false;

		uint32_t tick;
		while (ScheduledTask != 0 && TickLessThanEqualToTick(ScheduledTask->Tick, GetTick()))
		{
			ScheduledTask->Execute();
			Task *callTask = ScheduledTask;
			ScheduledTask = ScheduledTask->NextTask;
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
		//make sure we have enough time to schedule
		uint32_t curr = GetTick();
		if(TickLessThanTick(tick, curr + 50))
			return false;

		Task *iterator = ScheduledTask;
		if(iterator == 0)
		{
			task->Tick = tick;
			task->NextTask = 0;
			ScheduledTask = task;
			ScheduleCallBack(tick);
		}
		else if(iterator != task)
		{
			while(iterator->NextTask != 0 && TickLessThanTick(iterator->NextTask->Tick, tick)) iterator = iterator->NextTask;

			if(iterator == ScheduledTask && TickLessThanTick(tick, iterator->Tick))
			{
				task->NextTask = ScheduledTask;
				ScheduledTask = task;

				ScheduleCallBack(tick);
			}
			else
			{
				task->Tick = tick;
				task->NextTask = iterator->NextTask;
				iterator->NextTask = task;
			}
		}		
		
		return true;
	}

	const bool ITimerService::ReScheduleTask(Task *task, const uint32_t tick)
	{
		//make sure we have enough time to schedule
		uint32_t curr = GetTick();
		if(TickLessThanTick(tick, curr + 50))
			return false;

		bool success = false;
		//if next scheduled task and not moving
		if(ScheduledTask == task && (ScheduledTask->NextTask == 0 || TickLessThanTick(tick, ScheduledTask->NextTask->Tick)))
		{
			task->Tick = tick;
			ScheduleCallBack(tick);
			success = true;
		}
		else
		{
			if(UnScheduleTask(task))
				success = ScheduleTask(task, tick);
		}

		return success;
	}

	const bool ITimerService::UnScheduleTask(Task *task)
	{
		//make sure we have enough time to schedule
		uint32_t curr = GetTick();
		if(TickLessThanTick(curr + 50, task->Tick))
			return false;

		//if is next scheduled task
		if(ScheduledTask == task)
		{
			ScheduledTask = ScheduledTask->NextTask;
			ScheduleCallBack(ScheduledTask->Tick);
		}
		else if(ScheduledTask != 0)
		{
			Task *iterator = ScheduledTask;
			while(iterator->NextTask != 0 && iterator->NextTask != task) iterator = iterator->NextTask;
			uint32_t curr = GetTick();
			if(TickLessThanTick(iterator->Tick, curr + 50))
				return false;
			if(iterator->NextTask != 0)
			{
				iterator->NextTask = iterator->NextTask->NextTask;
				task->NextTask = 0;
			}
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
