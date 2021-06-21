#include <stdint.h>
#include <math.h>
#include <functional>
#include "Stm32duinoTimerService.h"
#include "Stm32duinoConf.h"

using namespace EmbeddedIOServices;

namespace Stm32
{	
	Stm32duinoTimerService::Stm32duinoTimerService(TimerIndex index)
	{	
		if(_timFrequencyLocked[index])
			return;
		_timFrequencyLocked[index] = true;

		EnableTimerInterrupts(index);
		_hardwareTimer[index]->setMode(1, TIMER_OUTPUT_COMPARE);
		TimInit(index, 1, 0xFFFF);

		TIM = TimIndexToTIM(index);
		_timCallBack[index] = [this]() { this->TimerInterrupt(); };
		
		//set ticks per second
		_ticksPerSecond = HAL_RCC_GetSysClockFreq();
		
		//enable DWT-CYCCNT
		CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
		DWT->CTRL |= 1; 
		//factor in latency
		volatile uint32_t interruptTick;
		volatile uint32_t latencyTick;
		interruptTick = this->GetTick();
		latencyTick = this->GetTick();
		const uint32_t getTickCompensation = latencyTick - interruptTick;
		_interrupt = [this, &interruptTick]() { interruptTick = this->GetTick(); };

		//get overall latency
		interruptTick = 0;
		latencyTick = GetTick() + 10000;
		ScheduleCallBack(latencyTick);
		while(interruptTick == 0) ;
		_latency = interruptTick - latencyTick;

		//remove overhead latency
		interruptTick = 0;
		latencyTick = GetTick() + 10000;
		ScheduleCallBack(latencyTick);
		while(interruptTick == 0) ;
		_latency += interruptTick - latencyTick - getTickCompensation;
		
		//set minimum tick
		interruptTick = 0;
		latencyTick = GetTick() + 10000;
		interruptTick = this->GetTick();
		ScheduleCallBack(latencyTick);
		latencyTick = this->GetTick();
		_minTick = latencyTick - interruptTick - getTickCompensation + _latency;
		
		//set minimum tick latency
		interruptTick = 0;
		latencyTick = GetTick() + _minTick;
		ScheduleCallBack(latencyTick);
		while(interruptTick == 0) ;
		_minTicklatency = interruptTick - latencyTick - getTickCompensation;

		//set return callback to interface
		_interrupt = [this]() { this->ReturnCallBack(); };
	}

	const uint32_t Stm32duinoTimerService::GetTick()
	{
		return DWT->CYCCNT;
	}

	void Stm32duinoTimerService::ScheduleCallBack(const uint32_t tick)
	{
		const uint32_t minTick = tick - _minTick;

		if(TickLessThanTick(minTick, DWT->CYCCNT))
		{
			const uint32_t callTick = tick - _minTicklatency;
			while(TickLessThanTick(DWT->CYCCNT, callTick)) ;
			_interrupt();
		}
		else
		{
			const uint32_t callTick = tick == _latency? 1 : tick - _latency;
			TIM->CCR1 = TIM->CNT + (callTick - DWT->CYCCNT);
			_callTick = callTick;
		}
	}
	
	void Stm32duinoTimerService::TimerInterrupt(void)
	{
		if(_callTick != 0 && TickLessThanTick(_callTick, DWT->CYCCNT))
		{
			_callTick = 0;
			_interrupt();
		}
	}

	void Stm32duinoTimerService::AttachInterrupt(std::function<void()> interrupt)
	{
		_interrupt = interrupt;
	}

	const uint32_t Stm32duinoTimerService::GetTicksPerSecond()
	{
		return _ticksPerSecond;
	}
}
