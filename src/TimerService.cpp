#include <algorithm>
#include <stdint.h>
#include "ITimerService.h"

#ifdef ITIMERSERVICE_H
namespace EmbeddedIOServices
{	
	ITimerService::ITimerService()
	{
		_taskList = new std::forward_list<Task *>();
	}

	ITimerService::~ITimerService()
	{
		delete _taskList;
	}

	void ITimerService::Calibrate()
	{
		//save taskList
		std::forward_list<Task *> *taskList = _taskList;
		_taskList = new std::forward_list<Task *>();

		//calibrate best case latency
		_latency = 0;
		volatile bool called = false;
		volatile uint32_t latency = GetTick() + 10000;
		ScheduleCallBack([this, &called, &latency]() { latency = GetTick() - latency; called = true; }, latency);
		while(!called);
		_latency = latency;

		//return taskList;
		delete _taskList;
		_taskList = taskList;

		ScheduleFirstTaskInList();
	}

	void ITimerService::ScheduleFirstTaskInList()
	{
		//remove all unscheduled tasks
		std::forward_list<Task *>::iterator next = _taskList->begin();
		while(next != _taskList->end() && (*next)->Scheduled == false)
		{
			if((*next)->DeleteAfterExecution)
				delete *next;
			_taskList->pop_front();
			next = _taskList->begin();
		}

		//schedule first task
		if(next != _taskList->end())
			ScheduleCallBack((*next)->Tick - _latency);
	}

	void ITimerService::ReturnCallBack(void)
	{
		//skip all unscheduled tasks
		std::forward_list<Task *>::iterator next = _taskList->begin();;
		while(next != _taskList->end() && (*next)->Scheduled == false)
			next++;

		//execute all tasks that are ready
		while (next != _taskList->end() && TickLessThanEqualToTick((*next)->Tick, GetTick()))
		{
			(*next)->Scheduled = false;
			(*next)->Tick = GetTick();
			(*next)->CallBack();
			next++;
		}

		if(next != _taskList->end())
		 	ScheduleCallBack((*next)->Tick - _latency);
	}

	void ITimerService::ScheduleCallBack(std::function<void()> callBack, uint32_t tick)
	{
		Task *taskToSchedule = new Task(callBack);
		taskToSchedule->DeleteAfterExecution = true;
		
		ScheduleTask(taskToSchedule, tick);
	}

	void ITimerService::ScheduleTask(Task *task, uint32_t tick)
	{
#ifdef ALLOW_TASK_TO_SCHEDULE_IN_CALLBACK
		if(tick == 0)
			tick++;

		_scheduleRequestList.push_back(ScheduleRequest(task, tick));
		FlushScheduleRequests();
	}

	void ITimerService::FlushScheduleRequests()
	{
		//only flush schedule request if a flush schedule request is not already running somewhere else. if it is running somewhere else, they will flush it for us
		if(!_scheduleLock)
		{
			_scheduleLock = true;

			std::list<ScheduleRequest>::iterator scheduleRequest;
			while((scheduleRequest = _scheduleRequestList.begin()) != _scheduleRequestList.end())
			{
				Task * const task = scheduleRequest->TaskToSchedule;
				const uint32_t tick = scheduleRequest->Tick
#endif
				_taskList->remove(task);

#ifdef ALLOW_TASK_TO_SCHEDULE_IN_CALLBACK
				//if this is to reschedule task, add it back to the list
				if(tick != 0)
				{
#endif
					//find position to place Task
					std::forward_list<Task *>::iterator before = _taskList->before_begin();
					std::forward_list<Task *>::iterator next = before;
					next++;
					while(next != _taskList->end())
					{
						if((*next)->Scheduled && TickLessThanTick(tick, (*next)->Tick))
							break;

						before = next++;
					}

					task->Tick = tick;
					task->Scheduled = true;
					_taskList->insert_after(before, task);
#ifdef ALLOW_TASK_TO_SCHEDULE_IN_CALLBACK
				}

				_scheduleRequestList.pop_front();
			}
#endif

			ScheduleFirstTaskInList();

#ifdef ALLOW_TASK_TO_SCHEDULE_IN_CALLBACK
			_scheduleLock = false;

			//make sure we scheduled all tasks. if we interrupted after the while loop, there is a possibility a new task was added
			if(_scheduleRequestList.begin() != _scheduleRequestList.end())
				FlushScheduleRequests();
		}
#endif
	}

	void ITimerService::UnScheduleTask(Task *task)
	{
#ifdef ALLOW_TASK_TO_SCHEDULE_IN_CALLBACK
		_scheduleRequestList.push_back(ScheduleRequest(task, 0));
		FlushScheduleRequests();
#else
		_taskList->remove(task);
		ScheduleFirstTaskInList();
#endif
	}
}
#endif
