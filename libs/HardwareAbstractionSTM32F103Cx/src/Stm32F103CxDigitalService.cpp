#include <stdint.h>
#include "Stm32F103CxDigitalService.h"

unsigned short PinToGPIO_Pin(uint8_t pin)
{
	switch (pin)
	{
	case 0:
		return 0x0001;
	case 1:
		return 0x0002;
	case 2:
		return 0x0004;
	case 3:
		return 0x0008;
	case 4:
		return 0x0010;
	case 5:
		return 0x0020;
	case 6:
		return 0x0040;
	case 7:
		return 0x0080;
	case 8:
		return 0x0100;
	case 9:
		return 0x0200;
	case 10:
		return 0x0400;
	case 11:
		return 0x0800;
	case 12:
		return 0x1000;
	case 13:
		return 0x2000;
	case 14:
		return 0x4000;
	case 15:
		return 0x8000;
	}
}

namespace Stm32
{
	void Stm32F103CxDigitalService::InitPin(uint8_t pin, HardwareAbstraction::PinDirection direction)
	{		
		if (pin == 0)
			return;
		pin -= 1;
				
		//PA 0-15
		//PB 16-31
		//PC 32-47
		switch(pin / 8)
		{
		case 0: //PA
			RCC->APB2ENR |= 0x00000004;
			GPIOA->CRL = (GPIOA->CRL & (0x0F << (pin << 2))) | ((((HardwareAbstraction::In ? 0x04 : 0x10) & 0x0F) | (HardwareAbstraction::In ? 0 : 0x03)) << (pin << 2));
			break;
		case 1: //PA
			RCC->APB2ENR |= 0x00000004;
			pin -= 8;
			GPIOA->CRH = (GPIOA->CRH & (0x0F << (pin << 2))) | ((((HardwareAbstraction::In ? 0x04 : 0x10) & 0x0F) | (HardwareAbstraction::In ? 0 : 0x03)) << (pin << 2));
			break;
		case 2: //PB
			RCC->APB2ENR |= 0x00000008;
			pin -= 16;
			GPIOB->CRL = (GPIOB->CRL & (0x0F << (pin << 2))) | ((((HardwareAbstraction::In ? 0x04 : 0x10) & 0x0F) | (HardwareAbstraction::In ? 0 : 0x03)) << (pin << 2));
			break;
		case 3: //PB
			RCC->APB2ENR |= 0x00000008;
			pin -= 24;
			GPIOB->CRH = (GPIOB->CRH & (0x0F << (pin << 2))) | ((((HardwareAbstraction::In ? 0x04 : 0x10) & 0x0F) | (HardwareAbstraction::In ? 0 : 0x03)) << (pin << 2));
			break;
		case 4: //PC
			RCC->APB2ENR |= 0x00000008;
			pin -= 16;
			GPIOC->CRL = (GPIOC->CRL & (0x0F << (pin << 2))) | ((((HardwareAbstraction::In ? 0x04 : 0x10) & 0x0F) | (HardwareAbstraction::In ? 0 : 0x03)) << (pin << 2));
			break;
		case 5: //PC
			RCC->APB2ENR |= 0x00000008;
			pin -= 24;
			GPIOC->CRH = (GPIOC->CRH & (0x0F << (pin << 2))) | ((((HardwareAbstraction::In ? 0x04 : 0x10) & 0x0F) | (HardwareAbstraction::In ? 0 : 0x03)) << (pin << 2));
			break;
		}
	}
	
	bool Stm32F103CxDigitalService::ReadPin(uint8_t pin)
	{
		if (pin == 0)
			return false;
		pin -= 1;
		
		switch (pin / 16)
		{
		case 0: //PA
			return GPIOA->IDR & PinToGPIO_Pin(pin);
		case 1: //PB
			return GPIOB->IDR & PinToGPIO_Pin(pin - 16);
		case 2: //PC
			return GPIOC->IDR & PinToGPIO_Pin(pin - 32);
		}
	}
	
	void Stm32F103CxDigitalService::WritePin(uint8_t pin, bool value)
	{
		if (pin == 0)
			return;
		pin -= 1;
		
		switch (pin / 16)
		{
		case 0: //PA
			if(value)
				GPIOA->BRR = PinToGPIO_Pin(pin);
			else
				GPIOA->BSRR = PinToGPIO_Pin(pin);
			break;
		case 1: //PB
			if(value)
				GPIOB->BRR = PinToGPIO_Pin(pin - 16);
			else
				GPIOB->BSRR = PinToGPIO_Pin(pin - 16);
			break;
		case 2: //PC
			if(value)
				GPIOC->BRR = PinToGPIO_Pin(pin - 32);
			else
				GPIOC->BSRR = PinToGPIO_Pin(pin - 32);
			break;
		}
	}

	void ScheduleRecurringInterrupt(unsigned char pin, Task *)
	{
		//TODO
		//Setup Interrupt
	}

	void ScheduleNextInterrupt(unsigned char pin, Task *)
	{
		//TODO
	}
}