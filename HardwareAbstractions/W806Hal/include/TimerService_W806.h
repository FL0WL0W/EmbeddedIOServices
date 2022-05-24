#include "ITimerService.h"

#ifndef TIMERSERVICE_W806_H
#define TIMERSERVICE_W806_H
namespace EmbeddedIOServices
{
	class TimerService_W806 : public ITimerService
	{
	protected:
		TIM_HandleTypeDef _tickTimerH;
		TIM_HandleTypeDef _interruptTimerH;
		uint32_t *_tick;
		void ScheduleCallBack(const tick_t tick);
	public:
		/**
		 * @brief W806 timers are weird. need one timer that will be our tick reference that counts from 0-2^32.
		 * The other will be used to actually schedule the interrupt.
		 * @param tickTimer tick reference timer index
		 * @param interruptTimer interrupt timer index
		 */
		TimerService_W806(uint8_t tickTimer, uint8_t interruptTimer);
		tick_t GetTick();
		tick_t GetTicksPerSecond();
	};
}
#endif
