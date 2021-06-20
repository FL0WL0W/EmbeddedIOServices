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
		
		//set ticks per second
		_ticksPerSecond = HAL_RCC_GetSysClockFreq();
		
		//enable DWT-CYCCNT
		CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
		DWT->CTRL |= 1; 

		//enable DWT-CYCCNT
		CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
		DWT->CTRL |= 1; 

		const uint32_t latencyTick = GetTick() + 1000;
		_interrupt = [this, latencyTick]() { this->_latency = DWT->CYCCNT - latencyTick; };
		ScheduleCallBack(latencyTick);

		//set return callback to interface
		_interrupt = [this]() { this->ReturnCallBack(); };
	}

	const uint32_t Stm32duinoTimerService::GetTick()
	{
		return DWT->CYCCNT;
	}

	void Stm32duinoTimerService::ScheduleCallBack(const uint32_t tick)
	{
		if(TickLessThanTick(tick, DWT->CYCCNT))
		_callTick = tick - _latency;
		if(TickLessThanTick(_callTick, DWT->CYCCNT))
		{
			while(TickLessThanTick(DWT->CYCCNT, tick)) ;
			_interrupt();
		}
		else
		{
			*_timCallBackPtr = [this]() { this->TimerInterrupt(); };
			TIM->CCR1 = TIM->CNT + (_callTick - DWT->CYCCNT);
		}	
	}
	
	void Stm32duinoTimerService::TimerInterrupt(void)
	{
		if(TickLessThanTick(_callTick, DWT->CYCCNT))
		{
			*_timCallBackPtr = 0;
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
