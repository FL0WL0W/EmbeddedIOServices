#include "Stm32F103CxPwmService.h"
#include <algorithm>
#include <math.h>
#include "TimerLocks.h"

namespace Stm32
{
	Stm32F103CxPwmService *PwmService = 0;
	
	Stm32F103CxPwmService::Stm32F103CxPwmService()
	{
		PwmService = this;
	}
	
	void Stm32F103CxPwmService::InitPin(unsigned char pin, HardwareAbstraction::PinDirection direction, unsigned short minFrequency)
	{
		if (pin == 0)
			return;
		
		uint16_t TIM_Channel;
		TIM_TypeDef *TIM;
		GPIO_TypeDef *GPIO;
		
		switch (pin)
		{
		case 1:
			TIM_Channel = 0x0000;
			TIM = TIM2;
			GPIO = GPIOA;
			pin = 0;
			RCC->APB1ENR |= 0x00000001;
			break;
		case 2:
			TIM_Channel = 0x0004;
			TIM = TIM2;
			GPIO = GPIOA;
			pin = 1;
			RCC->APB1ENR |= 0x00000001;
			break;
		case 3:
			TIM_Channel = 0x0008;
			TIM = TIM2;
			GPIO = GPIOA;
			pin = 2;
			RCC->APB1ENR |= 0x00000001;
			break;
		case 4:
			TIM_Channel = 0x000C;
			TIM = TIM2;
			GPIO = GPIOA;
			pin = 3;
			RCC->APB1ENR |= 0x00000001;
			break;
		case 7:
			TIM_Channel = 0x0000;
			TIM = TIM3;
			GPIO = GPIOA;
			pin = 6;
			RCC->APB1ENR |= 0x00000002;
			break;
		case 8:
			TIM_Channel = 0x0004;
			TIM = TIM3;
			GPIO = GPIOA;
			pin = 7;
			RCC->APB1ENR |= 0x00000002;
			break;
		case 9:
			TIM_Channel = 0x0000;
			TIM = TIM1;
			GPIO = GPIOA;
			pin = 8;
			RCC->APB2ENR |= 0x00000800;
			break;
		case 10:
			TIM_Channel = 0x0004;
			TIM = TIM1;
			GPIO = GPIOA;
			pin = 9;
			RCC->APB2ENR |= 0x00000800;
			break;
		case 11:
			TIM_Channel = 0x0008;
			TIM = TIM1;
			GPIO = GPIOA;
			pin = 10;
			RCC->APB2ENR |= 0x00000800;
			break;
		case 12:
			TIM_Channel = 0x000C;
			TIM = TIM1;
			GPIO = GPIOA;
			pin = 11;
			RCC->APB2ENR |= 0x00000800;
			break;
		case 17:
			TIM_Channel = 0x0008;
			TIM = TIM3;
			GPIO = GPIOB;
			pin = 0;
			RCC->APB1ENR |= 0x00000002;
			break;
		case 18:
			TIM_Channel = 0x000C;
			TIM = TIM3;
			GPIO = GPIOB;
			pin = 1;
			RCC->APB1ENR |= 0x00000002;
			break;
		case 23:
			TIM_Channel = 0x0000;
			TIM = TIM4;
			GPIO = GPIOB;
			pin = 6;
			RCC->APB1ENR |= 0x00000004;
			break;
		case 24:
			TIM_Channel = 0x0004;
			TIM = TIM4;
			GPIO = GPIOB;
			pin = 7;
			RCC->APB1ENR |= 0x00000004;
			break;
		case 25:
			TIM_Channel = 0x0008;
			TIM = TIM4;
			GPIO = GPIOB;
			pin = 8;
			RCC->APB1ENR |= 0x00000004;
			break;
		case 26:
			TIM_Channel = 0x000C;
			TIM = TIM4;
			GPIO = GPIOB;
			pin = 9;
			RCC->APB1ENR |= 0x00000004;
			break;
		}
				
		if (direction == HardwareAbstraction::Out)
		{
			if (pin > 7)
				GPIO->CRH = (GPIO->CRH & ~(0x0F << ((pin - 8) << 2))) | (0x0B << ((pin - 8) << 2));
			else
				GPIO->CRL = (GPIO->CRL & ~(0x0F << (pin << 2))) | (0x0B << (pin << 2));
			
			if (!((TIM == TIM1 && (TIM1_Freq_Locked || TIM1_Input)) || (TIM == TIM2 && (TIM2_Freq_Locked || TIM2_Input)) || (TIM == TIM3 && (TIM3_Freq_Locked || TIM1_Input)) || (TIM == TIM4 && (TIM4_Freq_Locked || TIM4_Input))))
			{
				//set the frequency and mode
				TIM->CR1 &= ~0x0370;
				TIM->PSC = std::max((unsigned short)1, (unsigned short)ceil(((72 * 1000 * 1000.0) / 65535) * 1));
				TIM->ARR = (int)ceil(((72 * 1000 * 1000.0) / TIM->PSC) * 1);
			}
			//make sure we are in the right mode
			else if (TIM->CR1 & 0x0370 == 0)
				return; 

			switch (TIM_Channel)
			{
			case 0x0000:
				TIM->CCER &= ~0x0001;
				TIM->CCMR1 = (TIM->CCMR1 & ~0x0073) | 0x0060;
				if (TIM == TIM1)
					TIM->CR2 |= 0x0100;
				TIM->CCR1 = 0;
				TIM->CCER = (TIM->CCER & ~0x0002) | 0x0001;
				break;
			case 0x0004:
				TIM->CCER &= ~0x0010;
				TIM->CCMR1 = (TIM->CCMR1 & ~0x7300) | 0x6000;
				if (TIM == TIM1)
					TIM->CR2 |= 0x0400;
				TIM->CCR2 = 0;
				TIM->CCER = (TIM->CCER & ~0x0020) | 0x0010;
				break;
			case 0x0008:
				TIM->CCER &= ~0x0100;
				TIM->CCMR2 = (TIM->CCMR2 & ~0x0073) | 0x0060;
				if (TIM == TIM1)
					TIM->CR2 |= 0x1000;
				TIM->CCR3 = 0;
				TIM->CCER = (TIM->CCER & ~0x0200) | 0x0100;
				break;
			case 0x000C:
				TIM->CCER &= ~0x1000;
				TIM->CCMR2 = (TIM->CCMR2 & ~0x7300) | 0x6000;
				if (TIM == TIM1)
					TIM->CR2 = 0x4000;
				TIM->CCR4 = 0;
				TIM->CCER = (TIM->CCER & ~0x2000) | 0x1000;
				break;
			}

			//enable timer
			TIM->CR1 |= 0x0001;
			
			//enable pwm output
			TIM->BDTR |= 0x8000;
		}
		else
		{
			if (pin > 7)
				GPIO->CRH = (GPIO->CRH & ~(0x0F << ((pin - 8) << 2))) | (0x04 << ((pin - 8) << 2));
			else
				GPIO->CRL = (GPIO->CRL & ~(0x0F << (pin << 2))) | (0x04 << (pin << 2));
			
			if (!((TIM == TIM1 && (TIM1_Freq_Locked)) || (TIM == TIM2 && (TIM2_Freq_Locked)) || (TIM == TIM3 && (TIM3_Freq_Locked)) || (TIM == TIM4 && (TIM4_Freq_Locked))))
			{
				//set the frequency and mode
				TIM->CR1 &= ~0x0370;
				TIM->PSC = std::max((unsigned short)TIM->PSC, (unsigned short)ceil(((72 * 1000 * 1000.0) / 65535) / minFrequency));
				TIM->ARR = 0xFFFF;
			}
			//make sure we are in the right mode
			else if (TIM->CR1 & 0x0370 == 0)
				return;   
			
			//enable intterupt
			if (TIM == TIM1)
			{
				NVIC->IP[25] = 0;
				NVIC->ISER[0] |= 0x01 << 25;
				NVIC->IP[27] = 0;
				NVIC->ISER[0] |= 0x01 << 27;
			}
			else if (TIM == TIM2)
			{
				NVIC->IP[28] = 0;
				NVIC->ISER[0] |= 0x01 << 28;
			}
			else if (TIM == TIM3)
			{
				NVIC->IP[29] = 0;
				NVIC->ISER[29] |= 0x01 << 29;
			}
			else if (TIM == TIM4)
			{
				NVIC->IP[30] = 0;
				NVIC->ISER[0] |= 0x01 << 30;
			}
												
			switch (TIM_Channel)
			{
			case 0x0000:
				TIM->SR = ~0x0006;
				TIM->DIER |= 0x0006;
				TIM->CCER &= ~0x0011;
				TIM->CCMR1 = (TIM->CCMR1 & ~0xFFFF) | 0x0201;
				TIM->CCER = (TIM->CCER & ~0x0022) | 0x0031;
			case 0x0004:
				TIM->SR = ~0x0006;
				TIM->DIER |= 0x0006;
				TIM->CCER &= ~0x0011;
				TIM->CCMR1 = (TIM->CCMR1 & ~0xFFFF) | 0x0102;
				TIM->CCER = (TIM->CCER & ~0x0022) | 0x0031;
			case 0x0008:
				TIM->SR = ~0x0018;
				TIM->DIER |= 0x0018;
				TIM->CCER &= ~0x1100;
				TIM->CCMR2 = (TIM->CCMR2 & ~0xFFFF) | 0x0201;
				TIM->CCER = (TIM->CCER & ~0x2200) | 0x3100;
			case 0x000C:
				TIM->SR = ~0x0018;
				TIM->DIER |= 0x0018;
				TIM->CCER &= ~0x1100;
				TIM->CCMR2 = (TIM->CCMR2 & ~0xFFFF) | 0x0102;
				TIM->CCER = (TIM->CCER & ~0x2200) | 0x3100;
			}
			
			//enable timer
			TIM->CR1 |= 0x0001;
		}
	}
	
