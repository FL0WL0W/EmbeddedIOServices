#include "DigitalService_W806.h"

#ifdef DIGITALSERVICE_W806_H
namespace EmbeddedIOServices
{
	void DigitalService_W806::InitPin(digitalpin_t pin, PinDirection direction)
	{
		//Enable GPIO Clock
    	SET_BIT(RCC->CLK_EN, RCC_CLK_EN_GPIO);

		GPIO_TypeDef *GPIOx = pin > 31? GPIOB : GPIOA;
		uint32_t GPIOPin = PinToGPIOPin(pin);

		//GPIO
		CLEAR_BIT(GPIOx->AF_SEL, GPIOPin);

		//Input/Output
		if(direction == Out)
			SET_BIT(GPIOx->DIR, GPIOPin);
		else
			CLEAR_BIT(GPIOx->DIR, GPIOPin);

		//No Pull
		SET_BIT(GPIOx->PULLUP_EN, GPIOPin);
		CLEAR_BIT(GPIOx->PULLDOWN_EN, GPIOPin);
	}
	bool DigitalService_W806::ReadPin(digitalpin_t pin)
	{
		GPIO_TypeDef *GPIOx = pin > 31? GPIOB : GPIOA;
		uint32_t GPIOPin = PinToGPIOPin(pin);
		
		return (GPIOx->DATA & GPIOPin) != 0
	}
	void DigitalService_W806::WritePin(digitalpin_t pin, bool value)
	{
		GPIO_TypeDef *GPIOx = pin > 31? GPIOB : GPIOA;
		uint32_t GPIOPin = PinToGPIOPin(pin);

		uint32_t GPIODateEn = READ_REG(GPIOx->DATA_B_EN);
		SET_BIT(GPIOx->DATA_B_EN, GPIOPin);
		if (value)
		{
			SET_BIT(GPIOx->DATA, GPIOPin);
		}
		else
		{
			CLEAR_BIT(GPIOx->DATA, GPIOPin);
		}
		WRITE_REG(GPIOx->DATA_B_EN, GPIODateEn);
	}
	void DigitalService_W806::AttachInterrupt(digitalpin_t pin, callback_t callBack)
	{
		//Enable GPIO Clock
    	SET_BIT(RCC->CLK_EN, RCC_CLK_EN_GPIO);

		//Enable GPIO Interrupts with Highest Priority
		NVIC_SetPriority(pin > 31? GPIOB_IRQn : GPIOA_IRQn, 0);
		NVIC_EnableIRQ(pin > 31? GPIOB_IRQn : GPIOA_IRQn);

		GPIO_TypeDef *GPIOx = pin > 31? GPIOB : GPIOA;
		uint32_t GPIOPin = PinToGPIOPin(pin);

		//GPIO
		CLEAR_BIT(GPIOx->AF_SEL, GPIOPin);

		//Input
		CLEAR_BIT(GPIOx->DIR, GPIOPin);

		//No Pull
		SET_BIT(GPIOx->PULLUP_EN, GPIOPin);
		CLEAR_BIT(GPIOx->PULLDOWN_EN, GPIOPin);

		//Interrupt Rising & Falling
		CLEAR_BIT(GPIOx->IS, GPIOPin);
		SET_BIT(GPIOx->IBE, GPIOPin);
		CLEAR_BIT(GPIOx->IEV, GPIOPin);

		//Add Interrupt to list
		(pin > 31? GPIOBInterruptList : GPIOAInterruptList).push_front(Interrupt(GPIOPin, callBack));

		//Interrupt Enable
		SET_BIT(GPIOx->IE, GPIOPin);
	}
	void DigitalService_W806::DetachInterrupt(digitalpin_t pin)
	{
		uint32_t GPIOPin = PinToGPIOPin(pin);

		//Interrupt None
		CLEAR_BIT(GPIOx->IS, GPIOPin);
		CLEAR_BIT(GPIOx->IBE, GPIOPin);
		CLEAR_BIT(GPIOx->IEV, GPIOPin);

		//Interrupt Disable
		Clear_BIT((pin > 31? GPIOB : GPIOA)->IE, GPIOPin);

		//Remove Interrupt from list
		(pin > 31? GPIOBInterruptList : GPIOAInterruptList).remove_if([](const Interrupt& interrupt) { return interrupt.GPIOPin == GPIOPin; });
	}
	
	InterruptList DigitalService_W806::GPIOAInterruptList;
	InterruptList DigitalService_W806::GPIOBInterruptList;
	uint32_t DigitalService_W806::PinToGPIOPin(digitalpin_t pin)
	{
		switch(pin % 32)
		{
			case 0:
				return GPIO_PIN_0;
			case 1:
				return GPIO_PIN_1;
			case 2:
				return GPIO_PIN_2;
			case 3:
				return GPIO_PIN_3;
			case 4:
				return GPIO_PIN_4;
			case 5:
				return GPIO_PIN_5;
			case 6:
				return GPIO_PIN_6;
			case 7:
				return GPIO_PIN_7;
			case 8:
				return GPIO_PIN_8;
			case 9:
				return GPIO_PIN_9;
			case 10:
				return GPIO_PIN_10;
			case 11:
				return GPIO_PIN_11;
			case 12:
				return GPIO_PIN_12;
			case 13:
				return GPIO_PIN_13;
			case 14:
				return GPIO_PIN_14;
			case 15:
				return GPIO_PIN_15;
			case 16:
				return GPIO_PIN_16;
			case 17:
				return GPIO_PIN_17;
			case 18:
				return GPIO_PIN_18;
			case 19:
				return GPIO_PIN_19;
			case 20:
				return GPIO_PIN_20;
			case 21:
				return GPIO_PIN_21;
			case 22:
				return GPIO_PIN_22;
			case 23:
				return GPIO_PIN_23;
			case 24:
				return GPIO_PIN_24;
			case 25:
				return GPIO_PIN_25;
			case 26:
				return GPIO_PIN_26;
			case 27:
				return GPIO_PIN_27;
			case 28:
				return GPIO_PIN_28;
			case 29:
				return GPIO_PIN_29;
			case 30:
				return GPIO_PIN_30;
			case 31:
				return GPIO_PIN_31;
		}
	}
}

using namespace EmbeddedIOServices;

__attribute__((isr)) void GPIOA_IRQHandler(void)
{
	InterruptList::iterator interrupt;
	while((interrupt = DigitalService_W806::GPIOAInterruptList.begin()) != DigitalService_W806::GPIOAInterruptList.end())
	{
		if((interrupt->GPIOPin & GPIOA->MIS) != 0)
		{
			interrupt->CallBack();
		}
	}
	GPIOA->IC = 0;
}

__attribute__((isr)) void GPIOB_IRQHandler(void)
{
	InterruptList::iterator interrupt;
	while((interrupt = DigitalService_W806::GPIOBInterruptList.begin()) != DigitalService_W806::GPIOBInterruptList.end())
	{
		if((interrupt->GPIOPin & GPIOB->MIS) != 0)
		{
			interrupt->CallBack();
		}
	}
	GPIOB->IC = 0;
}
#endif
