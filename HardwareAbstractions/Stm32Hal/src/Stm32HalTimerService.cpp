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
		
		//Set Compare Channel 1 Intterupt
		_timCallBack[index] = [this]() { this->TimerInterrupt(); };
		
		//set ticks per second
		_ticksPerSecond = HAL_RCC_GetSysClockFreq();

		//enable DWT-CYCCNT
		CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
		DWT->CTRL |= 1; 

		Calibrate();
	}

	void Stm32HalTimerService::Calibrate()
	{
		_latency = 0;
		_minTicks = 0;

		//Get Tick Compensation
		volatile tick_t interruptTick;
		volatile tick_t latencyTick;
		interruptTick = this->GetTick();
		latencyTick = this->GetTick();
		const uint16_t getTickCompensation = latencyTick - interruptTick;

		//setup interrupt
		_interrupt = [this, &interruptTick]() { interruptTick = this->GetTick(); };

		//get minimum number of ticks to trigger an interrupt
		while(true)
		{
			_minTicks++;
			interruptTick = 0;
			ScheduleCallBack(GetTick());
			while(TickLessThanTick(GetTick(), _callTick)) ;
			if(interruptTick != 0)
				break;
		}

		//get minimum tick to add that schedules far enough in advance
		interruptTick = 0;
		latencyTick = GetTick();
		ScheduleCallBack(latencyTick);
		while(interruptTick == 0) ;
		const uint16_t minTickAdd = interruptTick - latencyTick - getTickCompensation;

		//get latency
		interruptTick = 0;
		latencyTick = GetTick() + minTickAdd;
		ScheduleCallBack(latencyTick);
		while(interruptTick == 0) ;
		_latency = interruptTick - latencyTick - getTickCompensation;

		//set return callback to interface
		_interrupt = [this]() { this->ReturnCallBack(); };

		ITimerService::Calibrate();
	}

	const tick_t Stm32HalTimerService::GetTick()
	{
		return DWT->CYCCNT;
	}

	void Stm32HalTimerService::ScheduleCallBack(const tick_t tick)
	{
		__disable_irq();
		_callTick = tick - _latency;
		TIM->DIER |= TIM_IT_CC1;
		const int ticks = _callTick - DWT->CYCCNT;
		if(ticks < static_cast<int>(_minTicks))
			TIM->CCR1 = TIM->CNT + _minTicks;
		else
			TIM->CCR1 = TIM->CNT + ticks;
		__enable_irq();
	}
	
	void Stm32HalTimerService::TimerInterrupt(void)
	{
		if(TickLessThanTick(_callTick - _latency, DWT->CYCCNT))
		{
			TIM->DIER &= ~TIM_IT_CC1;
			_interrupt();
		}
	}

	void Stm32HalTimerService::AttachInterrupt(std::function<void()> interrupt)
	{
		_interrupt = interrupt;
	}

	const tick_t Stm32HalTimerService::GetTicksPerSecond()
	{
		return _ticksPerSecond;
	}
}