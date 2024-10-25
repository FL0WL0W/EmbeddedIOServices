#include <stdint.h>
#include <math.h>
#include "CallBack.h"
#include "Stm32duinoTimerService.h"
#include "Stm32duinoConf.h"

using namespace EmbeddedIOServices;

namespace Stm32
{	
	Stm32duinoTimerService::Stm32duinoTimerService(TimerIndex index) :
		TIM(TimIndexToTIM(index)),
		_ticksPerSecond(HAL_RCC_GetSysClockFreq())

	{	
		if(_timFrequencyLocked[index])
			return;
		_timFrequencyLocked[index] = true;

		EnableTimerInterrupts(index);
		_hardwareTimer[index]->setMode(1, TIMER_OUTPUT_COMPARE);
		TimInit(index, 1, TIM_ARR_ARR);
		
		_timCallBack[index] = [this]() { this->TimerInterrupt(); };
				
		//enable DWT-CYCCNT
		CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
		DWT->CTRL |= 1; 

		Calibrate();
	}

	tick_t Stm32duinoTimerService::GetTick()
	{
		return DWT->CYCCNT;
	}

	void Stm32duinoTimerService::ScheduleCallBack(const tick_t tick)
	{
		__disable_irq();
		_callTick = tick;
		TIM->DIER |= TIM_IT_CC1;
		TIM->CCR1 = TIM->CNT + (tick - DWT->CYCCNT);
		if(TickLessThanTick(tick, DWT->CYCCNT))
			TIM->EGR = TIM_EGR_CC1G;
		__enable_irq();
	}
	
	void Stm32duinoTimerService::TimerInterrupt(void)
	{
		if(TickLessThanTick(_callTick - _latency, DWT->CYCCNT))
		{
			TIM->DIER &= ~TIM_IT_CC1;
			ReturnCallBack();
		}
	}

	uint32_t Stm32duinoTimerService::GetTicksPerSecond()
	{
		return _ticksPerSecond;
	}
}
