#include <algorithm>
#include <stdint.h>
#include "ITimerService.h"
#include <cstdio>

#ifdef ITIMERSERVICE_H
namespace EmbeddedIOServices
{	
	void ITimerService::Calibrate()
	{
		// Startup only: interrupts must be enabled, and this timer must have no
		// application tasks, pending updates, or other producers during calibration.
		constexpr unsigned BackgroundTaskCount = 100; // Representative loaded task map.
		constexpr unsigned NewTaskCount = 3;
		constexpr unsigned SampleCount = 16;
		// Four reschedules + three inserts + the probe fill the eight-slot buffer.
		constexpr unsigned RescheduleCount = 4;
		const tick_t frequency = GetTicksPerSecond();
		const tick_t probeLead = std::max<tick_t>(frequency / 1000U, 1U);
		const tick_t spacing = std::max<tick_t>(frequency / 10000U, 1U);
		const tick_t backgroundLead = std::max<tick_t>(frequency / 4U, probeLead * 64U);

		// Do not let calibration's busy-wait hide the latency being measured.
		_latency = 0;
		_minTick = 0;
		volatile bool probeComplete = false;
		Task probe([&]() { probeComplete = true; });
		Task *background[BackgroundTaskCount];
		Task *newTasks[NewTaskCount];
		for (unsigned i = 0; i < BackgroundTaskCount; ++i)
			background[i] = new Task([]() {});
		for (unsigned i = 0; i < NewTaskCount; ++i)
			newTasks[i] = new Task([]() {});

		// Build and remove the fixture through the ISR, just like application tasks.
		// Deadlines stay far ahead so the background callbacks do not affect samples.
		const auto populateList = [&]() {
			const tick_t firstTick = GetTick() + backgroundLead;
			for (unsigned i = 0; i < BackgroundTaskCount; ++i)
			{
				ScheduleTask(background[i], firstTick + i * spacing);
				while (!background[i]->Scheduled) ;
			}
			return firstTick;
		};
		const auto removeFixtureTask = [&](Task *task) {
			if (task->Scheduled)
			{
				UnScheduleTask(task);
				// These fixture tasks are still scheduled far into the future, so
				// the flag clears when the ISR applies their cancellation.
				while (task->Scheduled) ;
			}
		};
		const auto clearList = [&]() {
			for (unsigned i = 0; i < BackgroundTaskCount; ++i)
				removeFixtureTask(background[i]);
			for (unsigned i = 0; i < NewTaskCount; ++i)
				removeFixtureTask(newTasks[i]);
		};

		// Measure normal deadline wakeup: programming delay + ISR entry + reaching
		// the task's execution check. Queue processing happens in an earlier IRQ.
		populateList();
		tick_t worstWakeup = 0;
		for (unsigned sample = 0; sample < SampleCount; ++sample)
		{
			probeComplete = false;
			const tick_t deadline = GetTick() + probeLead;
			ScheduleTask(&probe, deadline);
			while (!probeComplete) ;
			worstWakeup = std::max<tick_t>(worstWakeup, probe.ExecutedTick - deadline);
		}
		clearList();

		// Measure a loaded request wakeup. The last request is an overdue probe,
		// so its execution timestamp includes the preceding seven updates.
		tick_t worstLoadedWakeup = 0;
		for (unsigned sample = 0; sample < SampleCount; ++sample)
		{
			const tick_t firstTick = populateList();
			probeComplete = false;
			for (unsigned i = 0; i < RescheduleCount; ++i)
			{
				// Move near-tail deadlines beyond the tail to exercise extract/reinsert.
				TaskUpdate update = { background[BackgroundTaskCount - 2U - i],
					firstTick + (BackgroundTaskCount + i) * spacing };
				_taskUpdateBuffer.Push(update, 1U);
			}
			for (unsigned i = 0; i < NewTaskCount; ++i)
			{
				TaskUpdate update = { newTasks[i],
					firstTick + (BackgroundTaskCount + RescheduleCount + i) * spacing };
				_taskUpdateBuffer.Push(update, 1U);
			}
			TaskUpdate probeUpdate = { &probe, GetTick() };
			_taskUpdateBuffer.Push(probeUpdate, 1U);

			const tick_t started = GetTick();
			ScheduleCallBack(started);
			while (!probeComplete) ;
			worstLoadedWakeup = std::max<tick_t>(
				worstLoadedWakeup, probe.ExecutedTick - started);
			clearList();
		}

		for (unsigned i = 0; i < BackgroundTaskCount; ++i)
			delete background[i];
		for (unsigned i = 0; i < NewTaskCount; ++i)
			delete newTasks[i];

		const tick_t lead = worstWakeup + 1U;
		_latency = static_cast<uint16_t>(std::min<tick_t>(lead, 0xFFFFU));

		const tick_t window = std::max<tick_t>(lead, worstLoadedWakeup);
		_minTick = static_cast<uint16_t>(std::min<tick_t>(
			window + 1U, 0xFFFFU));
		printf("Latency: %u, MinTick: %u\n", _latency, _minTick);
		// Both fields are 16-bit: saturation means the measured budget will not fit.
	}

