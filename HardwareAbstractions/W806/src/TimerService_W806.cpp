#include "TimerService_W806.h"

#define RCC ((RCC_TypeDef *)RCC_BASE)
#define TIM ((TIM_TypeDef *)TIM_BASE)

#ifdef TIMERSERVICE_W806_H
namespace EmbeddedIOServices
{	
	TimerInterruptList TimerService_W806::InterruptList;

	TimerService_W806::TimerService_W806(uint8_t tickTimer, uint8_t interruptTimer) : _tick(&TIM->TIM0_CNT + tickTimer), _interruptTimer(interruptTimer), _interruptEn(3 << (interruptTimer * 5 + 2)), _interruptPrd(&TIM->TIM0_PRD + interruptTimer)
	{
		//Enable Timer Clock
    	RCC->CLK_EN |= RCC_CLK_EN_TIMER;

		//set timing divider value to apb clock - 1. 
		//this gives us microsecond resolution which is the expected standard.
		//for more resolution we could probably just set this value to 0
		const uint8_t apbclk = (480 / (0xFF & RCC->CLK_DIV)) / (0xFF & (RCC->CLK_DIV >> 16));
		TIM->TMR_CONFIG = apbclk-1;//this sets the tickrate to 1mhz
		// TIM->TMR_CONFIG = 0;//this sets the tickrate to 40mhz. However this does it for all timers
		_ticksPerSecond = (apbclk * 1000000) / (TIM->TMR_CONFIG+1);

		//reload at max uint32
		*(&TIM->TIM0_PRD + tickTimer) = UINT32_MAX;
		//set timer to microsecond resolution
		TIM->CR &= ~(1 << (tickTimer * 5));
		//set timer to autoreload
		TIM->CR &= ~(1 << (tickTimer * 5 + 1));
		//disable interrupt
		TIM->CR &= ~(1 << (tickTimer * 5 + 3));
		//enable timer
		TIM->CR |= (1 << (tickTimer * 5 + 2));
		
		csi_vic_enable_irq(TIM_IRQn);
		csi_vic_set_prio(TIM_IRQn, 1);

		//period at max uint32
		*(&TIM->TIM0_PRD + interruptTimer) = UINT32_MAX;
		//set timer to microsecond resolution
		TIM->CR &= ~(1 << (interruptTimer * 5));
		//set timer to not autoreload
		TIM->CR |= (1 << (interruptTimer * 5 + 1));
		//disable interrupt
		TIM->CR &= ~(1 << (interruptTimer * 5 + 3));
		//disable timer
		TIM->CR &= ~(1 << (interruptTimer * 5 + 2));
		InterruptList.push_front(TimerInterrupt(interruptTimer, [this](){TimerInterruptCallback();}));
		
		Calibrate();
	}
	TimerService_W806::~TimerService_W806()
	{
		InterruptList.remove_if([this](const TimerInterrupt& interrupt) { return interrupt.Timer == _interruptTimer; });
	}
	void TimerService_W806::ScheduleCallBack(const tick_t tick)
	{
		//disable timer and interrupt
		TIM->CR &= ~_interruptEn;
    	const uint32_t flags = csi_irq_save();
		const uint32_t res = *_interruptPrd = tick - *_tick;
		if(res & 0x80000000)
			*_interruptPrd = 0;
		//enable timer and interrupt
		TIM->CR |= _interruptEn;
    	csi_irq_restore(flags);
	}
	void TimerService_W806::TimerInterruptCallback()
	{
		//disable timer and interrupt
		TIM->CR &= ~_interruptEn;
		ReturnCallBack();
	}
	tick_t TimerService_W806::GetTick()
	{
		return *_tick;
	}
	tick_t TimerService_W806::GetTicksPerSecond()
	{
		return _ticksPerSecond;
	}
}

using namespace EmbeddedIOServices;

extern "C" __attribute__((section(".interrupt"))) __attribute__((isr)) void TIM0_5_IRQHandler(void)
{
	const uint32_t CRCache = TIM->CR;
	TIM->CR = CRCache | (0x10842108 & CRCache);
	for (TimerInterruptList::iterator interrupt = TimerService_W806::InterruptList.begin(); interrupt != TimerService_W806::InterruptList.end(); ++interrupt)
	{
		if((1 << (interrupt->Timer * 5 + 4)) & CRCache)
		{
			interrupt->CallBack();
		}
	}
}
#endif
