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
		TIM_TypeDef * const TIM;
		const EmbeddedIOServices::tick_t _ticksPerSecond;
		EmbeddedIOServices::tick_t _callTick = 0;
	protected:
		void ScheduleCallBack(const EmbeddedIOServices::tick_t tick);
	public:
		void TimerInterrupt();
		Stm32duinoTimerService(TimerIndex timer);
		EmbeddedIOServices::tick_t GetTick() override;
		EmbeddedIOServices::tick_t GetTicksPerSecond() override;
	};
}

#endif