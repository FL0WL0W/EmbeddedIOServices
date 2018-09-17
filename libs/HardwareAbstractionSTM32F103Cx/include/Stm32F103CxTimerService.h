#include "Stm32F103CxDefines.h"
#include "HardwareAbstraction/ITimerService.h"

namespace Stm32
{		
	class Stm32F103CxTimerService : public HardwareAbstraction::ITimerService
	{
	private:
		unsigned int _ticksPerSecond;
		TIM_TypeDef *TIM;
		unsigned char _compareRegister;
		unsigned short TIM_IT_CC;
		unsigned int _tick = 0;
		unsigned int _callTick = 0;
		bool _futureTick = false;
		bool _futureTock = false;
		void ReturnCallBack(void);
		void ScheduleCallBack(unsigned int tick);
	public:
		Stm32F103CxTimerService(unsigned char timer, unsigned char compareRegister, unsigned int ticksPerSecond);
		void Interrupt(void);
		unsigned int GetTick(void);
		unsigned int GetTicksPerSecond(void);
	};
	
	extern Stm32F103CxTimerService *TimerService1;
	extern Stm32F103CxTimerService *TimerService2;
	extern Stm32F103CxTimerService *TimerService3;
	extern Stm32F103CxTimerService *TimerService4;
}