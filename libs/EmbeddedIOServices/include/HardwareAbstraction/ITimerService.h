#include "ICallBack.h"
#include "stdint.h"

#ifndef ITIMERSERVICE_H
#define ITIMERSERVICE_H

#define TIMERSERVICE_MAX_STACK_SIZE 256

namespace HardwareAbstraction
{
	class Task
	{
	public:
		Task() {}
		Task(void(*callBack)(void *), void *parameters, bool deleteOnExecution)
		{
			CallBackInstance = new CallBack(callBack, parameters);
			DeleteOnExecution = deleteOnExecution;
		}
		Task(ICallBack *callBack, bool deleteOnExecution)
		{
			CallBackInstance = callBack;
			DeleteOnExecution = deleteOnExecution;
		}

		void Execute()
		{
			CallBackInstance->Execute();
		}

		ICallBack *CallBackInstance;
		bool DeleteOnExecution;
		//only let TimerService edit these values
		bool Scheduled = false;
		uint32_t Tick;
	};

	class ITimerService
	{
	protected:
		void SortCallBackStack();
		virtual void ScheduleCallBack(const uint32_t tick) = 0;
	public:
#if TIMERSERVICE_MAX_STACK_SIZE <= 2^8
		uint8_t StackSize = 0;
#elif TIMERSERVICE_MAX_STACK_SIZE <= 2^16
		uint16_t StackSize = 0;
#elif TIMERSERVICE_MAX_STACK_SIZE <= 2^32
		uint32_t StackSize = 0;
#endif
		Task *CallBackStackPointer[TIMERSERVICE_MAX_STACK_SIZE];

		virtual const uint32_t GetTick() = 0;
		virtual const uint32_t GetTicksPerSecond() = 0;

		void ReturnCallBack(void);
		Task *ScheduleTask(void(*)(void *), void *, const uint32_t, const bool);
		const bool ScheduleTask(Task *, const uint32_t);
		const bool ReScheduleTask(Task *, const uint32_t);
		const bool UnScheduleTask(Task *);
		
		const uint32_t GetElapsedTick(const uint32_t);
		const float GetElapsedTime(const uint32_t);
	};
}
#endif
