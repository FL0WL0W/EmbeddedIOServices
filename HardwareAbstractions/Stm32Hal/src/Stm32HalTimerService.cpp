#include <stdint.h>
#include <math.h>
#include <functional>
#include "Stm32HalTimerService.h"
#include "Stm32HalConf.h"

using namespace EmbeddedIOServices;

namespace Stm32
{	
	Stm32HalTimerService *_timer[TimerIndex::Num];

	Stm32HalTimerService::Stm32HalTimerService(TimerIndex index)
	{	
		if(_timFrequencyLocked[index])
			return;
		_timFrequencyLocked[index] = true;
		
		TIM_HandleTypeDef TIM_Handle = TimInit(index, 0, 0xFFFF);
		TIM = TIM_Handle.Instance;

		//Set Output Compare Channel 1 to timing
		TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
		TIM_OC_InitStruct.OCMode = TIM_OCMODE_TIMING;
		TIM_OC_InitStruct.OCPolarity = TIM_OCPOLARITY_HIGH;
		TIM_OC_InitStruct.Pulse = 0;
		HAL_TIM_OC_ConfigChannel(&TIM_Handle, &TIM_OC_InitStruct, TIM_CHANNEL_1);
				
		_timer[index] = this;

		//set ticks per second
		_ticksPerSecond = HAL_RCC_GetSysClockFreq();

		//enable DWT-CYCCNT
		CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
		DWT->CTRL |= 1; 

		Calibrate();
		}

	void Stm32HalTimerService::Calibrate()
	{
		_minTicks = 0;

		//get minimum number of ticks to trigger an interrupt
		while(true)
		{
			ScheduleCallBack(GetTick());
			uint16_t count = 0;
			while(count++ < _minTicks) ;
			if(!(TIM->DIER & TIM_IT_CC1))
				break;
			_minTicks++;
		}
		
		ITimerService::Calibrate();
	}

	const tick_t Stm32HalTimerService::GetTick()
	{
		return DWT->CYCCNT;
	}

	void Stm32HalTimerService::ScheduleCallBack(const tick_t tick)
	{
		_callTick = tick;
		TIM->DIER |= TIM_IT_CC1;
		__disable_irq();
		const tick_t ticks = _callTick - DWT->CYCCNT;
		if(ticks - _minTicks > 0x80000000)
			TIM->CCR1 = TIM->CNT + _minTicks;
		else
			TIM->CCR1 = TIM->CNT + ticks;
		__enable_irq();
	}
	
	void Stm32HalTimerService::TimerInterrupt(void)
	{
		if(TickLessThanTick(_callTick, DWT->CYCCNT))
		{
			TIM->DIER &= ~TIM_IT_CC1;
			ReturnCallBack();
		}
	}

	const tick_t Stm32HalTimerService::GetTicksPerSecond()
	{
		return _ticksPerSecond;
	}
}