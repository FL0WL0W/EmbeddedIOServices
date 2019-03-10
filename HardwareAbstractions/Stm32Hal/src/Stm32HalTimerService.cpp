#include <stdint.h>
#include <math.h>
#include <functional>
#include "Stm32HalTimerService.h"
#include "Stm32HalConf.h"

namespace Stm32
{
#ifdef TIM1
	Stm32HalTimerService *TimerService1 = 0;
#endif
#ifdef TIM2
	Stm32HalTimerService *TimerService2 = 0;
#endif
#ifdef TIM3
	Stm32HalTimerService *TimerService3 = 0;
#endif
#ifdef TIM4
	Stm32HalTimerService *TimerService4 = 0;
#endif
#ifdef TIM5
	Stm32HalTimerService *TimerService5 = 0;
#endif
#ifdef TIM6
	Stm32HalTimerService *TimerService6 = 0;
#endif
#ifdef TIM7
	Stm32HalTimerService *TimerService7 = 0;
#endif
#ifdef TIM8
	Stm32HalTimerService *TimerService8 = 0;
#endif
#ifdef TIM9
	Stm32HalTimerService *TimerService9 = 0;
#endif
#ifdef TIM10
	Stm32HalTimerService *TimerService10 = 0;
#endif
#ifdef TIM11
	Stm32HalTimerService *TimerService11 = 0;
#endif
#ifdef TIM12
	Stm32HalTimerService *TimerService12 = 0;
#endif
#ifdef TIM13
	Stm32HalTimerService *TimerService13 = 0;
#endif
#ifdef TIM14
	Stm32HalTimerService *TimerService14 = 0;
#endif
	
	Stm32HalTimerService::Stm32HalTimerService(uint8_t timer, uint8_t compareRegister, uint32_t ticksPerSecond)
	{	
		switch (timer)
		{
#ifdef TIM1
		case 1:
			if (TIM1_Freq_Locked)
				return;
			TIM1_Freq_Locked = true;
			TimerService1 = this;
			TIM = TIM1;
			break;
#endif
#ifdef TIM2
		case 2:
			if (TIM2_Freq_Locked)
				return;
			TIM2_Freq_Locked = true;
			TimerService2 = this;
			TIM = TIM2;
			break;
#endif
#ifdef TIM3
		case 3:
			if (TIM3_Freq_Locked)
				return;
			TIM3_Freq_Locked = true;
			TimerService3 = this;
			TIM = TIM3;
			break;
#endif
#ifdef TIM4
		case 4:
			if (TIM4_Freq_Locked)
				return;
			TIM4_Freq_Locked = true;
			TimerService4 = this;
			TIM = TIM4;
			break;
#endif
#ifdef TIM5
		case 5:
			if (TIM5_Freq_Locked)
				return;
			TIM5_Freq_Locked = true;
			TimerService5 = this;
			TIM = TIM5;
			break;
#endif
#ifdef TIM6
		case 6:
			if (TIM6_Freq_Locked)
				return;
			TIM6_Freq_Locked = true;
			TimerService6 = this;
			TIM = TIM6;
			break;
#endif
#ifdef TIM7
		case 7:
			if (TIM7_Freq_Locked)
				return;
			TIM7_Freq_Locked = true;
			TimerService7 = this;
			TIM = TIM7;
			break;
#endif
#ifdef TIM8
		case 8:
			if (TIM8_Freq_Locked)
				return;
			TIM8_Freq_Locked = true;
			TimerService8 = this;
			TIM = TIM8;
			break;
#endif
#ifdef TIM9
		case 9:
			if (TIM9_Freq_Locked)
				return;
			TIM9_Freq_Locked = true;
			TimerService9 = this;
			TIM = TIM9;
			__HAL_RCC_TIM9_CLK_ENABLE();
			break;
#endif
#ifdef TIM10
		case 10:
			if (TIM10_Freq_Locked)
				return;
			TIM10_Freq_Locked = true;
			TimerService10 = this;
			TIM = TIM10;
			__HAL_RCC_TIM10_CLK_ENABLE();
			break;
#endif
#ifdef TIM11
		case 11:
			if (TIM11_Freq_Locked)
				return;
			TIM11_Freq_Locked = true;
			TimerService11 = this;
			TIM = TIM11;
			break;
#endif
#ifdef TIM12
		case 12:
			if (TIM12_Freq_Locked)
				return;
			TIM12_Freq_Locked = true;
			TimerService12 = this;
			TIM = TIM12;
			break;
#endif
#ifdef TIM13
		case 13:
			if (TIM13_Freq_Locked)
				return;
			TIM13_Freq_Locked = true;
			TimerService13 = this;
			TIM = TIM13;
			break;
#endif
#ifdef TIM14
		case 14:
			if (TIM14_Freq_Locked)
				return;
			TIM14_Freq_Locked = true;
			TimerService14 = this;
			TIM = TIM14;
			break;
#endif
		}

		EnableTimerInterrupts(timer);
		EnableTimerClock(timer);
		
		TIM_HandleTypeDef TIM_HandleStruct = {0};
		TIM_HandleStruct.Instance = TIM;

		//set prescaler
		uint32_t clockFrequency = HAL_RCC_GetSysClockFreq();
		TIM_HandleStruct.Init.Prescaler = clockFrequency / ticksPerSecond;
		_ticksPerSecond = clockFrequency / TIM_HandleStruct.Init.Prescaler;
		
		//tick overhead compensation
		//no optimizations = 110 instructions from TIM_CC_HANDLER to the callback
		//-O3 = less than 108 instructions from TIM_CC_HANDLER to the callback
		//lets just go with a wild ass guess of 110 clock cycles. probably too little for debug buy maybe just right for -O3?
		//time = 110 / clockFrequency
		//tickComp = time / (1 / _ticksPerSecond) = (110 * _ticksPerSecond) / clockFrequency
		_tickCompensation = round((110.0f * _ticksPerSecond) / clockFrequency);
		
		//set mode
		TIM_HandleStruct.Init.CounterMode = TIM_COUNTERMODE_UP;
		TIM_HandleStruct.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  		TIM_HandleStruct.Init.Period = 0xFFFF;
  		TIM_HandleStruct.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
		
		//init
		HAL_TIM_Base_Init(&TIM_HandleStruct);

		//configure clock
		TIM_ClockConfigTypeDef TIM_ClockConfigStruct = {0};
		TIM_ClockConfigStruct.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		HAL_TIM_ConfigClockSource(&TIM_HandleStruct, &TIM_ClockConfigStruct);
	
		//configure channel
		uint32_t Channel;
		TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
		TIM_OC_InitStruct.OCMode = TIM_OCMODE_TIMING;
		TIM_OC_InitStruct.OCPolarity = TIM_OCPOLARITY_HIGH;
		TIM_OC_InitStruct.Pulse = 0;
		
		switch (compareRegister)
		{
		case 1:
			Channel = TIM_CHANNEL_1;
			_compare_IT = TIM_IT_CC1;
			break;
		case 2:
			Channel = TIM_CHANNEL_2;
			_compare_IT = TIM_IT_CC2;
			break;
		case 3:
			Channel = TIM_CHANNEL_3;
			_compare_IT = TIM_IT_CC3;
			break;
		case 4:
			Channel = TIM_CHANNEL_4;
			_compare_IT = TIM_IT_CC4;
			break;
		}

		__HAL_TIM_ENABLE_IT(&TIM_HandleStruct, _compare_IT);

		HAL_TIM_OC_ConfigChannel(&TIM_HandleStruct, &TIM_OC_InitStruct, Channel);

		//enable update interrupt
		__HAL_TIM_ENABLE_IT(&TIM_HandleStruct, TIM_IT_UPDATE);
	
		//start channel
		HAL_TIM_OC_Start(&TIM_HandleStruct, Channel);

		//start timer
		HAL_TIM_Base_Start(&TIM_HandleStruct);
	}

