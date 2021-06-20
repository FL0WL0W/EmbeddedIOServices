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
		uint32_t _ticksPerSecond;
		TIM_TypeDef *TIM;
		uint32_t _callTick = 0;
		uint32_t _latency = 0;
		std::function<void()> _interrupt;
		std::function<void()> *_timCallBackPtr;

		void TimerInterrupt();
		void ScheduleCallBack(const uint32_t tick);
	public:
		Stm32duinoTimerService(TimerIndex timer);
		void AttachInterrupt(std::function<void()>);
		const uint32_t GetTick() override;
		const uint32_t GetTicksPerSecond() override;
	};
}

#endif