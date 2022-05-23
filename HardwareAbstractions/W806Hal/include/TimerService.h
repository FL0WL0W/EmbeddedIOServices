#include "ITimerService.h"

#ifndef TIMERSERVICE_H
#define TIMERSERVICE_H
namespace EmbeddedIOServices
{
	class TimerService : public ITimerService
	{
	private:
	protected:
		void Calibrate();
		void ScheduleCallBack(const tick_t tick);
	public:
		tick_t GetTick();
		tick_t GetTicksPerSecond();
	};
}
#endif
