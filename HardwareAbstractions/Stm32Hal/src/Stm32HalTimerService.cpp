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
	
	TIM_HandleTypeDef TIM_HandleStruct = {0};

	Stm32HalTimerService::Stm32HalTimerService(uint8_t timer)
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
		
		//set prescaler
		_ticksPerSecond = HAL_RCC_GetSysClockFreq();
				
		//set mode
		TIM_HandleStruct.Instance = TIM;
		TIM_HandleStruct.Init.CounterMode = TIM_COUNTERMODE_UP;
		TIM_HandleStruct.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  		TIM_HandleStruct.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
		TIM_HandleStruct.Init.Prescaler = _ticksPerSecond / (1000 * 1000) - 1; // set ticks to 10 us
  		TIM_HandleStruct.Init.Period = 0xFFFF; // autoreload at max
		
		//init
		HAL_TIM_Base_Init(&TIM_HandleStruct);

		//configure clock
		TIM_ClockConfigTypeDef TIM_ClockConfigStruct = {0};
		TIM_ClockConfigStruct.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		HAL_TIM_ConfigClockSource(&TIM_HandleStruct, &TIM_ClockConfigStruct);

		uint32_t Channel;
		TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
		TIM_OC_InitStruct.OCMode = TIM_OCMODE_TIMING;
		TIM_OC_InitStruct.OCPolarity = TIM_OCPOLARITY_HIGH;
		TIM_OC_InitStruct.Pulse = 0;
		
		__HAL_TIM_ENABLE_IT(&TIM_HandleStruct, TIM_IT_CC1);
		
		HAL_TIM_OC_ConfigChannel(&TIM_HandleStruct, &TIM_OC_InitStruct, TIM_CHANNEL_1);
	
		//start timer
		HAL_TIM_Base_Start(&TIM_HandleStruct);

		CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
		DWT->CTRL |= 1; 

		//calibrate. this stuff will probably need to be tweaked
		//set _functionCallCompensation
		uint32_t beforeTick;
		uint32_t afterTick;
		beforeTick = DWT->CYCCNT;
		afterTick = DWT->CYCCNT;
		_getTickCompensation = afterTick - beforeTick;

		_functionCallCompensation = DWT->CYCCNT;
		SetFunctionCallCompensation();
		_functionCallCompensation -= _getTickCompensation;

		_callTick = afterTick;
		if(_callTick == 0)
			_callTick = 1;

		//set _whileWaitCompensation
		_whileWaitCompensation = DWT->CYCCNT;
		while(TickLessThanTick(DWT->CYCCNT, _callTick)) ;
		_whileWaitCompensation = DWT->CYCCNT - _whileWaitCompensation - _getTickCompensation;

		//set _returnCallBackCompensation
		_returnCallBackCompensation = DWT->CYCCNT;
		if (TIM->SR & TIM_IT_CC1) ;
		if(!_interruptDisabled) ;
		if(_callTick != 0)
		{
			const uint32_t lt = _callTick - _whileWaitCompensation - _functionCallCompensation;
			while(TickLessThanTick(DWT->CYCCNT, lt)) ;
		}
		_returnCallBackCompensation = DWT->CYCCNT - _returnCallBackCompensation - _getTickCompensation + 150; //adding 150 clocks for interrupt latency. 
		
		_callTick = 0;
	}

	#pragma GCC push_options
	#pragma GCC optimize("O0")
	void Stm32HalTimerService::SetFunctionCallCompensation() 
	{
		uint32_t tick = DWT->CYCCNT;
		_functionCallCompensation = tick - _functionCallCompensation;
	}
	#pragma GCC pop_options

	const uint32_t Stm32HalTimerService::GetTick()
	{
		return DWT->CYCCNT;
	}

	void Stm32HalTimerService::ScheduleCallBack(const uint32_t tick)
	{
		__disable_irq();

		if(TickLessThanTick(tick, DWT->CYCCNT + 250))
		{
			_interruptDisabled = true;
			__enable_irq();
			const uint32_t lt = tick - _whileWaitCompensation - _functionCallCompensation;
			while(TickLessThanTick(DWT->CYCCNT, lt)) ;
			ITimerService::ReturnCallBack();
			_interruptDisabled = false;
		}
		else
		{
			_callTick = tick;
			if(_callTick == 0)
				_callTick = 1;
			uint32_t ccr = (_callTick - _functionCallCompensation * 2 - _returnCallBackCompensation - DWT->CYCCNT) / (TIM->PSC + 1);
			TIM->CCR1 = TIM->CNT + ccr;
			__enable_irq();
		}		
	}
	
	// extern "C" int minTickDiff;
	// extern "C" int maxTickDiff;
	// extern "C" int tickDiff;
	void Stm32HalTimerService::ReturnCallBack(void)
	{
		if(_callTick != 0)
		{
			const uint32_t lt = _callTick - _whileWaitCompensation - _functionCallCompensation;
			while(TickLessThanTick(DWT->CYCCNT, lt)) ;
			//tickDiff = DWT->CYCCNT;
			ITimerService::ReturnCallBack();
			_callTick = 0;
			// tickDiff -= (lt + _whileWaitCompensation) + _getTickCompensation;
			// if(minTickDiff > tickDiff)
			// 	minTickDiff = tickDiff;
			// else if(maxTickDiff < tickDiff)
			// 	maxTickDiff = tickDiff;
		}
	}
	
	void Stm32HalTimerService::Interrupt(void)
	{		
		if (TIM->SR & TIM_IT_CC1) {
			if(!_interruptDisabled)
				ReturnCallBack();
			TIM->SR = ~TIM_IT_CC1;
		}
	}

	const uint32_t Stm32HalTimerService::GetTicksPerSecond()
	{
		return _ticksPerSecond;
	}
}