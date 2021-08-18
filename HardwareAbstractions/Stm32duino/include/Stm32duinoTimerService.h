#include "ITimerService.h"
#include "Stm32duinoTimer.h"
#include "Stm32duinoConf.h"
#include "stdint.h"

#ifndef STM32DUINOTIMERSERVICE_H
#define STM32DUINOTIMERSERVICE_H

namespace Stm32
{		
	class Stm32duinoTimerService : public EmbeddedIOServices::ITimerService
	{
	private:
		EmbeddedIOServices::tick_t _ticksPerSecond;
		TIM_TypeDef *TIM;
		EmbeddedIOServices::tick_t _callTick = 0;
		uint16_t _latency;
		uint16_t _minTicks;
		std::function<void()> _interrupt;

		void TimerInterrupt();
		void ScheduleCallBack(const EmbeddedIOServices::tick_t tick);
	public:
		Stm32duinoTimerService(TimerIndex timer);
		void AttachInterrupt(std::function<void()>);
		const EmbeddedIOServices::tick_t GetTick() override;
		const EmbeddedIOServices::tick_t GetTicksPerSecond() override;
	};
}

#endif