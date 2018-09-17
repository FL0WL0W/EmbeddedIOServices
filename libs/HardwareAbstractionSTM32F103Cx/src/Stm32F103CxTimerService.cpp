#include <stdint.h>
#include <functional>
#include "Stm32F103CxTimerService.h"
#include "TimerLocks.h"

namespace Stm32
{
	Stm32F103CxTimerService *TimerService1 = 0;
	Stm32F103CxTimerService *TimerService2 = 0;
	Stm32F103CxTimerService *TimerService3 = 0;
	Stm32F103CxTimerService *TimerService4 = 0;
	
	Stm32F103CxTimerService::Stm32F103CxTimerService(unsigned char timer, unsigned char compareRegister, unsigned int ticksPerSecond)
	{	
		_compareRegister = compareRegister;
		
		switch (timer)
		{
		case 1:
			if (TIM1_Freq_Locked)
				return;
			NVIC->IP[25] = 0;
			NVIC->ISER[0] |= 0x01 << 25;
			NVIC->IP[27] = 0;
			NVIC->ISER[0] = 0x01 << 27;
			TIM1_Freq_Locked = true;
			TimerService1 = this;
			RCC->APB2ENR |= 0x00000800;
			TIM = TIM1;
		case 2:
			if (TIM2_Freq_Locked)
				return;
			NVIC->IP[28] = 0;
			NVIC->ISER[0] |= 0x01 << 28;
			TIM2_Freq_Locked = true;
			TimerService2 = this;
			RCC->APB1ENR |= 0x00000001;
			TIM = TIM2;
		case 3:
			if (TIM3_Freq_Locked)
				return;
			NVIC->IP[29] = 0;
			NVIC->ISER[0] |= 0x01 << 29;
			TIM3_Freq_Locked = true;
			TimerService3 = this;
			RCC->APB1ENR |= 0x00000002;
			TIM = TIM3;
		case 4:
			if (TIM4_Freq_Locked)
				return;
			NVIC->IP[30] = 0;
			NVIC->ISER[0] = 0x01 << 30;
			TIM4_Freq_Locked = true;
			TimerService4 = this;
			RCC->APB1ENR |= 0x00000004;
			TIM = TIM4;
		}
		
		//set prescaler
		TIM->PSC = (72 * 1000 * 1000) / ticksPerSecond;
		_ticksPerSecond = (72 * 1000 * 1000) / TIM->PSC;
		
		//set mode
		TIM->CR1 &= ~0x0370;
		TIM->ARR = 0xFFFF;
		
		TIM->CNT = 0;
		
		//enable timer
		TIM->CR1 |= 0x0001;
	
		unsigned short TIM_IT;
		
		/* Enable the timer global Interrupt */
		switch (compareRegister)
		{
		case 1:
			TIM_IT = 0x0002;
			TIM->CCER &= ~0x0001;
			TIM->CCMR1 = (TIM->CCMR1 & ~0x0073) | 0x0010;
			if (TIM == TIM1)
				TIM->CR2 &= ~0x0100;
			TIM->CCR1 = 0;
			TIM->CCER &= ~0x0002;
			TimerService1 = this;
			break;
		case 2:
			TIM_IT = 0x0004;
			TIM->CCER &= ~0x0010;
			TIM->CCMR1 = (TIM->CCMR1 & ~0x7300) | 0x1000;
			if (TIM == TIM1)
				TIM->CR2 &= ~0x0400;
			TIM->CCR2 = 0;
			TIM->CCER &= ~0x0020;
			TimerService2 = this;
			break;
		case 3:
			TIM_IT = 0x0008;
			TIM->CCER &= ~0x0100;
			TIM->CCMR2 = (TIM->CCMR1 & ~0x0073) | 0x0010;
			if (TIM == TIM1)
				TIM->CR2 &= ~0x1000;
			TIM->CCR3 = 0;
			TIM->CCER &= ~0x0200;
			TimerService3 = this;
			break;
		case 4:
			TIM_IT = 0x0010;
			TIM->CCER &= ~0x1000;
			TIM->CCMR2 = (TIM->CCMR1 & ~0x7300) | 0x1000;
			if (TIM == TIM1)
				TIM->CR2 &= ~0x4000;
			TIM->CCR4 = 0;
			TIM->CCER &= ~0x2000;
			TimerService4 = this;
			break;
		}
	
		TIM->SR = ~(TIM_IT | 0x0001);
		TIM->DIER |= 0x0001 | TIM_IT;
	}

	unsigned int Stm32F103CxTimerService::GetTick()
	{
		return _tick | TIM->CNT;
	}

	void Stm32F103CxTimerService::ScheduleCallBack(unsigned int tick)
	{
		//1 tick of overhead
		tick--;
		unsigned int counter = _tick | TIM->CNT;
		if (_tick == (tick & 0xFFFF0000))
		{
			if (tick <= counter)
			{
				ReturnCallBack();
			}
			else
			{
				_futureTock = false;
				_futureTick = true;
				switch (_compareRegister)
				{
				case 1:
					TIM->CCR1 = tick & 0xFFFF;
					break;
				case 2:
					TIM->CCR2 = tick & 0xFFFF;
					break;
				case 3:
					TIM->CCR3 = tick & 0xFFFF;
					break;
				case 4:
					TIM->CCR4 = tick & 0xFFFF;
					break;
				}
			}	
		}
		else if ((tick < counter && (counter - tick <= 2147483648)) || (tick > counter && (tick - counter > 2147483648)))
		{
			ReturnCallBack();
		}
		else
		{
			_futureTock = true;
			_futureTick = false;
			_callTick = tick;
		}
	}

	void Stm32F103CxTimerService::ReturnCallBack(void)
	{
		_futureTick = false;
		_futureTock = false;
		ITimerService::ReturnCallBack();
	}
	
	void Stm32F103CxTimerService::Interrupt(void)
	{
		unsigned short TIM_IT;
		switch (_compareRegister)
		{
		case 1:
			TIM_IT = 0x0002;
			break;
		case 2:
			TIM_IT = 0x0004;
			break;
		case 3:
			TIM_IT = 0x0008;
			break;
		case 4:
			TIM_IT = 0x0010;
			break;
		}
		
		if (TIM->SR & TIM_IT) {
			if (_futureTick)
			{
				ReturnCallBack();
			}
			TIM->SR = ~TIM_IT;
		}
		if (TIM->SR & 0x0001) {
			_tick += 0x00010000;	
			if (_futureTock)
			{
				ScheduleCallBack(_callTick);
			}
			TIM->SR = ~0x0001;
		}
	}

	unsigned int Stm32F103CxTimerService::GetTicksPerSecond()
	{
		return _ticksPerSecond;
	}
}