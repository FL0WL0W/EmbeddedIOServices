#include "ITimerService.h"
#include "wm_regs.h"

#ifndef TIMERSERVICE_W806_H
#define TIMERSERVICE_W806_H
namespace EmbeddedIOServices
{
	struct TimerInterrupt 
	{
		callback_t CallBack;
		uint8_t Timer;

		TimerInterrupt(uint8_t timer, callback_t callBack) : Timer(timer), CallBack(callBack) { }
	};
	typedef std::forward_list<TimerInterrupt> TimerInterruptList;

	class TimerService_W806 : public ITimerService
	{
	protected:
		const uint8_t _interruptTimer;
		const volatile uint32_t *_tick;
		volatile bool _waitForInterrupt;
		void ScheduleCallBack(const tick_t tick);
	public:
		/**
		 * @brief W806 timers are weird. need one timer that will be our tick reference that counts from 0-2^32.
		 * The other will be used to actually schedule the interrupt.
		 * @param tickTimer tick reference timer index
		 * @param interruptTimer interrupt timer index
		 */
		TimerService_W806(uint8_t tickTimer, uint8_t interruptTimer);
		~TimerService_W806();
		tick_t GetTick();
		tick_t GetTicksPerSecond();
		void TimerInterruptCallback();

		static TimerInterruptList InterruptList;
	};
}
#endif
