#include <algorithm>
#include <stdint.h>
#include "ITimerService.h"

#ifdef ITIMERSERVICE_H
namespace EmbeddedIOServices
{	
	void ITimerService::Calibrate()
	{
		// Startup only: interrupts must be enabled, and this timer must have no
		// application tasks, pending updates, or other producers during calibration.
		constexpr unsigned BackgroundTaskCount = 100; // Representative loaded task map.
		constexpr unsigned NewTaskCount = 4;
		constexpr unsigned SampleCount = 16;
		// Four reschedules + four inserts fill the eight-slot buffer.
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

		const auto queueUpdateBatch = [&](tick_t firstTick) {
			for (unsigned i = 0; i < RescheduleCount; ++i)
			{
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
		};

		// Estimate the batch duration ONLY to choose deadline offsets below.
		// This elapsed time is not used as the busy-wait window. The last insert's
		// Scheduled flag confirms that the ISR has processed the entire batch.
		const tick_t firstTick = populateList();
		queueUpdateBatch(firstTick);
		const tick_t started = GetTick();
		ScheduleCallBack(started);
		while (!newTasks[NewTaskCount - 1U]->Scheduled) ;
		const tick_t batchSpan = std::max<tick_t>(GetTick() - started, 1U);
		clearList();

		// With minTick still zero, place the probe in the active map BEFORE filling
		// the buffer. Sweep the early kick across the batch: a deadline crossed by
		// an update is noticed by ExecuteAwaitingTasks() immediately after it.
		// Measure execution lateness, not time spent waiting behind queued requests.
		tick_t worstUpdateLateness = 0;
		const tick_t deadlineLead = std::max<tick_t>(
			probeLead, 2U * (worstWakeup + batchSpan + 1U));
		for (unsigned sample = 0; sample < SampleCount; ++sample)
		{
			const tick_t firstTick = populateList();
			probeComplete = false;
			const tick_t deadline = GetTick() + deadlineLead;
			ScheduleTask(&probe, deadline);
			while (!probe.Scheduled && !probeComplete) ;
			queueUpdateBatch(firstTick);

			// Sweep from a near-immediate deadline through and beyond the batch.
			const tick_t kickLead = 1U +
				static_cast<tick_t>((static_cast<uint64_t>(batchSpan + worstWakeup) * sample) /
					(SampleCount - 1U));
			const tick_t kickAt = deadline - kickLead;
			while (TickLessThanTick(GetTick(), kickAt)) ;
			ScheduleCallBack(GetTick());
			while (!probeComplete) ;
			if (TickLessThanEqualToTick(deadline, probe.ExecutedTick))
				worstUpdateLateness = std::max<tick_t>(
					worstUpdateLateness, probe.ExecutedTick - deadline);
			clearList();
		}

		for (unsigned i = 0; i < BackgroundTaskCount; ++i)
			delete background[i];
		for (unsigned i = 0; i < NewTaskCount; ++i)
			delete newTasks[i];

		const tick_t lead = worstWakeup + 1U;
		_latency = static_cast<uint16_t>(std::min<tick_t>(lead, 0xFFFFU));

		const tick_t window = std::max<tick_t>(lead, worstUpdateLateness);
		_minTick = static_cast<uint16_t>(std::min<tick_t>(
			window + 1U, 0xFFFFU));
		// Both fields are 16-bit: saturation means the measured budget will not fit.
	}

	void ITimerService::ReturnCallBack()
	{
		ExecuteAwaitingTasks();

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
			}
			else if(command == 2U)
			{
				if (currentLocation != range.second)
					_taskMap.erase(currentLocation);
				taskUpdate.task->Scheduled = false;
			}

			ExecuteAwaitingTasks();
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
