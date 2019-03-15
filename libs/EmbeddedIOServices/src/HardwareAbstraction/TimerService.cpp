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
	
	void ITimerService::ReturnCallBack(void)
	{
		uint32_t tick;
		Task *next = 0;
		int i = 0;
		while (FirstTask != 0 && TickLessThanEqualToTick(FirstTask->Tick, GetTick() + TimerCallBackAdvance))
		{
			i++;
			next = FirstTask->NextTask;
			
			uint32_t delay = GetTick() - FirstTask->Tick;
			if(delay > _maxDelay && delay < 0x10000000)
			{
				_maxDelay = delay;
				_delayStack = i;
			}

			while(TickLessThanTick(GetTick(), FirstTask->Tick)) ;
			
			FirstTask->Execute();
			FirstTask->Scheduled = false;
			if (FirstTask->DeleteOnExecution)
				delete FirstTask;
			
			FirstTask = next;
		}

		if(FirstTask != 0)
			ScheduleCallBack(FirstTask->Tick);
	}

	Task *ITimerService::ScheduleTask(void(*callBack)(void *), void *parameters, uint32_t tick, bool deleteOnExecution)
	{
		Task *taskToSchedule = new Task(callBack, parameters, deleteOnExecution);

		ScheduleTask(taskToSchedule, tick);

		return taskToSchedule;
	}

	const bool ITimerService::ScheduleTask(Task *task, const uint32_t tick)
	{
		//make this not static 1ms
		if(FirstTask != 0 && TickLessThanTick(FirstTask->Tick, GetTick() + 72000))
			return false;

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
		return GetTick() - lastTick;
	}
	
	const float ITimerService::GetElapsedTime(const uint32_t lastTick)
	{
		return (GetElapsedTick(lastTick) / (float)GetTicksPerSecond());
	}
}
#endif
