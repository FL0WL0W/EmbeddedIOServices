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
		//Get Tick Compensation
		volatile tick_t interruptTick;
		volatile tick_t latencyTick;
		interruptTick = this->GetTick();
		latencyTick = this->GetTick();
		const uint16_t getTickCompensation = latencyTick - interruptTick;
		
		//setup interrupt
		std::function<void()> callBack = [this, &interruptTick]() { interruptTick = this->GetTick(); };

		//get minimum tick to add that schedules far enough in advance
		interruptTick = 0;
		latencyTick = GetTick();
		ScheduleCallBack(callBack, latencyTick);
		while(interruptTick == 0) ;
		const uint16_t minTickAdd = interruptTick - latencyTick - getTickCompensation;

		//get latency
		interruptTick = 0;
		latencyTick = GetTick() + minTickAdd;
		ScheduleCallBack(callBack, latencyTick);
		while(interruptTick == 0) ;
		_latency = interruptTick - latencyTick - getTickCompensation;

		//return taskList;
		_taskList = taskList;

		const TaskList::iterator begin = RemoveUnscheduledTasksAndReturnBegin();
		if(begin != _taskList.end())
			ScheduleCallBack((*begin)->Tick - _latency);
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

	void ITimerService::ReturnCallBack(void)
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
		while (TickLessThanEqualToTick((*next)->Tick - _latency, GetTick()))
		{
			while(TickLessThanTick(GetTick(), (*next)->Tick)) ;
			(*next)->Scheduled = false;
			(*next)->Tick = GetTick();
			(*next)->CallBack();

			if(++next == _taskList.end())
				return;
		}

		ScheduleCallBack((*next)->Tick - _latency);
	}

	void ITimerService::ScheduleCallBack(std::function<void()> callBack, tick_t tick)
	{
		Task *taskToSchedule = new Task(callBack);
		taskToSchedule->DeleteAfterExecution = true;
		
		ScheduleTask(taskToSchedule, tick);
	}

	void ITimerService::ScheduleTask(Task *task, tick_t tick)
	{
#ifdef ALLOW_TASK_TO_SCHEDULE_IN_CALLBACK
		_scheduleRequestList.push_front(ScheduleRequest(task, tick));
		FlushScheduleRequests();
	}

	void ITimerService::FlushScheduleRequests()
	{
		//only flush schedule request if a flush schedule request is not already running somewhere else. if it is running somewhere else, they will flush it for us
		if(!_scheduleLock)
		{
			_scheduleLock = true;

			std::forward_list<Task *>::iterator removeRequest;
			while((removeRequest = _removeRequestList.begin()) != _removeRequestList.end())
			{
				_taskList.remove(*removeRequest);
				_removeRequestList.pop_front();
			}

			std::forward_list<ScheduleRequest>::iterator scheduleRequest;
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
				const TaskList::iterator newLocation = std::find_if(begin, end, [tick](Task *task) {
					return TickLessThanTick(tick, task->Tick);
				});

				if(currentLocation != end)
				{
					TaskList::iterator afterCurrentLocation = currentLocation;
					afterCurrentLocation++;
					if(newLocation != currentLocation && newLocation != afterCurrentLocation)
					{
						if(TickLessThanTick(tick, task->Tick))
							task->Tick = tick;
						_taskList.insert(newLocation, task);
						_taskList.erase(currentLocation);
					}
					task->Tick = tick;
				}
				else
				{
					task->Scheduled = true;
					task->Tick = tick;
					_taskList.insert(newLocation, task);
				}
				
					
#ifdef ALLOW_TASK_TO_SCHEDULE_IN_CALLBACK
				_scheduleRequestList.pop_front();
			}

#endif
			ScheduleCallBack((*_taskList.begin())->Tick - _latency);
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
		FlushScheduleRequests();
#else
		_taskList.remove(task);
		ScheduleCallBack((*_taskList.begin())->Tick - _latency);
#endif
	}
}
#endif
