#include "TimerService_W80x.h"

#define RCC ((RCC_TypeDef *)RCC_BASE)
#define TIM ((TIM_TypeDef *)TIM_BASE)

#ifdef TIMERSERVICE_W80X_H
namespace EmbeddedIOServices
{	
	callback_t TimerService_W80x::Timer0CallBack = 0;
	callback_t TimerService_W80x::Timer1CallBack = 0;
	callback_t TimerService_W80x::Timer2CallBack = 0;
	callback_t TimerService_W80x::Timer3CallBack = 0;
	callback_t TimerService_W80x::Timer4CallBack = 0;
	callback_t TimerService_W80x::Timer5CallBack = 0;

	TimerService_W80x::TimerService_W80x(uint8_t tickTimer, uint8_t interruptTimer, tick_t ticksPerSecond) : _interruptTimer(interruptTimer), _tick(&TIM->TIM0_CNT + tickTimer), _interruptPrd(&TIM->TIM0_PRD + interruptTimer), _timerEn(1 << (interruptTimer * 5 + 2))
	{
		//Enable Timer Clock
    	RCC->CLK_EN |= RCC_CLK_EN_TIMER;

		//set timing divider value to apb clock - 1. 
		//this gives us microsecond resolution which is the expected standard.
		//for more resolution we could probably just set this value to 0
		const uint8_t apbclk = (480 / (0xFF & RCC->CLK_DIV)) / (0xFF & (RCC->CLK_DIV >> 16));
		TIM->TMR_CONFIG = (((apbclk * 1000000) + (ticksPerSecond / 2)) / ticksPerSecond) - 1;
		_ticksPerSecond = (apbclk * 1000000) / (TIM->TMR_CONFIG+1);

		//reload at max uint32
		*(&TIM->TIM0_PRD + tickTimer) = UINT32_MAX;
		//set timer to microsecond resolution
		TIM->CR &= ~(1 << (tickTimer * 5));
		//set timer to autoreload
		TIM->CR &= ~(1 << (tickTimer * 5 + 1));
		//disable interrupt
		TIM->CR |= (1 << (tickTimer * 5 + 3));
		//enable timer
		TIM->CR |= (1 << (tickTimer * 5 + 2));
		
		//period at max uint32
		*(&TIM->TIM0_PRD + interruptTimer) = UINT32_MAX;
		//set timer to microsecond resolution
		TIM->CR &= ~(1 << (interruptTimer * 5));
		//set timer to not autoreload
		TIM->CR |= (1 << (interruptTimer * 5 + 1));
		//disable timer
		TIM->CR &= ~(1 << (interruptTimer * 5 + 2));
		callback_t callback = [this](){ReturnCallBack();};
		switch(interruptTimer)
		{
			case 0:
				Timer0CallBack = callback;
				break;
			case 1:
				Timer1CallBack = callback;
				break;
			case 2:
				Timer2CallBack = callback;
				break;
			case 3:
				Timer3CallBack = callback;
				break;
			case 4:
				Timer4CallBack = callback;
				break;
			case 5:
				Timer5CallBack = callback;
				break;
		}
		//enable interrupt
		TIM->CR |= (1 << (interruptTimer * 5 + 3));
		csi_vic_enable_irq(TIM_IRQn);
		csi_vic_set_prio(TIM_IRQn, 1);

		Calibrate();
	}
	TimerService_W80x::~TimerService_W80x()
	{
		//disable timer
		TIM->CR &= ~(1 << (_interruptTimer * 5 + 2));
		//disable interrupt
		TIM->CR &= ~(1 << (_interruptTimer * 5 + 3));
		switch(_interruptTimer)
		{
			case 0:
				Timer0CallBack = 0;
				break;
			case 1:
				Timer1CallBack = 0;
				break;
			case 2:
				Timer2CallBack = 0;
				break;
			case 3:
				Timer3CallBack = 0;
				break;
			case 4:
				Timer4CallBack = 0;
				break;
			case 5:
				Timer5CallBack = 0;
				break;
		}
	}
	void TimerService_W80x::ScheduleCallBack(const tick_t tick)
	{
		//disable timer
		TIM->CR &= ~_timerEn;
		const uint32_t res = tick - *_tick;
		if(res & 0x80000000)
			*_interruptPrd = 0;
		else
			*_interruptPrd = res;
		//enable timer
		TIM->CR |= _timerEn;
	}
	tick_t TimerService_W80x::GetTick()
	{
		return *_tick;
	}
	tick_t TimerService_W80x::GetTicksPerSecond()
	{
		return _ticksPerSecond;
	}
}

using namespace EmbeddedIOServices;

extern "C" __attribute__((isr)) void TIM0_5_IRQHandler(void)
{
	const uint32_t CRCache = TIM->CR;
	TIM->CR = CRCache | (0x10842108 & CRCache);
	if(CRCache & (1 << (0 * 5 + 4)) && TimerService_W80x::Timer0CallBack != 0)
		TimerService_W80x::Timer0CallBack();
	if(CRCache & (1 << (1 * 5 + 4)) && TimerService_W80x::Timer1CallBack != 0)
		TimerService_W80x::Timer1CallBack();
	if(CRCache & (1 << (2 * 5 + 4)) && TimerService_W80x::Timer2CallBack != 0)
		TimerService_W80x::Timer2CallBack();
	if(CRCache & (1 << (3 * 5 + 4)) && TimerService_W80x::Timer3CallBack != 0)
		TimerService_W80x::Timer3CallBack();
	if(CRCache & (1 << (4 * 5 + 4)) && TimerService_W80x::Timer4CallBack != 0)
		TimerService_W80x::Timer4CallBack();
	if(CRCache & (1 << (5 * 5 + 4)) && TimerService_W80x::Timer5CallBack != 0)
		TimerService_W80x::Timer5CallBack();
}
#endif
