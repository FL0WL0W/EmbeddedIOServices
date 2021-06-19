#include <algorithm>
#include <stdint.h>
#include "ITimerService.h"

#ifdef ITIMERSERVICE_H
namespace EmbeddedIOServices
{	
	void ITimerService::ScheduleFirstTaskInList()
	{
		//remove all unscheduled tasks
		std::forward_list<Task *>::iterator next = _taskList.begin();
		while(next != _taskList.end() && (*next)->Scheduled == false)
		{
			if((*next)->DeleteAfterExecution)
				delete *next;
			_taskList.pop_front();
			next = _taskList.begin();
		}

		//schedule first task
		if(next != _taskList.end())
			ScheduleCallBack((*next)->Tick);
	}

	void ITimerService::ReturnCallBack(void)
	{
		//skip all unscheduled tasks
		std::forward_list<Task *>::iterator next = _taskList.begin();;
		while(next != _taskList.end() && (*next)->Scheduled == false)
			next++;

		//execute all tasks that are ready
		while (next != _taskList.end() && TickLessThanEqualToTick((*next)->Tick, GetTick()))
		{
			(*next)->CallBack();
			(*next)->Scheduled = false;
			next++;
		}

		if(next != _taskList.end())
		 	ScheduleCallBack((*next)->Tick);
	}

	void ITimerService::ScheduleCallBack(std::function<void()> callBack, uint32_t tick)
	{
		Task *taskToSchedule = new Task(callBack);
		taskToSchedule->DeleteAfterExecution = true;
		
		ScheduleTask(taskToSchedule, tick);
	}

	void ITimerService::ScheduleTask(Task *task, uint32_t tick)
	{
		_taskList.remove(task);

		//find position to place Task
		std::forward_list<Task *>::iterator before = _taskList.before_begin();
		std::forward_list<Task *>::iterator next = before;
		next++;
		while(next != _taskList.end())
		{
			if((*next)->Scheduled && TickLessThanTick(tick, (*next)->Tick))
				break;

			before = next++;
		}

		task->Tick = tick;
		task->Scheduled = true;
		_taskList.insert_after(before, task);

		ScheduleFirstTaskInList();
	}

	void ITimerService::UnScheduleTask(Task *task)
	{
		_taskList.remove(task);
		ScheduleFirstTaskInList();
	}
}
#endif
