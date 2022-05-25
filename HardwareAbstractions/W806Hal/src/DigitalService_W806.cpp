#include "DigitalService_W806.h"

#ifdef DIGITALSERVICE_W806_H
namespace EmbeddedIOServices
{
	void DigitalService_W806::InitPin(digitalpin_t pin, PinDirection direction)
	{
		//Enable GPIO Clock
    	((RCC_TypeDef *)RCC_BASE)->CLK_EN |= RCC_CLK_EN_GPIO;

		GPIO_TypeDef *GPIOx = pin > 31? GPIOB : GPIOA;
		uint32_t GPIOPin = PinToGPIOPin(pin);

		//GPIO
		GPIOx->AF_SEL &= ~GPIOPin;

		//Input/Output
		if(direction == Out)
			GPIOx->DIR |= GPIOPin;
		else
			GPIOx->DIR &= ~GPIOPin;

		//No Pull
		GPIOx->PULLUP_EN |= GPIOPin;
		GPIOx->PULLDOWN_EN &= ~GPIOPin;
	}
	bool DigitalService_W806::ReadPin(digitalpin_t pin)
	{
		GPIO_TypeDef *GPIOx = pin > 31? GPIOB : GPIOA;
		uint32_t GPIOPin = PinToGPIOPin(pin);
		
		return (GPIOx->DATA & GPIOPin) != 0;
	}
	void DigitalService_W806::WritePin(digitalpin_t pin, bool value)
	{
		GPIO_TypeDef *GPIOx = pin > 31? GPIOB : GPIOA;
		uint32_t GPIOPin = PinToGPIOPin(pin);

		uint32_t GPIODataEn = GPIOx->DATA_B_EN;
		GPIOx->DATA_B_EN |= GPIOPin;
		if (value)
		{
			GPIOx->DATA |= GPIOPin;
		}
		else
		{
			GPIOx->DATA &= ~GPIOPin;
		}
		GPIOx->DATA_B_EN = GPIODataEn;
	}
	void DigitalService_W806::AttachInterrupt(digitalpin_t pin, callback_t callBack)
	{
		//Enable GPIO Clock
    	((RCC_TypeDef *)RCC_BASE)->CLK_EN |= RCC_CLK_EN_GPIO;

		//Enable GPIO Interrupts with Highest Priority
		NVIC_SetPriority(pin > 31? GPIOB_IRQn : GPIOA_IRQn, 0);
		NVIC_EnableIRQ(pin > 31? GPIOB_IRQn : GPIOA_IRQn);

		GPIO_TypeDef *GPIOx = pin > 31? GPIOB : GPIOA;
		uint32_t GPIOPin = PinToGPIOPin(pin);

		//GPIO
		GPIOx->AF_SEL &= ~GPIOPin;

		//Input
		GPIOx->DIR &= ~GPIOPin;

		//No Pull
		GPIOx->PULLUP_EN |= GPIOPin;
		GPIOx->PULLDOWN_EN &= ~GPIOPin;

		//Interrupt Rising & Falling
		GPIOx->IS &= ~GPIOPin;
		GPIOx->IBE |= GPIOPin;
		GPIOx->IEV &= ~GPIOPin;

		//Add Interrupt to list
		(pin > 31? GPIOBInterruptList : GPIOAInterruptList).push_front(DigitalInterrupt(GPIOPin, callBack));

		//Interrupt Enable
		SET_BIT(GPIOx->IE, GPIOPin);
	}
	void DigitalService_W806::DetachInterrupt(digitalpin_t pin)
	{
		GPIO_TypeDef *GPIOx = pin > 31? GPIOB : GPIOA;
		uint32_t GPIOPin = PinToGPIOPin(pin);

		//Interrupt None
		GPIOx->IS &= ~GPIOPin;
		GPIOx->IBE &= ~GPIOPin;
		GPIOx->IEV &= ~GPIOPin;

		//Interrupt Disable
		(pin > 31? GPIOB : GPIOA)->IE &= ~GPIOPin;

		//Remove Interrupt from list
		(pin > 31? GPIOBInterruptList : GPIOAInterruptList).remove_if([GPIOPin](const DigitalInterrupt& interrupt) { return interrupt.GPIOPin == GPIOPin; });
	}
	
	DigitalInterruptList DigitalService_W806::GPIOAInterruptList;
	DigitalInterruptList DigitalService_W806::GPIOBInterruptList;
	uint32_t DigitalService_W806::PinToGPIOPin(digitalpin_t pin)
	{
		return 1 << (pin % 32);
	}
}

using namespace EmbeddedIOServices;

extern "C" __attribute__((isr)) void GPIOA_IRQHandler(void)
{
	for (DigitalInterruptList::iterator interrupt = DigitalService_W806::GPIOAInterruptList.begin(); interrupt != DigitalService_W806::GPIOAInterruptList.end(); ++interrupt)
	{
		if(interrupt->GPIOPin & GPIOA->MIS)
		{
			interrupt->CallBack();
		}
	}
	GPIOA->IC |= 0xFFFFFFFF;
}

extern "C" __attribute__((isr)) void GPIOB_IRQHandler(void)
{
	for (DigitalInterruptList::iterator interrupt = DigitalService_W806::GPIOBInterruptList.begin(); interrupt != DigitalService_W806::GPIOBInterruptList.end(); ++interrupt)
	{
		if(interrupt->GPIOPin & GPIOB->MIS)
		{
			interrupt->CallBack();
		}
	}
	GPIOB->IC |= 0xFFFFFFFF;
}
#endif
