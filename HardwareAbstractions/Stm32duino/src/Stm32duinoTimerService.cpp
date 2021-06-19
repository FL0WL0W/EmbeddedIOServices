#include <stdint.h>
#include <math.h>
#include <functional>
#include "Stm32duinoTimerService.h"
#include "Stm32duinoConf.h"

using namespace EmbeddedIOServices;

namespace Stm32
{	
	Stm32duinoTimerService::Stm32duinoTimerService(TimerIndex index)
	{	
		if(_timFrequencyLocked[index])
			return;
		_timFrequencyLocked[index] = true;

		EnableTimerInterrupts(index);
		_hardwareTimer[index]->setMode(1, TIMER_OUTPUT_COMPARE);
		TimInit(index, 1, 0xFFFF);

		TIM = TimIndexToTIM(index);
					
		_timCallBack[index] = [this]() { this->SetInterruptLatency(); };	
		_hardwareTimer[index]->resume();

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

		//set _returnCallBackCompensation. ignore warnings, but the compiler is probably going to optimize these out anyway
		_returnCallBackCompensation = TIM->CNT;
		if(_callTick != 0) ;
		const uint32_t t = _callTick - DWT->CYCCNT;
		if(t < 0x80000000 && t > _coarseTimerHalfLength)
			asm("nop");
		const uint32_t lt = _callTick - _whileWaitCompensation;
		_returnCallBackCompensation = _interruptLatency + TIM->CNT - _returnCallBackCompensation - getTickCompensation;
		
		_callTick = 0;

		_timCallBack[index] = [this]() { this->ReturnCallBack(); };
		_ticksPerSecond = HAL_RCC_GetSysClockFreq();
		const uint32_t _prescaler = _ticksPerSecond / (100 * 1000) - 1;
		_coarseTimerHalfLength = _prescaler * 0x7FFF;
		TimInit(index, _prescaler, 0xFFFF);

		//enable DWT-CYCCNT
		CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
		DWT->CTRL |= 1; 
	}

#pragma GCC push_options
#pragma GCC optimize("O0")
	void Stm32duinoTimerService::SetFunctionCallCompensation() 
	{
		uint32_t tick = TIM->CNT;
		_functionCallCompensation = tick - _functionCallCompensation;
	}
	void Stm32duinoTimerService::SetInterruptLatency() 
	{
		_interruptLatency = TIM->CNT - TIM->CCR1;
	}
#pragma GCC pop_options

	const uint32_t Stm32duinoTimerService::GetTick()
	{
		return DWT->CYCCNT;
	}

	void Stm32duinoTimerService::ScheduleCallBack(const uint32_t tick)
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
	
	void Stm32duinoTimerService::ReturnCallBack(void)
	{
		if(_callTick != 0)
		{
			const uint32_t t = _callTick - DWT->CYCCNT;
			if(t < 0x80000000 && t > _coarseTimerHalfLength)
				return;
			const uint32_t lt = _callTick - _whileWaitCompensation;
			while(TickLessThanTick(DWT->CYCCNT, lt)) ;
			_callTick = 0;
			ITimerService::ReturnCallBack();
		}
	}

	const uint32_t Stm32duinoTimerService::GetTicksPerSecond()
	{
		return _ticksPerSecond;
	}
}
