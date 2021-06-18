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
}

#endif