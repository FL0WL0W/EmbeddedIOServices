#include "ITimerService.h"
#include "wm_regs.h"

#ifndef TIMERSERVICE_W80X_H
#define TIMERSERVICE_W80X_H
namespace EmbeddedIOServices
{
	class TimerService_W80x : public ITimerService
	{
	protected:
		const uint8_t _interruptTimer;
		const uint32_t _timerEn;
		volatile uint32_t * const _interruptPrd;
		const volatile uint32_t * const _tick;
		volatile bool _waitForInterrupt;
		tick_t _ticksPerSecond;
		void ScheduleCallBack(const tick_t tick);
	public:
		/**
		 * @brief W806 timers are weird. need one timer that will be our tick reference that counts from 0-2^32.
		 * The other will be used to actually schedule the interrupt.
		 * @param tickTimer tick reference timer index
		 * @param interruptTimer interrupt timer index
		 */
		TimerService_W80x(uint8_t tickTimer, uint8_t interruptTimer);
		~TimerService_W80x();
		tick_t GetTick();
		tick_t GetTicksPerSecond();

		static callback_t Timer0CallBack;
		static callback_t Timer1CallBack;
		static callback_t Timer2CallBack;
		static callback_t Timer3CallBack;
		static callback_t Timer4CallBack;
		static callback_t Timer5CallBack;
	};
}
#endif
