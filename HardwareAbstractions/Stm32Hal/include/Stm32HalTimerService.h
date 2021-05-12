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


		uint32_t _getTickCompensation;
		uint32_t _functionCallCompensation;
		void SetFunctionCallCompensation();
		uint32_t _whileWaitCompensation;
		uint32_t _returnCallBackCompensation;


		void ReturnCallBack();
		void ScheduleCallBack(const uint32_t tick);
	public:
		Stm32HalTimerService(uint8_t timer);
		void Interrupt();
		const uint32_t GetTick() override;
		const uint32_t GetTicksPerSecond() override;
	};
	
#ifdef TIM1
	extern Stm32HalTimerService *TimerService1;
#endif
#ifdef TIM2
	extern Stm32HalTimerService *TimerService2;
#endif
#ifdef TIM3
	extern Stm32HalTimerService *TimerService3;
#endif
#ifdef TIM4
	extern Stm32HalTimerService *TimerService4;
#endif
#ifdef TIM5
	extern Stm32HalTimerService *TimerService5;
#endif
#ifdef TIM6
	extern Stm32HalTimerService *TimerService6;
#endif
#ifdef TIM7
	extern Stm32HalTimerService *TimerService7;
#endif
#ifdef TIM8
	extern Stm32HalTimerService *TimerService8;
#endif
#ifdef TIM9
	extern Stm32HalTimerService *TimerService9;
#endif
#ifdef TIM10
	extern Stm32HalTimerService *TimerService10;
#endif
#ifdef TIM11
	extern Stm32HalTimerService *TimerService11;
#endif
#ifdef TIM12
	extern Stm32HalTimerService *TimerService12;
#endif
#ifdef TIM13
	extern Stm32HalTimerService *TimerService13;
#endif
#ifdef TIM14
	extern Stm32HalTimerService *TimerService14;
#endif
}

#endif