	void Stm32F103CxPwmService::InterruptTim1()
	{
		if (TIM4->SR & 0x0002 && TIM4->DIER & 0x0002)
		{
			_prevCC[0][0] = _currCC[0][0];
			_currCC[0][0] = TIM1->CCR1;
		}
		if (TIM4->SR & 0x0004 && TIM4->DIER & 0x0004)
		{
			_prevCC[0][1] = _currCC[0][1];
			_currCC[0][1] = TIM1->CCR2;
		}
		if (TIM4->SR & 0x0008 && TIM4->DIER & 0x0008)
		{
			_prevCC[0][2] = _currCC[0][2];
			_currCC[0][2] = TIM1->CCR3;
		}
		if (TIM4->SR & 0x0010 && TIM4->DIER & 0x0010)
		{
			_prevCC[0][3] = _currCC[0][3];
			_currCC[0][3] = TIM1->CCR4;
		}
	}
	
	void Stm32F103CxPwmService::InterruptTim2()
	{
		if (TIM2->SR & 0x0002 && TIM2->DIER & 0x0002)
		{
			_prevCC[1][0] = _currCC[1][0];
			_currCC[1][0] = TIM2->CCR1;
		}
		if (TIM2->SR & 0x0004 && TIM2->DIER & 0x0004)
		{
			_prevCC[1][1] = _currCC[1][1];
			_currCC[1][1] = TIM2->CCR2;
		}
		if (TIM2->SR & 0x0008 && TIM2->DIER & 0x0008)
		{
			_prevCC[1][2] = _currCC[1][2];
			_currCC[1][2] = TIM2->CCR3;
		}
		if (TIM2->SR & 0x0010 && TIM2->DIER & 0x0010)
		{
			_prevCC[1][3] = _currCC[1][3];
			_currCC[1][3] = TIM2->CCR4;
		}
	}
	
