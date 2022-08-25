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
		while((begin = _taskList.begin()) != _taskList.end() && (*begin)->Scheduled == false)
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
		TaskList::iterator next = _taskList.begin();
		while(true)
		{
			if(next == _taskList.end())
				return;
			if((*next)->Scheduled == true)
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
				return;
		}
		ScheduleCallBack((*next)->ScheduledTick - _latency);
	}

	void ITimerService::ScheduleCallBack(callback_t callBack, tick_t tick)
	{
		Task *taskToSchedule = new Task(callBack, true);
		
		ScheduleTask(taskToSchedule, tick);
	}

	void ITimerService::ScheduleTask(Task *task, tick_t tick)
	{
#ifdef ALLOW_TASK_TO_SCHEDULE_IN_CALLBACK
		_scheduleRequestList.push_front(ScheduleRequest(task, tick));
#ifndef DO_NOT_FLUSH_ON_SCHEDULE
		FlushScheduleRequests();
#endif
	}

	void ITimerService::FlushScheduleRequests()
	{
		//only flush schedule request if a flush schedule request is not already running somewhere else. if it is running somewhere else, they will flush it for us
		if(!_scheduleLock)
		{
			_scheduleLock = true;

			RemoveRequestList::iterator removeRequest;
			while((removeRequest = _removeRequestList.begin()) != _removeRequestList.end())
			{
				_taskList.remove(*removeRequest);
				_removeRequestList.pop_front();
				(*removeRequest)->Scheduled = false;
			}

			ScheduleRequestList::iterator scheduleRequest;
			while((scheduleRequest = _scheduleRequestList.begin()) != _scheduleRequestList.end())
			{
				Task * const task = scheduleRequest->TaskToSchedule;
				const tick_t tick = scheduleRequest->Tick;
#endif
				//remove all unscheduled tasks and get begin and end
				const TaskList::iterator begin = RemoveUnscheduledTasksAndReturnBegin();
				const TaskList::iterator end = _taskList.end();

				//find current location 
				const TaskList::iterator currentLocation = std::find(begin, end, task);

				//find new location
				const TaskList::iterator newLocation = std::find_if(begin, end, [tick](Task *taskFind) {
					return TickLessThanTick(tick, taskFind->ScheduledTick);
				});

				//reschedule
				if(currentLocation != end)
				{
					TaskList::iterator afterCurrentLocation = currentLocation;
					afterCurrentLocation++;
					if(newLocation != currentLocation && newLocation != afterCurrentLocation)
					{
						if(TickLessThanTick(tick, task->ScheduledTick))
							task->ScheduledTick = tick;
						_taskList.insert(newLocation, task);
						_taskList.erase(currentLocation);
					}
					task->ScheduledTick = tick;
				}
				//schedule
				else
				{
					task->ScheduledTick = tick;
					task->Scheduled = true;
					_taskList.insert(newLocation, task);
				}
				
#ifdef ALLOW_TASK_TO_SCHEDULE_IN_CALLBACK
				_scheduleRequestList.pop_front();
			}

#endif
			ScheduleCallBack((*_taskList.begin())->ScheduledTick - _latency);
#ifdef ALLOW_TASK_TO_SCHEDULE_IN_CALLBACK

			_scheduleLock = false;

			//make sure we scheduled all tasks. if we interrupted after the while loop, there is a possibility a new task was added
			if(_scheduleRequestList.begin() != _scheduleRequestList.end() || _removeRequestList.begin() != _removeRequestList.end())
				FlushScheduleRequests();
		}
#endif
	}

	void ITimerService::UnScheduleTask(Task *task)
	{
#ifdef ALLOW_TASK_TO_SCHEDULE_IN_CALLBACK
		_removeRequestList.push_front(task);
#ifndef DO_NOT_FLUSH_ON_SCHEDULE
		FlushScheduleRequests();
#endif
#else
		_taskList.remove(task);
		task->Scheduled = false;
		if(_taskList.size() > 0)
			ScheduleCallBack((*_taskList.begin())->ScheduledTick - _latency);
#endif
	}
}
#endif
