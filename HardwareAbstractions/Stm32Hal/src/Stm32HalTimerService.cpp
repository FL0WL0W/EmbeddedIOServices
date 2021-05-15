#include <stdint.h>
#include <math.h>
#include <functional>
#include "Stm32HalTimerService.h"
#include "Stm32HalConf.h"

using namespace EmbeddedIOServices;

namespace Stm32
{
#ifdef TIM1
	ICallBack *timerCallBack1 = 0;
#endif
#ifdef TIM2
	ICallBack *timerCallBack2 = 0;
#endif
#ifdef TIM3
	ICallBack *timerCallBack3 = 0;
#endif
#ifdef TIM4
	ICallBack *timerCallBack4 = 0;
#endif
#ifdef TIM5
	ICallBack *timerCallBack5 = 0;
#endif
#ifdef TIM6
	ICallBack *timerCallBack6 = 0;
#endif
#ifdef TIM7
	ICallBack *timerCallBack7 = 0;
#endif
#ifdef TIM8
	ICallBack *timerCallBack8 = 0;
#endif
#ifdef TIM9
	ICallBack *timerCallBack9 = 0;
#endif
#ifdef TIM10
	ICallBack *timerCallBack10 = 0;
#endif
#ifdef TIM11
	ICallBack *timerCallBack11 = 0;
#endif
#ifdef TIM12
	ICallBack *timerCallBack12 = 0;
#endif
#ifdef TIM13
	ICallBack *timerCallBack13 = 0;
#endif
#ifdef TIM14
	ICallBack *timerCallBack14 = 0;
#endif
	
	TIM_HandleTypeDef TIM_HandleStruct = {0};

	void TimInit(TIM_TypeDef *TIM, uint32_t prescaler)
	{
		//set mode
		TIM_HandleStruct.Instance = TIM;
		TIM_HandleStruct.Init.CounterMode = TIM_COUNTERMODE_UP;
		TIM_HandleStruct.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  		TIM_HandleStruct.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
		TIM_HandleStruct.Init.Prescaler = prescaler;
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
	}

