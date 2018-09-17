#include <stdint.h>
#include "Stm32F103CxAnalogService.h"

#define RCC_PCLK2_Mask                   ((uint32_t)0xFFFF3FFF)
#define RCC_PCLK2_Div2                   ((uint32_t)0x00000000)
#define RCC_PCLK2_Div4                   ((uint32_t)0x00004000)
#define RCC_PCLK2_Div6                   ((uint32_t)0x00008000)
#define RCC_PCLK2_Div8                   ((uint32_t)0x0000C000)

namespace Stm32
{
	Stm32F103CxAnalogService::Stm32F103CxAnalogService()
	{
		RCC->CFGR = (RCC->CFGR & 0xFFFF3FFF) | 0x00008000;
		ADC1->CR1 &= 0xFFF0FEFF;
		ADC1->CR2 = (ADC1->CR2 & 0xFFF1F7FD) | 0x000E0000;
		ADC1->SQR1 &= 0xFF0FFFFF;
		ADC1->CR2 |= 0x00000001;

		//calibrate the ADC
		ADC1->CR2 |= 0x00000008;
		while (ADC1->CR2 & 0x00000008) ;
		ADC1->CR2 |= 0x00000004;
		while (ADC1->CR2 & 0x00000004) ;
	}

	void Stm32F103CxAnalogService::InitPin(unsigned char pin)
	{
		if (pin == 0)
			return;
		pin -= 1;
		
		if (pin < 8)
		{
			GPIOA->CRL = (GPIOA->CRL & (0x0F << pin)) | (0x03 << pin);
		}
		else if (pin > 15 && pin < 18)
		{
			GPIOB->CRL = (GPIOB->CRL & (0x0F << (pin - 16))) | (0x03 << (pin-16));
		}
	}
	
	float Stm32F103CxAnalogService::ReadPin(unsigned char pin)
	{
		if (pin == 0)
			return 0;
		pin -= 1;
		if (pin > 15)
			pin -= 8;
		
		//set the channel to the pin we want to read
		ADC1->SQR3 = (ADC1->SQR3 & ~0x1F) | pin;
			
		//start the conversion
		ADC1->CR2 |= 0x00500000;
		//wait for conversion to finish
		while(!(ADC1->SR & 0x02)) ;
		//return the value
		return ADC1->DR * 0.000244140625f * 3.3;   //convert from 12 bit to float and convert to voltage
	}
}