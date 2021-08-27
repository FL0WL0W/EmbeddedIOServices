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
		
		TIM_HandleTypeDef TIM_Handle = TimInit(index, 0, TIM_ARR_ARR);
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

	const tick_t Stm32HalTimerService::GetTick()
	{
		return DWT->CYCCNT;
	}

	void Stm32HalTimerService::ScheduleCallBack(const tick_t tick)
	{
		__disable_irq();
		_callTick = tick;
		TIM->DIER |= TIM_IT_CC1;
		TIM->CCR1 = TIM->CNT + (_callTick - DWT->CYCCNT);
		if(TickLessThanTick(_callTick, DWT->CYCCNT))
			TIM->EGR = TIM_EGR_CC1G;
		__enable_irq();
	}
	
	void Stm32HalTimerService::TimerInterrupt(void)
	{
		if(TickLessThanEqualToTick(_callTick, DWT->CYCCNT))
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