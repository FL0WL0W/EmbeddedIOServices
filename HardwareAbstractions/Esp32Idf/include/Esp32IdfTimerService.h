#include "ITimerService.h"
#include "driver/gptimer.h"

#ifndef ESP32IDFTIMERSERVICE_H
#define ESP32IDFTIMERSERVICE_H

namespace Esp32
{		
	class Esp32IdfTimerService : public EmbeddedIOServices::ITimerService
	{
	private:
		const EmbeddedIOServices::tick_t _ticksPerSecond = 40000000; //APB clock speed is 80mhz, lowest prescaler is 2
    	gptimer_handle_t gptimer;
	protected:
		void ScheduleCallBack(const EmbeddedIOServices::tick_t tick) override;
	public:
		void TimerInterrupt();
		Esp32IdfTimerService(uint32_t group_num, uint32_t timer_num);
		EmbeddedIOServices::tick_t GetTick() override;
		EmbeddedIOServices::tick_t GetTicksPerSecond() override;
	};
}

#endif