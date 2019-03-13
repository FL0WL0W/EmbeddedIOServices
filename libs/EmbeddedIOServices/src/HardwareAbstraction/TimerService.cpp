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
		return (j > 2147483647 && static_cast<uint32_t>(i + 2147483647) < static_cast<uint32_t>(j + 2147483647)) || (j <= 2147483647 && i < j);
	}

	bool ITimerService::TickLessThanEqualToTick(const uint32_t i, const uint32_t j)
	{
		return (j > 2147483647 && static_cast<uint32_t>(i + 2147483647) <= static_cast<uint32_t>(j + 2147483647)) || (j <= 2147483647 && i <= j);
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

	int64_t ITimerService::TickMinusTickSigned(uint32_t i, uint32_t j)
	{
		if(j > 2147483647)
		{
			i = i + 2147483647;
			j = j + 2147483647;
		}
		return i - j;
	}

	void ITimerService::ReturnCallBack(void)
	{
		if(_disableCallBack)
			return;
		_disableCallBack = true;

		uint32_t tick;
		Task *iterator = FirstTask;
		Task *next = 0;
		Task *prev = 0;
		while (iterator != 0)
		{
			next = iterator->NextTask;
			if(TickLessThanEqualToTick(iterator->Tick, GetTick()))
			{
				iterator->Execute();
				if(FirstTask == iterator)
				{
					FirstTask = iterator->NextTask;
					iterator->Scheduled = false;
					iterator->NextTask = 0;
					if (iterator->DeleteOnExecution)
						delete iterator;
					prev = iterator;
				}
				else
				{
					prev->NextTask = iterator->NextTask;
					iterator->Scheduled = false;
					iterator->NextTask = 0;
					if (iterator->DeleteOnExecution)
						delete iterator;
				}
			}
			else
			{
				prev = iterator;
			}
			
			iterator = next;
		}

		if(FirstTask != 0)
		{
			_disableCallBack = false;
			ScheduleCallBack(FirstTask->Tick);
		}
		else
			_disableCallBack = false;
	}

	Task *ITimerService::ScheduleTask(void(*callBack)(void *), void *parameters, uint32_t tick, bool deleteOnExecution)
	{
		Task *taskToSchedule = new Task(callBack, parameters, deleteOnExecution);

		ScheduleTask(taskToSchedule, tick);

		return taskToSchedule;
	}

	const bool ITimerService::ScheduleTask(Task *task, const uint32_t tick)
	{
		bool disabledCallBackPrim = _disableCallBack;
		_disableCallBack = true;

		//min tick is current tick
		if(TickLessThanTick(tick, GetTick()))
		 	task->Tick = GetTick();
		else
			task->Tick = tick;

		//set to not scheduled
		task->Scheduled = false;

		//remove all FirstTasks where not scheduled
		while(FirstTask != 0 && !FirstTask->Scheduled)
		{
			FirstTask = FirstTask->NextTask;
		}

		if(FirstTask != 0)
		{
			//task is somewhere in the task list
			//remove task
			Task *iterator = FirstTask;
			while (iterator->NextTask != 0)
			{
				if(!iterator->NextTask->Scheduled)
				{
					//unschedule task
					iterator->NextTask = iterator->NextTask->NextTask;
				}
				else
				{
					iterator = iterator->NextTask;
				}
			}

			if(TickLessThanTick(task->Tick, FirstTask->Tick))
			{
				//task is FirstTask
				task->Scheduled = true;
				task->NextTask = FirstTask;
				FirstTask = task;
			}
			else
			{
				//insert task
				iterator = FirstTask;
				while (iterator->NextTask != 0 && !task->Scheduled)
				{
					//this is where our task is to be scheduled
					if(TickLessThanTick(task->Tick, iterator->NextTask->Tick))
					{
						task->Scheduled = true;
						task->NextTask = iterator->NextTask;
						iterator->NextTask = task;
					}

					iterator = iterator->NextTask;
				}

				if(!task->Scheduled)
				{
					task->Scheduled = true;
					task->NextTask = 0;
					iterator->NextTask = task;
				}		
			}
		}
		else
		{
			//task is only task
			task->Scheduled = true;
			task->NextTask = 0;
			FirstTask = task;
		}

		_disableCallBack = disabledCallBackPrim;
		ScheduleCallBack(FirstTask->Tick);

		return true;
	}

	const bool ITimerService::UnScheduleTask(Task *task)
	{
		bool disableCallBackPrim = _disableCallBack;
		_disableCallBack = true;

		//if is next scheduled task
		Task *iterator = FirstTask;
		if(iterator == task)
		{
			FirstTask = iterator->NextTask;
			task->NextTask = 0;
		}
		else if(iterator != 0)
		{
			while(iterator->NextTask != 0 && iterator->NextTask != task) iterator = iterator->NextTask;
			if(iterator->NextTask != 0)
				iterator->NextTask = iterator->NextTask->NextTask;
			task->NextTask = 0;
		}
		
		if(FirstTask != 0)
		{
			_disableCallBack = disableCallBackPrim;
			if(!_disableCallBack)
				ScheduleCallBack(FirstTask->Tick);
		}
		else
			_disableCallBack = disableCallBackPrim;

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