	void Stm32F103CxPwmService::InterruptTim3()
	{
		if (TIM3->SR & 0x0002 && TIM3->DIER & 0x0002)
		{
			_prevCC[2][0] = _currCC[2][0];
			_currCC[2][0] = TIM3->CCR1;
		}
		if (TIM3->SR & 0x0004 && TIM3->DIER & 0x0004)
		{
			_prevCC[2][1] = _currCC[2][1];
			_currCC[2][1] = TIM3->CCR2;
		}
		if (TIM3->SR & 0x0008 && TIM3->DIER & 0x0008)
		{
			_prevCC[2][2] = _currCC[2][2];
			_currCC[2][2] = TIM3->CCR3;
		}
		if (TIM3->SR & 0x0010 && TIM3->DIER & 0x0010)
		{
			_prevCC[2][3] = _currCC[2][3];
			_currCC[2][3] = TIM3->CCR4;
		}
	}
	
	void Stm32F103CxPwmService::InterruptTim4()
	{
		if (TIM4->SR & 0x0002 && TIM4->DIER & 0x0002)
		{
			_prevCC[3][0] = _currCC[3][0];
			_currCC[3][0] = TIM4->CCR1;
		}
		if (TIM4->SR & 0x0004 && TIM4->DIER & 0x0004)
		{
			_prevCC[3][1] = _currCC[3][1];
			_currCC[3][1] = TIM4->CCR2;
		}
		if (TIM4->SR & 0x0008 && TIM4->DIER & 0x0008)
		{
			_prevCC[3][2] = _currCC[3][2];
			_currCC[3][2] = TIM4->CCR3;
		}
		if (TIM4->SR & 0x0010 && TIM4->DIER & 0x0010)
		{
			_prevCC[3][3] = _currCC[3][3];
			_currCC[3][3] = TIM4->CCR4;
		}
	}
		