	void ITimerService::ReturnCallBack()
	{
		//skip all unscheduled tasks
		TaskMap::iterator next;
		while ((next = _taskMap.begin()) != _taskMap.end() && TickLessThanEqualToTick(next->first - _minTick, GetTick()))
		{
			while(TickLessThanTick(next->second->ExecutedTick = GetTick(), next->first)) ;
			next->second->Scheduled = false;
			next->second->CallBack();
			if((next->second)->DeleteAfterExecution)
				delete next->second;

			_taskMap.erase(next);
		}

		//handle task updates
		uint8_t command;
		TaskUpdate taskUpdate;
		while((command = _taskUpdateBuffer.Pop(taskUpdate)) != 0U)
		{
			// find and remove task from multimap
			const auto range = _taskMap.equal_range(taskUpdate.task->ScheduledTick);
			const auto currentLocation = std::find_if(range.first, range.second, [taskUpdate](const auto& entry) { return entry.second == taskUpdate.task; });
			
			if(command == 1U)
			{				
				//schedule
				taskUpdate.task->ScheduledTick = taskUpdate.tick;
				//check if we should go ahead and execute the task
				if(TickLessThanEqualToTick(taskUpdate.task->ScheduledTick - _minTick, GetTick()))
				{
					while(TickLessThanTick(taskUpdate.task->ExecutedTick = GetTick(), taskUpdate.task->ScheduledTick)) ;
					taskUpdate.task->Scheduled = false;
					taskUpdate.task->CallBack();
					if(taskUpdate.task->DeleteAfterExecution)
						delete taskUpdate.task;

					if (currentLocation != range.second)
						_taskMap.erase(currentLocation);
					continue;
				}
				//otherwise schedule it
				taskUpdate.task->Scheduled = true;
				if (currentLocation != range.second)
				{
					auto node = _taskMap.extract(currentLocation);
					node.key() = taskUpdate.tick;
					_taskMap.insert(std::move(node));
				}
				else
				{
					_taskMap.insert({taskUpdate.tick, taskUpdate.task});
				}
				continue;
			}
			if(command == 2U)
			{
				_taskMap.erase(currentLocation);
				taskUpdate.task->Scheduled = false;
			}
		}
		if(_taskMap.size() > 0)
			ScheduleCallBack(_taskMap.begin()->first - _latency);
	}

	void ITimerService::ScheduleCallBack(callback_t callBack, tick_t tick)
	{
		Task *task = new Task(callBack, true);
		
		TaskUpdate taskUpdate = { task, tick };
		if(_taskUpdateBuffer.Push(taskUpdate, 1U))
			ScheduleCallBack(GetTick());
		else
			delete task;
	}

	void ITimerService::ScheduleTask(Task *task, tick_t tick)
	{
		TaskUpdate taskUpdate = { task, tick };
		_taskUpdateBuffer.Push(taskUpdate, 1U);
		ScheduleCallBack(GetTick());
	}

	void ITimerService::UnScheduleTask(Task *task)
	{
		TaskUpdate taskUpdate = { task, 0 };
		_taskUpdateBuffer.Push(taskUpdate, 2U);
		ScheduleCallBack(GetTick());
	}
}
#endif