	const uint32_t Stm32HalTimerService::GetTick()
	{
		return _tick | TIM->CNT;
	}

	void Stm32HalTimerService::ScheduleCallBack(const uint32_t tick)
	{
		//tick overhead compensation
		uint32_t compensatedTick = tick - _tickCompensation;
		uint32_t counter = _tick | TIM->CNT;
		if (_tick == (compensatedTick & 0xFFFF0000))
		{
			if (compensatedTick - _tickCompensation <= counter)
			{
				ReturnCallBack();
			}
			else
			{
				_futureTick = true;
				switch (_compare_IT)
				{
				case TIM_IT_CC1:
					TIM->CCR1 = compensatedTick & 0xFFFF;
					break;
				case TIM_IT_CC2:
					TIM->CCR2 = compensatedTick & 0xFFFF;
					break;
				case TIM_IT_CC3:
					TIM->CCR3 = compensatedTick & 0xFFFF;
					break;
				case TIM_IT_CC4:
					TIM->CCR4 = compensatedTick & 0xFFFF;
					break;
				}
			}	
		}
		//catch ticks that have already passed
		else if ((compensatedTick < counter && (counter - compensatedTick <= 2147483648)) || (compensatedTick > counter && (compensatedTick - counter > 2147483648)))
		{
			ReturnCallBack();
		}
		else
		{
			_futureTick = false;
			_callTick = tick;
		}
	}

	void Stm32HalTimerService::ReturnCallBack(void)
	{
		_futureTick = false;
		ITimerService::ReturnCallBack();
	}
	
	void Stm32HalTimerService::Interrupt(void)
	{		
		if (TIM->SR & _compare_IT) {
			if (_futureTick)
			{
				ReturnCallBack();
			}
			TIM->SR = ~_compare_IT;
		}
		if (TIM->SR & TIM_IT_UPDATE) {
			_tick += 0x00010000;	
			ScheduleCallBack(_callTick);
			TIM->SR = ~TIM_IT_UPDATE;
		}
	}

	const uint32_t Stm32HalTimerService::GetTicksPerSecond()
	{
		return _ticksPerSecond;
	}
}