	Stm32HalTimerService::Stm32HalTimerService(uint8_t timer)
	{	
		ICallBack **timerCallBack = 0;
		switch (timer)
		{
#ifdef TIM1
		case 1:
			if (TIM1_Freq_Locked)
				return;
			TIM1_Freq_Locked = true;
			timerCallBack = &timerCallBack1;
			TIM = TIM1;
			break;
#endif
#ifdef TIM2
		case 2:
			if (TIM2_Freq_Locked)
				return;
			TIM2_Freq_Locked = true;
			timerCallBack = &timerCallBack2;
			TIM = TIM2;
			break;
#endif
#ifdef TIM3
		case 3:
			if (TIM3_Freq_Locked)
				return;
			TIM3_Freq_Locked = true;
			timerCallBack = &timerCallBack3;
			TIM = TIM3;
			break;
#endif
#ifdef TIM4
		case 4:
			if (TIM4_Freq_Locked)
				return;
			TIM4_Freq_Locked = true;
			timerCallBack = &timerCallBack4;
			TIM = TIM4;
			break;
#endif
#ifdef TIM5
		case 5:
			if (TIM5_Freq_Locked)
				return;
			TIM5_Freq_Locked = true;
			timerCallBack = &timerCallBack5;
			TIM = TIM5;
			break;
#endif
#ifdef TIM6
		case 6:
			if (TIM6_Freq_Locked)
				return;
			TIM6_Freq_Locked = true;
			timerCallBack = &timerCallBack6;
			TIM = TIM6;
			break;
#endif
#ifdef TIM7
		case 7:
			if (TIM7_Freq_Locked)
				return;
			TIM7_Freq_Locked = true;
			timerCallBack = &timerCallBack7;
			TIM = TIM7;
			break;
#endif
#ifdef TIM8
		case 8:
			if (TIM8_Freq_Locked)
				return;
			TIM8_Freq_Locked = true;
			timerCallBack = &timerCallBack8;
			TIM = TIM8;
			break;
#endif
#ifdef TIM9
		case 9:
			if (TIM9_Freq_Locked)
				return;
			TIM9_Freq_Locked = true;
			timerCallBack = &timerCallBack9;
			TIM = TIM9;
			__HAL_RCC_TIM9_CLK_ENABLE();
			break;
#endif
#ifdef TIM10
		case 10:
			if (TIM10_Freq_Locked)
				return;
			TIM10_Freq_Locked = true;
			timerCallBack = &timerCallBack10;
			TIM = TIM10;
			__HAL_RCC_TIM10_CLK_ENABLE();
			break;
#endif
#ifdef TIM11
		case 11:
			if (TIM11_Freq_Locked)
				return;
			TIM11_Freq_Locked = true;
			timerCallBack = &timerCallBack11;
			TIM = TIM11;
			break;
#endif
#ifdef TIM12
		case 12:
			if (TIM12_Freq_Locked)
				return;
			TIM12_Freq_Locked = true;
			timerCallBack = &timerCallBack12;
			TIM = TIM12;
			break;
#endif
#ifdef TIM13
		case 13:
			if (TIM13_Freq_Locked)
				return;
			TIM13_Freq_Locked = true;
			timerCallBack = &timerCallBack13;
			TIM = TIM13;
			break;
#endif
#ifdef TIM14
		case 14:
			if (TIM14_Freq_Locked)
				return;
			TIM14_Freq_Locked = true;
			timerCallBack = &timerCallBack14;
			TIM = TIM14;
			break;
#endif
		}
		EnableTimerInterrupts(timer);
		EnableTimerClock(timer);
					
		*timerCallBack = new CallBack<Stm32HalTimerService>(this, &Stm32HalTimerService::SetInterruptLatency);	
		TimInit(TIM, 0);

		//calibrate
		//set _functionCallCompensation
		uint32_t beforeTick;
		uint32_t afterTick;
		beforeTick = TIM->CNT;
		afterTick = TIM->CNT;
		uint8_t getTickCompensation = afterTick - beforeTick;

		_functionCallCompensation = TIM->CNT;
		SetFunctionCallCompensation();
		_functionCallCompensation -= getTickCompensation;
		
		TIM->CCR1 = TIM->CNT + 1000;
		while(_interruptLatency == 0) ;

		_callTick = afterTick;
		if(_callTick == 0)
			_callTick = 1;

		//set _whileWaitCompensation
		_whileWaitCompensation = TIM->CNT;
		while(TickLessThanTick(TIM->CNT, _callTick)) ;
		_whileWaitCompensation = TIM->CNT - _whileWaitCompensation - getTickCompensation;

		//set _returnCallBackCompensation
		_returnCallBackCompensation = TIM->CNT;
		if(_callTick != 0) ;
		const uint32_t lt = _callTick - _whileWaitCompensation;
		_returnCallBackCompensation = _interruptLatency + TIM->CNT - _returnCallBackCompensation - getTickCompensation;
		
		_callTick = 0;

		*timerCallBack = new CallBack<Stm32HalTimerService>(this, &Stm32HalTimerService::ReturnCallBack);
		_ticksPerSecond = HAL_RCC_GetSysClockFreq();
		TimInit(TIM, _ticksPerSecond / (100 * 1000) - 1);

		//enable DWT-CYCCNT
		CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
		DWT->CTRL |= 1; 
	}

#pragma GCC push_options
#pragma GCC optimize("O0")
	void Stm32HalTimerService::SetFunctionCallCompensation() 
	{
		uint32_t tick = TIM->CNT;
		_functionCallCompensation = tick - _functionCallCompensation;
	}
	void Stm32HalTimerService::SetInterruptLatency() 
	{
		_interruptLatency = TIM->CNT - TIM->CCR1;
	}
#pragma GCC pop_options

	const uint32_t Stm32HalTimerService::GetTick()
	{
		return DWT->CYCCNT;
	}

	void Stm32HalTimerService::ScheduleCallBack(const uint32_t tick)
	{
		if(TickLessThanTick(tick, DWT->CYCCNT + _returnCallBackCompensation + TIM->PSC))
		{
			_callTick = tick;
			if(_callTick == 0)
				_callTick = 1;
			ReturnCallBack();
		}
		else
		{
			_callTick = tick;
			if(_callTick == 0)
				_callTick = 1;
			const int ccr = (_callTick - _returnCallBackCompensation - DWT->CYCCNT + 1) / (TIM->PSC + 1) - 1;
			TIM->CCR1 = TIM->CNT + ccr;
		}		
	}
	
	void Stm32HalTimerService::ReturnCallBack(void)
	{
		if(_callTick != 0)
		{
			const uint32_t lt = _callTick - _whileWaitCompensation;
			while(TickLessThanTick(DWT->CYCCNT, lt)) ;
			_callTick = 0;
			ITimerService::ReturnCallBack();
		}
	}

	const uint32_t Stm32HalTimerService::GetTicksPerSecond()
	{
		return _ticksPerSecond;
	}
}