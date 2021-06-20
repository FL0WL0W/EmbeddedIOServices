#include <stdint.h>
#include <math.h>
#include <functional>
#include "Stm32HalTimerService.h"
#include "Stm32HalConf.h"

using namespace EmbeddedIOServices;

namespace Stm32
{	
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
		
		//Enable Compare Channel 1 Intterupt
		__HAL_TIM_ENABLE_IT(&TIM_Handle, TIM_IT_CC1);
		_timCallBackPtr = &_timCallBack[index];
		
		//set ticks per second
		_ticksPerSecond = HAL_RCC_GetSysClockFreq();

		//enable DWT-CYCCNT
		CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
		DWT->CTRL |= 1; 

		const uint32_t latencyTick = GetTick() + 1000;
		_interrupt = [this, latencyTick]() { this->_latency = DWT->CYCCNT - latencyTick; };
		ScheduleCallBack(latencyTick);

		//set return callback to interface
		_interrupt = [this]() { this->ReturnCallBack(); };
	}

	const uint32_t Stm32HalTimerService::GetTick()
	{
		return DWT->CYCCNT;
	}

	void Stm32HalTimerService::ScheduleCallBack(const uint32_t tick)
	{
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
	
	void Stm32HalTimerService::TimerInterrupt(void)
	{
		if(TickLessThanTick(_callTick, DWT->CYCCNT))
		{
			*_timCallBackPtr = 0;
			_interrupt();
		}
	}

	void Stm32HalTimerService::AttachInterrupt(std::function<void()> interrupt)
	{
		_interrupt = interrupt;
	}

	const uint32_t Stm32HalTimerService::GetTicksPerSecond()
	{
		return _ticksPerSecond;
	}
}