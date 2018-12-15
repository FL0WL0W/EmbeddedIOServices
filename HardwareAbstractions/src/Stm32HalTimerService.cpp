#include <stdint.h>
#include <math.h>
#include <functional>
#include "Stm32HalTimerService.h"

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
	
	Stm32HalTimerService::Stm32HalTimerService(unsigned char timer, unsigned char compareRegister, unsigned int ticksPerSecond)
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
			__HAL_RCC_TIM1_CLK_ENABLE();
			//HAL_NVIC_SetPriority(TIM1_UP_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
			//HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
			//(TIM1_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM1_IRQn);
			break;
#endif
#ifdef TIM2
		case 2:
			if (TIM2_Freq_Locked)
				return;
			TIM2_Freq_Locked = true;
			TimerService2 = this;
			TIM = TIM2;
			__HAL_RCC_TIM2_CLK_ENABLE();
			//HAL_NVIC_SetPriority(TIM2_UP_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM2_UP_IRQn);
			//HAL_NVIC_SetPriority(TIM2_CC_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM2_CC_IRQn);
			//HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM2_IRQn);
			break;
#endif
#ifdef TIM3
		case 3:
			if (TIM3_Freq_Locked)
				return;
			TIM3_Freq_Locked = true;
			TimerService3 = this;
			TIM = TIM3;
			__HAL_RCC_TIM3_CLK_ENABLE();
			//HAL_NVIC_SetPriority(TIM3_UP_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM3_UP_IRQn);
			//HAL_NVIC_SetPriority(TIM3_CC_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM3_CC_IRQn);
			//HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM3_IRQn);
			break;
#endif
#ifdef TIM4
		case 4:
			if (TIM4_Freq_Locked)
				return;
			TIM4_Freq_Locked = true;
			TimerService4 = this;
			TIM = TIM4;
			__HAL_RCC_TIM4_CLK_ENABLE();
			//HAL_NVIC_SetPriority(TIM4_UP_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM4_UP_IRQn);
			//HAL_NVIC_SetPriority(TIM4_CC_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM4_CC_IRQn);
			//HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM4_IRQn);
			break;
#endif
#ifdef TIM5
		case 5:
			if (TIM5_Freq_Locked)
				return;
			TIM5_Freq_Locked = true;
			TimerService5 = this;
			TIM = TIM5;
			__HAL_RCC_TIM5_CLK_ENABLE();
			//HAL_NVIC_SetPriority(TIM5_UP_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM5_UP_IRQn);
			//HAL_NVIC_SetPriority(TIM5_CC_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM5_CC_IRQn);
			//HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM5_IRQn);
			break;
#endif
#ifdef TIM6
		case 6:
			if (TIM6_Freq_Locked)
				return;
			TIM6_Freq_Locked = true;
			TimerService6 = this;
			TIM = TIM6;
			__HAL_RCC_TIM6_CLK_ENABLE();
			//HAL_NVIC_SetPriority(TIM6_UP_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM6_UP_IRQn);
			//HAL_NVIC_SetPriority(TIM6_CC_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM6_CC_IRQn);
			//HAL_NVIC_SetPriority(TIM6_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM6_IRQn);
			break;
#endif
#ifdef TIM7
		case 7:
			if (TIM7_Freq_Locked)
				return;
			TIM7_Freq_Locked = true;
			TimerService7 = this;
			TIM = TIM7;
			__HAL_RCC_TIM7_CLK_ENABLE();
			//HAL_NVIC_SetPriority(TIM7_UP_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM7_UP_IRQn);
			//HAL_NVIC_SetPriority(TIM7_CC_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM7_CC_IRQn);
			//HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM7_IRQn);
			break;
#endif
#ifdef TIM8
		case 8:
			if (TIM8_Freq_Locked)
				return;
			TIM8_Freq_Locked = true;
			TimerService8 = this;
			TIM = TIM8;
			__HAL_RCC_TIM8_CLK_ENABLE();
			//HAL_NVIC_SetPriority(TIM8_UP_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM8_UP_IRQn);
			//HAL_NVIC_SetPriority(TIM8_CC_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM8_CC_IRQn);
			//HAL_NVIC_SetPriority(TIM8_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM8_IRQn);
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
			//HAL_NVIC_SetPriority(TIM9_UP_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM9_UP_IRQn);
			//HAL_NVIC_SetPriority(TIM9_CC_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM9_CC_IRQn);
			//HAL_NVIC_SetPriority(TIM9_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM9_IRQn);
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
			//HAL_NVIC_SetPriority(TIM10_UP_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM10_UP_IRQn);
			//HAL_NVIC_SetPriority(TIM10_CC_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM10_CC_IRQn);
			//HAL_NVIC_SetPriority(TIM10_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM10_IRQn);
			break;
