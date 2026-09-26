#include <algorithm>
#include <stdint.h>
#include "ITimerService.h"

#ifdef ITIMERSERVICE_H
namespace EmbeddedIOServices
{	
	void ITimerService::Calibrate()
	{
		//save taskList
		TaskList taskList = _taskList;

		_latency = 0;
		_minTick = 0;
		
		//setup task
		Task task([]() {});
		Task task2([]() {});

		//get minimum tick to add that schedules far enough in advance
		ScheduleTask(&task, GetTick());
		while(task.Scheduled) ;
		const uint16_t minTickAddTask = task.ExecutedTick - task.ScheduledTick + 1;//add 1 for timers where tickrate != clockrate

		//get latency
		ScheduleTask(&task, GetTick() + minTickAddTask);
		while(task.Scheduled) ;
		_latency = task.ExecutedTick - task.ScheduledTick + 1;//add 1 for timers where tickrate != clockrate

		//get min tick
		//this includes a single unscheduled task in the front of the list
		_taskList.push_back(&task2);
		task2.Scheduled = true;
		ScheduleCallBack(task2.ScheduledTick = GetTick());
		while(task2.Scheduled) ;
		_minTick = task2.ExecutedTick - task2.ScheduledTick + 1;//add 1 for timers where tickrate != clockrate

		//return taskList;
		_taskList = taskList;

		const TaskList::iterator begin = RemoveUnscheduledTasksAndReturnBegin();
		if(begin != _taskList.end())
			ScheduleCallBack((*begin)->ScheduledTick - _latency);
	}

	TaskList::iterator ITimerService::RemoveUnscheduledTasksAndReturnBegin()
	{
		//remove all unscheduled tasks
		TaskList::iterator begin;
		while((begin = _taskList.begin()) != _taskList.end() && !(*begin)->Scheduled)
		{
			if((*begin)->DeleteAfterExecution)
				delete *begin;
			_taskList.pop_front();
		}

		return begin;
	}

	void ITimerService::ReturnCallBack()
	{
		//skip all unscheduled tasks
		const bool taskListWasInUse = _taskListInUse;
		_taskListInUse = true;
		TaskList::iterator next = _taskList.begin();
		while(true)
		{
			if(next == _taskList.end())
			{
				_taskListInUse = taskListWasInUse;
				HandleTaskUpdates();
				return;
			}
			if((*next)->Scheduled)
				break;
			next++;
		}

		//execute all tasks that are ready
		while (TickLessThanEqualToTick((*next)->ScheduledTick - _minTick, GetTick()))
		{
			while(TickLessThanTick((*next)->ExecutedTick = GetTick(), (*next)->ScheduledTick)) ;
			(*next)->Scheduled = false;
			(*next)->CallBack();

			if(++next == _taskList.end())
			{
				_taskListInUse = taskListWasInUse;
				HandleTaskUpdates();
				return;
			}
		}
		ScheduleCallBack((*next)->ScheduledTick - _latency);
		_taskListInUse = taskListWasInUse;
		HandleTaskUpdates();
	}

	void ITimerService::ScheduleCallBack(callback_t callBack, tick_t tick)
	{
		Task *task = new Task(callBack, true);
		
		TaskUpdate taskUpdate = { task, tick };
		if(_taskUpdateBuffer.Push(taskUpdate, 1U))
			HandleTaskUpdates();
		else
			delete task;
	}

	void ITimerService::ScheduleTask(Task *task, tick_t tick)
	{
		TaskUpdate taskUpdate = { task, tick };
		_taskUpdateBuffer.Push(taskUpdate, 1U);
		
		HandleTaskUpdates();
	}

	void ITimerService::UnScheduleTask(Task *task)
	{
		TaskUpdate taskUpdate = { task, 0 };
		_taskUpdateBuffer.Push(taskUpdate, 2U);
		
		HandleTaskUpdates();
	}

	void ITimerService::HandleTaskUpdates()
	{
		if(_taskListInUse)
			return;

		uint8_t command;
		TaskUpdate taskUpdate;
		while((command = _taskUpdateBuffer.Pop(taskUpdate)) != 0U)
		{
			if(command == 1U)
			{
				_taskListInUse = true;

				//remove all unscheduled tasks and get begin and end
				const TaskList::iterator begin = RemoveUnscheduledTasksAndReturnBegin();
				const TaskList::iterator end = _taskList.end();

				//find current location 
				const TaskList::iterator currentLocation = std::find(begin, end, taskUpdate.task);
				
				//find new location
				const TaskList::iterator newLocation = std::find_if(begin, end, [taskUpdate](Task *taskFind) {
					return TickLessThanTick(taskUpdate.tick, taskFind->ScheduledTick);
				});

				//reschedule
				if(currentLocation != end)
				{
					TaskList::iterator afterCurrentLocation = currentLocation;
					afterCurrentLocation++;
					if(newLocation != currentLocation && newLocation != afterCurrentLocation)
					{
						if(TickLessThanTick(taskUpdate.tick, taskUpdate.task->ScheduledTick))
							taskUpdate.task->ScheduledTick = taskUpdate.tick;
						_taskList.insert(newLocation, taskUpdate.task);
						_taskList.erase(currentLocation);
					}
					taskUpdate.task->ScheduledTick = taskUpdate.tick;
				}
				//schedule
				else
				{
					taskUpdate.task->ScheduledTick = taskUpdate.tick;
					taskUpdate.task->Scheduled = true;
					_taskList.insert(newLocation, taskUpdate.task);
				}

				_taskListInUse = false;

				ScheduleCallBack((*_taskList.begin())->ScheduledTick - _latency);
			}
			else if(command == 2U)
			{
				_taskListInUse = true;

				_taskList.remove(taskUpdate.task);

				_taskListInUse = false;
				
				taskUpdate.task->Scheduled = false;
				if(_taskList.size() > 0)
					ScheduleCallBack((*_taskList.begin())->ScheduledTick - _latency);
			}
		}
	}
}
#endif
