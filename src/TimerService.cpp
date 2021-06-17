#include <algorithm>
#include <stdint.h>
#include "ITimerService.h"

#ifdef ITIMERSERVICE_H
namespace EmbeddedIOServices
{	
	void ITimerService::RemoveExecutedTasksFromList()
	{
		std::forward_list<Task *>::iterator next = _taskList.begin();
		while(next != _taskList.end() && (*next)->Scheduled == false)
		{
			if((*next)->DeleteAfterExecution)
				delete *next;
			_taskList.pop_front();
			next++;
		}
	}

	void ITimerService::ScheduleFirstTaskInList()
	{
		std::forward_list<Task *>::iterator next = _taskList.begin();
		if(next != _taskList.end())
			ScheduleCallBack((*next)->Tick);
	}

	void ITimerService::ReturnCallBack(void)
	{
		std::forward_list<Task *>::iterator next = _taskList.begin();;
		while(next != _taskList.end() && (*next)->Scheduled == false)
			next++;

		while (next != _taskList.end() && TickLessThanEqualToTick((*next)->Tick, GetTick()))
		{
			(*next)->CallBack();
			(*next)->Scheduled = false;
			next++;
		}

		if(next != _taskList.end())
		 	ScheduleCallBack((*next)->Tick);
	}

	Task *ITimerService::ScheduleTask(std::function<void()> callBack, uint32_t tick, bool deleteAfterExecution)
	{
		Task *taskToSchedule = new Task(callBack, deleteAfterExecution);
		
		ScheduleTask(taskToSchedule, tick);

		return taskToSchedule;
	}

	void ITimerService::ScheduleTask(Task *task, uint32_t tick)
	{
		_taskList.remove(task);
		RemoveExecutedTasksFromList();

		if(_taskList.empty())
		{
			task->Tick = tick;
			task->Scheduled = true;
			_taskList.push_front(task);
		}
		else
		{
			std::forward_list<Task *>::iterator before = _taskList.before_begin();
			std::forward_list<Task *>::iterator next = before;
			next++;
			while(next != _taskList.end())
			{
				if(TickLessThanTick(tick, (*next)->Tick))
				{
					task->Tick = tick;
					task->Scheduled = true;
					_taskList.insert_after(before, task);
				}

				before = next;
				next++;
			}
		}

		ScheduleFirstTaskInList();
	}

	void ITimerService::UnScheduleTask(Task *task)
	{
		_taskList.remove(task);
		RemoveExecutedTasksFromList();
		ScheduleFirstTaskInList();
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