#endif
#ifdef TIM11
		case 11:
			if (TIM11_Freq_Locked)
				return;
			TIM11_Freq_Locked = true;
			TimerService11 = this;
			TIM = TIM11;
			__HAL_RCC_TIM11_CLK_ENABLE();
			//HAL_NVIC_SetPriority(TIM11_UP_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM11_UP_IRQn);
			//HAL_NVIC_SetPriority(TIM11_CC_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM11_CC_IRQn);
			//HAL_NVIC_SetPriority(TIM11_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM11_IRQn);
			break;
#endif
#ifdef TIM12
		case 12:
			if (TIM12_Freq_Locked)
				return;
			TIM12_Freq_Locked = true;
			TimerService12 = this;
			TIM = TIM12;
			__HAL_RCC_TIM12_CLK_ENABLE();
			//HAL_NVIC_SetPriority(TIM12_UP_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM12_UP_IRQn);
			//HAL_NVIC_SetPriority(TIM12_CC_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM12_CC_IRQn);
			//HAL_NVIC_SetPriority(TIM12_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM12_IRQn);
			break;
#endif
#ifdef TIM13
		case 13:
			if (TIM13_Freq_Locked)
				return;
			TIM13_Freq_Locked = true;
			TimerService13 = this;
			TIM = TIM13;
			__HAL_RCC_TIM13_CLK_ENABLE();
			//HAL_NVIC_SetPriority(TIM13_UP_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM13_UP_IRQn);
			//HAL_NVIC_SetPriority(TIM13_CC_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM13_CC_IRQn);
			//HAL_NVIC_SetPriority(TIM13_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM13_IRQn);
			break;
#endif
#ifdef TIM14
		case 14:
			if (TIM14_Freq_Locked)
				return;
			TIM14_Freq_Locked = true;
			TimerService14 = this;
			TIM = TIM14;
			__HAL_RCC_TIM14_CLK_ENABLE();
			//HAL_NVIC_SetPriority(TIM14_UP_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM14_UP_IRQn);
			//HAL_NVIC_SetPriority(TIM14_CC_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM14_CC_IRQn);
			//HAL_NVIC_SetPriority(TIM14_IRQn, 0, 0);
    		//HAL_NVIC_EnableIRQ(TIM14_IRQn);
			break;
#endif
		}
		
		TIM_HandleTypeDef TIM_HandleStruct = {0};
		TIM_HandleStruct.Instance = TIM;

		//set prescaler
		unsigned int clockFrequency = HAL_RCC_GetSysClockFreq();
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
		unsigned int Channel;
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

	unsigned int Stm32HalTimerService::GetTick()
	{
		return _tick | TIM->CNT;
	}

	void Stm32HalTimerService::ScheduleCallBack(unsigned int tick)
	{
		//tick overhead compensation
		tick -= _tickCompensation;
		unsigned int counter = _tick | TIM->CNT;
		if (_tick == (tick & 0xFFFF0000))
		{
			if (tick <= counter)
			{
				ReturnCallBack();
			}
			else
			{
				_futureTock = false;
				_futureTick = true;
				switch (_compare_IT)
				{
				case TIM_IT_CC1:
					TIM->CCR1 = tick & 0xFFFF;
					break;
				case TIM_IT_CC2:
					TIM->CCR2 = tick & 0xFFFF;
					break;
				case TIM_IT_CC3:
					TIM->CCR3 = tick & 0xFFFF;
					break;
				case TIM_IT_CC4:
					TIM->CCR4 = tick & 0xFFFF;
					break;
				}
			}	
		}
		else if ((tick < counter && (counter - tick <= 2147483648)) || (tick > counter && (tick - counter > 2147483648)))
		{
			ReturnCallBack();
		}
		else
		{
			_futureTock = true;
			_futureTick = false;
			_callTick = tick;
		}
	}

	void Stm32HalTimerService::ReturnCallBack(void)
	{
		_futureTick = false;
		_futureTock = false;
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
			if (_futureTock)
			{
				ScheduleCallBack(_callTick);
			}
			TIM->SR = ~TIM_IT_UPDATE;
		}
	}

	unsigned int Stm32HalTimerService::GetTicksPerSecond()
	{
		return _ticksPerSecond;
	}
}