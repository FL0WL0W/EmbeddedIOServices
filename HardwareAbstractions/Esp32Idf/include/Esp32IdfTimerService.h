#include "ITimerService.h"
#include "driver/gptimer.h"

#ifndef ESP32IDFTIMERSERVICE_H
#define ESP32IDFTIMERSERVICE_H

namespace Esp32
{		
	class Esp32IdfTimerService : public EmbeddedIOServices::ITimerService
	{
	private:
		EmbeddedIOServices::tick_t _ticksPerSecond;
    	gptimer_handle_t gptimer;
	protected:
		void ScheduleCallBack(const EmbeddedIOServices::tick_t tick) override;
	public:
		void TimerInterrupt();
		Esp32IdfTimerService();
		EmbeddedIOServices::tick_t GetTick() override;
		EmbeddedIOServices::tick_t GetTicksPerSecond() override;
	};
}

#endif