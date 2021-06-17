#include "ITimerService.h"
#include "Stm32HalTimer.h"
#include "Stm32HalConf.h"
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
		Stm32HalTimerService(TimerIndex timer);
		const uint32_t GetTick() override;
		const uint32_t GetTicksPerSecond() override;
	};
	
#ifdef TIM1
	extern std::function<void()> timerCallBack1;
#endif
#ifdef TIM2
	extern std::function<void()> timerCallBack2;
#endif
#ifdef TIM3
	extern std::function<void()> timerCallBack3;
#endif
#ifdef TIM4
	extern std::function<void()> timerCallBack4;
#endif
#ifdef TIM5
	extern std::function<void()> timerCallBack5;
#endif
#ifdef TIM6
	extern std::function<void()> timerCallBack6;
#endif
#ifdef TIM7
	extern std::function<void()> timerCallBack7;
#endif
#ifdef TIM8
	extern std::function<void()> timerCallBack8;
#endif
#ifdef TIM9
	extern std::function<void()> timerCallBack9;
#endif
#ifdef TIM10
	extern std::function<void()> timerCallBack10;
#endif
#ifdef TIM11
	extern std::function<void()> timerCallBack11;
#endif
#ifdef TIM12
	extern std::function<void()> timerCallBack12;
#endif
#ifdef TIM13
	extern std::function<void()> timerCallBack13;
#endif
#ifdef TIM14
	extern std::function<void()> timerCallBack14;
#endif
}

#endif