#include "HardwareAbstraction/ITimerService.h"
#include "TimerLocks.h"

#ifndef STM32HALTIMERSERVICE_H
#define STM32HALTIMERSERVICE_H

namespace Stm32
{		
	class Stm32HalTimerService : public HardwareAbstraction::ITimerService
	{
	private:
		unsigned int _ticksPerSecond;
		TIM_TypeDef *TIM;
		unsigned short _compare_IT;
		unsigned short _tickCompensation;
		unsigned int _tick = 0;
		unsigned int _callTick = 0;
		bool _futureTick = false;
		bool _futureTock = false;
		void ReturnCallBack(void);
		void ScheduleCallBack(unsigned int tick);
	public:
		Stm32HalTimerService(unsigned char timer, unsigned char compareRegister, unsigned int ticksPerSecond);
		void Interrupt(void);
		unsigned int GetTick(void);
		unsigned int GetTicksPerSecond(void);
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