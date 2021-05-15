#include "ITimerService.h"
#include "Stm32HalTimer.h"
#include "stdint.h"

#ifndef STM32HALTIMERSERVICE_H
#define STM32HALTIMERSERVICE_H

namespace Stm32
{		
	class Stm32HalTimerService : public EmbeddedIOServices::ITimerService
	{
	private:
		uint32_t _ticksPerSecond;
		TIM_TypeDef *TIM;
		uint16_t _compare_IT;
		uint32_t _tick = 0;
		uint32_t _callTick = 0;
		bool _interruptDisabled = false;


		volatile uint16_t _interruptLatency = 0;
		uint16_t _functionCallCompensation = 0;
		uint16_t _whileWaitCompensation = 0;
		uint16_t _returnCallBackCompensation = 0;
		void SetFunctionCallCompensation();
		void SetInterruptLatency();


		void ReturnCallBack();
		void ScheduleCallBack(const uint32_t tick);
	public:
		Stm32HalTimerService(uint8_t timer);
		const uint32_t GetTick() override;
		const uint32_t GetTicksPerSecond() override;
	};
	
#ifdef TIM1
	extern EmbeddedIOServices::ICallBack *timerCallBack1;
#endif
#ifdef TIM2
	extern EmbeddedIOServices::ICallBack *timerCallBack2;
#endif
#ifdef TIM3
	extern EmbeddedIOServices::ICallBack *timerCallBack3;
#endif
#ifdef TIM4
	extern EmbeddedIOServices::ICallBack *timerCallBack4;
#endif
#ifdef TIM5
	extern EmbeddedIOServices::ICallBack *timerCallBack5;
#endif
#ifdef TIM6
	extern EmbeddedIOServices::ICallBack *timerCallBack6;
#endif
#ifdef TIM7
	extern EmbeddedIOServices::ICallBack *timerCallBack7;
#endif
#ifdef TIM8
	extern EmbeddedIOServices::ICallBack *timerCallBack8;
#endif
#ifdef TIM9
	extern EmbeddedIOServices::ICallBack *timerCallBack9;
#endif
#ifdef TIM10
	extern EmbeddedIOServices::ICallBack *timerCallBack10;
#endif
#ifdef TIM11
	extern EmbeddedIOServices::ICallBack *timerCallBack11;
#endif
#ifdef TIM12
	extern EmbeddedIOServices::ICallBack *timerCallBack12;
#endif
#ifdef TIM13
	extern EmbeddedIOServices::ICallBack *timerCallBack13;
#endif
#ifdef TIM14
	extern EmbeddedIOServices::ICallBack *timerCallBack14;
#endif
}

#endif