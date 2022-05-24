#include "TimerService_W806.h"

#ifdef TIMERSERVICE_W806_H
namespace EmbeddedIOServices
{	
	TimerService_W806::TimerService_W806(uint8_t tickTimer, uint8_t interruptTimer)
	{
		_tickTimerH.Instance = tickTimer;
		_tickTimerH.Init.Unit = TIM_UNIT_US;
		_tickTimerH.Init.Period = UINT32_MAX;
		_tickTimerH.Init.AutoReload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		HAL_TIM_Base_Init(&_tickTimerH);
    	HAL_TIM_Base_Start(&_tickTimerH);
		switch(tickTimer)
		{
			case TIM0:
				_tick = &TIM->TIM0_CNT;
				break;
			case TIM1:
				_tick = &TIM->TIM1_CNT;
				break;
			case TIM2:
				_tick = &TIM->TIM2_CNT;
				break;
			case TIM3:
				_tick = &TIM->TIM3_CNT;
				break;
			case TIM4:
				_tick = &TIM->TIM4_CNT;
				break;
			case TIM5:
				_tick = &TIM->TIM5_CNT;
				break;
		}
		
		_interruptTimerH.Instance = tickTimer;
		_interruptTimerH.Init.Unit = TIM_UNIT_US;
		_interruptTimerH.Init.Period = 1000000;
		_interruptTimerH.Init.AutoReload = TIM_AUTORELOAD_PRELOAD_Disable;
		HAL_TIM_Base_Init(&_interruptTimerH);
		
		Calibrate();
	}
	void TimerService_W806::ScheduleCallBack(const tick_t tick)
	{

	}
	tick_t TimerService_W806::GetTick()
	{
		return *_tick;
	}
	tick_t TimerService_W806::GetTicksPerSecond()
	{
		return 1000000;
	}
}
#endif
