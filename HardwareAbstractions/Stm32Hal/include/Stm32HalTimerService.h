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

		void ScheduleCallBack(const EmbeddedIOServices::tick_t tick) override;
	public:
		void TimerInterrupt();
		Stm32HalTimerService(TimerIndex timer);
		const EmbeddedIOServices::tick_t GetTick() override;
		const EmbeddedIOServices::tick_t GetTicksPerSecond() override;
		void Calibrate() override;
	};

	extern Stm32HalTimerService *_timer[TimerIndex::Num];
}

#endif