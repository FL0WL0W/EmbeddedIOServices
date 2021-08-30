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
		TIM_TypeDef * const TIM;
		const EmbeddedIOServices::tick_t _ticksPerSecond;
		EmbeddedIOServices::tick_t _callTick = 0;
	protected:
		void ScheduleCallBack(const EmbeddedIOServices::tick_t tick) override;
	public:
		void TimerInterrupt();
		Stm32HalTimerService(TimerIndex timer);
		EmbeddedIOServices::tick_t GetTick() override;
		EmbeddedIOServices::tick_t GetTicksPerSecond() override;
	};

	extern Stm32HalTimerService *_timer[TimerIndex::Num];
}

#endif