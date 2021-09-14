#include "ITimerService.h"
#include "hal/timer_hal.h"
#include "esp_intr_alloc.h"

#ifndef ESP32IDFTIMERSERVICE_H
#define ESP32IDFTIMERSERVICE_H

namespace Esp32
{		
	class Esp32IdfTimerService : public EmbeddedIOServices::ITimerService
	{
	private:
		const EmbeddedIOServices::tick_t _ticksPerSecond = 40000000; //APB clock speed is 80mhz, lowest prescaler is 2
		EmbeddedIOServices::tick_t _callTick = 0;
		timer_hal_context_t hal;
		intr_handle_t timer_isr_handle;
	protected:
		void ScheduleCallBack(const EmbeddedIOServices::tick_t tick) override;
	public:
		void TimerInterrupt();
		Esp32IdfTimerService(timer_group_t group_num, timer_idx_t timer_num);
		EmbeddedIOServices::tick_t GetTick() override;
		EmbeddedIOServices::tick_t GetTicksPerSecond() override;
	};
}

#endif