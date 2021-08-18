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
		EmbeddedIOServices::tick_t _ticksPerSecond;
		TIM_TypeDef *TIM;
		EmbeddedIOServices::tick_t _callTick = 0;
		uint16_t _latency;
		uint16_t _minTicks;
		std::function<void()> _interrupt;

		void TimerInterrupt();
		void ScheduleCallBack(const EmbeddedIOServices::tick_t tick) override;
	public:
		Stm32HalTimerService(TimerIndex timer);
		void AttachInterrupt(std::function<void()>);
		const EmbeddedIOServices::tick_t GetTick() override;
		const EmbeddedIOServices::tick_t GetTicksPerSecond() override;
		void Calibrate() override;
	};
}

#endif