	HardwareAbstraction::PwmValue Stm32F103CxPwmService::ReadPin(unsigned char pin)
	{
		HardwareAbstraction::PwmValue value = HardwareAbstraction::PwmValue();
		if (pin == 0)
			return value;
		
		unsigned char timerMinus1 = 0;;
		unsigned char iCMinus1 = 0;
		unsigned char iCMinus1Neg = 0;
		switch (pin)
		{
		case 1:
		case 2:
			timerMinus1 = 1;
			iCMinus1 = 0;
			iCMinus1Neg = 1;
			break;
		case 3 :
		case 4 :
			timerMinus1 = 1;
			iCMinus1 = 2;
			iCMinus1Neg = 3;
			break;
		case 7 :
		case 8 :
			timerMinus1 = 2;
			iCMinus1 = 0;
			iCMinus1Neg = 1;
			break;
		case 9 :
		case 10 :
			timerMinus1 = 0;
			iCMinus1 = 0;
			iCMinus1Neg = 1;
			break;
		case 11 :
		case 12 :
			timerMinus1 = 0;
			iCMinus1 = 2;
			iCMinus1Neg = 3;
			break;
		case 17 :
		case 18 :
			timerMinus1 = 2;
			iCMinus1 = 2;
			iCMinus1Neg = 3;
			break;
		case 23 :
		case 24 :
			timerMinus1 = 3;
			iCMinus1 = 0;
			iCMinus1Neg = 1;
			break;
		case 25 :
		case 26 :
			timerMinus1 = 3;
			iCMinus1 = 2;
			iCMinus1Neg = 3;
			break;
		}
		
		int pulseTick = (int)_currCC[timerMinus1][iCMinus1Neg] - _currCC[timerMinus1][iCMinus1];
		if(pulseTick < 0)
			pulseTick = (int)_currCC[timerMinus1][iCMinus1Neg] - _prevCC[timerMinus1][iCMinus1];
		if (pulseTick < 0)
			pulseTick = ((int)_currCC[timerMinus1][iCMinus1Neg] - _currCC[timerMinus1][iCMinus1]) + 65535;
		unsigned short periodTick = _currCC[timerMinus1][iCMinus1] - _prevCC[timerMinus1][iCMinus1];
		
		unsigned short prescaler = 1;
		switch (timerMinus1) 
		{
		case 0:
			prescaler = TIM1->PSC;
			break;
		case 1:
			prescaler = TIM2->PSC;
			break;
		case 2:
			prescaler = TIM3->PSC;
			break;
		case 3:
			prescaler = TIM4->PSC;
			break;
		}
		
		value.PulseWidth = pulseTick * (prescaler / (72 * 1000 * 1000.0));
		value.Period = periodTick * (prescaler / (72 * 1000 * 1000.0));
		
		return value;
	}
	
	void Stm32F103CxPwmService::WritePin(unsigned char pin, HardwareAbstraction::PwmValue value)
	{
		if (pin == 0)
			return;
		
		TIM_TypeDef *TIM;
		unsigned char oC;
		switch (pin)
		{
		case 1:
			oC = 1;
			TIM = TIM2;
			break;
		case 2:
			oC = 2;
			TIM = TIM2;
			break;
		case 3:
			oC = 3;
			TIM = TIM2;
			break;
		case 4:
			oC = 4;
			TIM = TIM2;
			break;
		case 7:
			oC = 1;
			TIM = TIM3;
			break;
		case 8:
			oC = 2;
			TIM = TIM3;
			break;
		case 9:
			oC = 1;
			TIM = TIM1;
			break;
		case 10:
			oC = 2;
			TIM = TIM1;
			break;
		case 11:
			oC = 3;
			TIM = TIM1;
			break;
		case 12:
			oC = 4;
			TIM = TIM1;
			break;
		case 17:
			oC = 3;
			TIM = TIM3;
			break;
		case 18:
			oC = 4;
			TIM = TIM3;
			break;
		case 23:
			oC = 1;
			TIM = TIM4;
			break;
		case 24:
			oC = 2;
			TIM = TIM4;
			break;
		case 25:
			oC = 3;
			TIM = TIM4;
			break;
		case 26:
			oC = 4;
			TIM = TIM4;
			break;
		}
		
		if (!((TIM == TIM1 && (TIM1_Freq_Locked || TIM1_Input)) || (TIM == TIM2 && (TIM2_Freq_Locked || TIM2_Input)) || (TIM == TIM3 && (TIM3_Freq_Locked || TIM1_Input)) || (TIM == TIM4 && (TIM4_Freq_Locked || TIM4_Input))))
		{
			//Set period
			TIM->PSC = std::max(1, (int)ceil(((72 * 1000 * 1000.0) / 65535) * value.Period));
			TIM->ARR = (int)ceil(((72 * 1000 * 1000.0) / TIM->PSC) * value.Period);
		}

		//set pulse width
		switch (oC)
		{
		case 1:
			TIM->CCR1 = TIM->ARR * (value.PulseWidth / value.Period);
			break;
		case 2:
			TIM->CCR2 = TIM->ARR * (value.PulseWidth / value.Period);
			break;
		case 3:
			TIM->CCR3 = TIM->ARR * (value.PulseWidth / value.Period);
			break;
		case 4:
			TIM->CCR4 = TIM->ARR * (value.PulseWidth / value.Period);
		}
	}
}