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
		uint32_t _callTick = 0;
		uint32_t _latency = 0;
		std::function<void()> _interrupt;

		void TimerInterrupt();
		void ScheduleCallBack(const uint32_t tick) override;
	public:
		Stm32HalTimerService(TimerIndex timer);
		void AttachInterrupt(std::function<void()>);
		const uint32_t GetTick() override;
		const uint32_t GetTicksPerSecond() override;
	};
}

#endif