#include "DigitalService_W80x.h"

#define RCC 	((RCC_TypeDef *)RCC_BASE)
#define GPIOA   ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB   ((GPIO_TypeDef *)GPIOB_BASE)

#ifdef DIGITALSERVICE_W80X_H
namespace EmbeddedIOServices
{
	void DigitalService_W80x::InitPin(digitalpin_t pin, PinDirection direction)
	{
		//Enable GPIO Clock
    	RCC->CLK_EN |= RCC_CLK_EN_GPIO;

		GPIO_TypeDef *GPIOx = pin > 31? GPIOB : GPIOA;
		const GPIOPin_W80x GPIOPin = PinToGPIOPin(pin);

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
	bool DigitalService_W80x::ReadPin(digitalpin_t pin)
	{
		GPIO_TypeDef *GPIOx = pin > 31? GPIOB : GPIOA;
		const GPIOPin_W80x GPIOPin = PinToGPIOPin(pin);
		
		return (GPIOx->DATA & GPIOPin) != 0;
	}
	void DigitalService_W80x::WritePin(digitalpin_t pin, bool value)
	{
		GPIO_TypeDef *GPIOx = pin > 31? GPIOB : GPIOA;
		const GPIOPin_W80x GPIOPin = PinToGPIOPin(pin);

		const uint32_t GPIODataEn = GPIOx->DATA_B_EN;
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
	void DigitalService_W80x::AttachInterrupt(digitalpin_t pin, callback_t callBack)
	{
		DetachInterrupt(pin);

		//Enable GPIO Interrupts with Highest Priority
		csi_vic_enable_irq(pin > 31? GPIOB_IRQn : GPIOA_IRQn);
		csi_vic_set_prio(pin > 31? GPIOB_IRQn : GPIOA_IRQn, 0);

		GPIO_TypeDef *GPIOx = pin > 31? GPIOB : GPIOA;
		const GPIOPin_W80x GPIOPin = PinToGPIOPin(pin);

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
		(pin > 31? GPIOBInterruptList : GPIOAInterruptList).push_front(DigitalInterrupt_W80x(GPIOPin, callBack));

		//Interrupt Enable
		GPIOx->IE |= GPIOPin;
	}
	void DigitalService_W80x::DetachInterrupt(digitalpin_t pin)
	{
		//Enable GPIO Clock
    	RCC->CLK_EN |= RCC_CLK_EN_GPIO;

		GPIO_TypeDef *GPIOx = pin > 31? GPIOB : GPIOA;
		const GPIOPin_W80x GPIOPin = PinToGPIOPin(pin);

		//Interrupt None
		GPIOx->IS &= ~GPIOPin;
		GPIOx->IBE &= ~GPIOPin;
		GPIOx->IEV &= ~GPIOPin;

		//Interrupt Disable
		GPIOx->IE &= ~GPIOPin;

		//Remove Interrupt from list
		(pin > 31? GPIOBInterruptList : GPIOAInterruptList).remove_if([GPIOPin](const DigitalInterrupt_W80x& interrupt) { return interrupt.GPIOPin == GPIOPin; });
	}
	
	DigitalInterruptList_W80x DigitalService_W80x::GPIOAInterruptList;
	DigitalInterruptList_W80x DigitalService_W80x::GPIOBInterruptList;
	GPIOPin_W80x DigitalService_W80x::PinToGPIOPin(digitalpin_t pin)
	{
		return 1 << (pin % 32);
	}
}

using namespace EmbeddedIOServices;

extern "C" __attribute__((section(".interrupt")))  __attribute__((isr)) void GPIOA_IRQHandler(void)
{
	GPIOB->DATA |= 1 << 4;
	GPIOB->DATA &= ~(1 << 4);
	for (DigitalInterruptList_W80x::iterator interrupt = DigitalService_W80x::GPIOAInterruptList.begin(); interrupt != DigitalService_W80x::GPIOAInterruptList.end(); ++interrupt)
	{
		if(interrupt->GPIOPin & GPIOA->MIS)
		{
			GPIOA->IC = interrupt->GPIOPin;
			interrupt->CallBack();
		}
	}
}

extern "C" __attribute__((section(".interrupt")))  __attribute__((isr)) void GPIOB_IRQHandler(void)
{
	for (DigitalInterruptList_W80x::iterator interrupt = DigitalService_W80x::GPIOBInterruptList.begin(); interrupt != DigitalService_W80x::GPIOBInterruptList.end(); ++interrupt)
	{
		if(interrupt->GPIOPin & GPIOB->MIS)
		{
			GPIOB->IC = interrupt->GPIOPin;
			interrupt->CallBack();
		}
	}
}
#endif
