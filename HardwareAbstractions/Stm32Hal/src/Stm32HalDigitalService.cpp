#include <stdint.h>
#include "Stm32HalDigitalService.h"

unsigned short PinToGPIO_Pin(unsigned char pin)
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
	void Stm32HalDigitalService::InitPin(unsigned short pin, HardwareAbstraction::PinDirection direction)
	{		
		if (pin == 0)
			return;
		pin -= 1;
				
		//PA 1-16
		//PB 17-32
		//PC 33-48
		//PD 49-64
		//PE 65-80
		//PF 81-96
		//PG 97-112
		//PH 113-128
		//PI 129-144
		//PJ 145-160
		//PK 161-176
		GPIO_TypeDef *GPIO;
		switch(pin / 16)
		{
#ifdef GPIOA
		case 0: //PA
  			__HAL_RCC_GPIOA_CLK_ENABLE();
			GPIO = GPIOA;
			break;
#endif
#ifdef GPIOB
		case 1: //PB
  			__HAL_RCC_GPIOB_CLK_ENABLE();
			pin -= 16;
			GPIO = GPIOB;
			break;
#endif
#ifdef GPIOC
		case 2: //PC
  			__HAL_RCC_GPIOC_CLK_ENABLE();
			pin -= 32;
			GPIO = GPIOC;
			break;
#endif
#ifdef GPIOD
		case 3: //PD
  			__HAL_RCC_GPIOD_CLK_ENABLE();
			pin -= 48;
			GPIO = GPIOD;
			break;
#endif
#ifdef GPIOE
		case 4: //PE
  			__HAL_RCC_GPIOE_CLK_ENABLE();
			pin -= 64;
			GPIO = GPIOE;
			break;
#endif
#ifdef GPIOF
		case 5: //PF
  			__HAL_RCC_GPIOF_CLK_ENABLE();
			pin -= 80;
			GPIO = GPIOF;
			break;
#endif
#ifdef GPIOG
		case 6: //PG
  			__HAL_RCC_GPIOG_CLK_ENABLE();
			pin -= 96;
			GPIO = GPIOG;
			break;
#endif
#ifdef GPIOH
		case 7: //PH
  			__HAL_RCC_GPIOH_CLK_ENABLE();
			pin -= 112;
			GPIO = GPIOH;
			break;
#endif
#ifdef GPIOI
		case 8: //PI
  			__HAL_RCC_GPIOI_CLK_ENABLE();
			pin -= 128;
			GPIO = GPIOI;
			break;
#endif
#ifdef GPIOJ
		case 9: //PJ
  			__HAL_RCC_GPIOJ_CLK_ENABLE();
			pin -= 144;
			GPIO = GPIOJ;
			break;
#endif
#ifdef GPIOK
		case 10: //PK
  			__HAL_RCC_GPIOK_CLK_ENABLE();
			pin -= 160;
			GPIO = GPIOK;
			break;
#endif
		}

  		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = PinToGPIO_Pin(pin);
		GPIO_InitStruct.Mode = HardwareAbstraction::In ? GPIO_MODE_INPUT : GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIO, &GPIO_InitStruct);
	}
	
	bool Stm32HalDigitalService::ReadPin(unsigned short pin)
	{
		if (pin == 0)
			return false;
		pin -= 1;
		
		GPIO_TypeDef *GPIO;
		switch(pin / 16)
		{
#ifdef GPIOA
		case 0: //PA
			GPIO = GPIOA;
			break;
#endif
#ifdef GPIOB
		case 1: //PB
			pin -= 16;
			GPIO = GPIOB;
			break;
#endif
#ifdef GPIOC
		case 2: //PC
			pin -= 32;
			GPIO = GPIOC;
			break;
#endif
#ifdef GPIOD
		case 3: //PD
			pin -= 48;
			GPIO = GPIOD;
			break;
#endif
#ifdef GPIOE
		case 4: //PE
			pin -= 64;
			GPIO = GPIOE;
			break;
#endif
#ifdef GPIOF
		case 5: //PF
			pin -= 80;
			GPIO = GPIOF;
			break;
#endif
#ifdef GPIOG
		case 6: //PG
			pin -= 96;
			GPIO = GPIOG;
			break;
#endif
#ifdef GPIOH
		case 7: //PH
			pin -= 112;
			GPIO = GPIOH;
			break;
#endif
#ifdef GPIOI
		case 8: //PI
			pin -= 128;
			GPIO = GPIOI;
			break;
#endif
#ifdef GPIOJ
		case 9: //PJ
			pin -= 144;
			GPIO = GPIOJ;
			break;
#endif
#ifdef GPIOK
		case 10: //PK
			pin -= 160;
			GPIO = GPIOK;
			break;
#endif
		}
		return HAL_GPIO_ReadPin(GPIO, PinToGPIO_Pin(pin)) == GPIO_PIN_SET;
	}
	
	void Stm32HalDigitalService::WritePin(unsigned short pin, bool value)
	{
		if (pin == 0)
			return;
		pin -= 1;
		
		GPIO_TypeDef *GPIO;
		switch(pin / 16)
		{
#ifdef GPIOA
		case 0: //PA
			GPIO = GPIOA;
			break;
#endif
#ifdef GPIOB
		case 1: //PB
			pin -= 16;
			GPIO = GPIOB;
			break;
#endif
#ifdef GPIOC
		case 2: //PC
			pin -= 32;
			GPIO = GPIOC;
			break;
#endif
#ifdef GPIOD
		case 3: //PD
			pin -= 48;
			GPIO = GPIOD;
			break;
#endif
#ifdef GPIOE
		case 4: //PE
			pin -= 64;
			GPIO = GPIOE;
			break;
#endif
#ifdef GPIOF
		case 5: //PF
			pin -= 80;
			GPIO = GPIOF;
			break;
#endif
#ifdef GPIOG
		case 6: //PG
			pin -= 96;
			GPIO = GPIOG;
			break;
#endif
#ifdef GPIOH
		case 7: //PH
			pin -= 112;
			GPIO = GPIOH;
			break;
#endif
#ifdef GPIOI
		case 8: //PI
			pin -= 128;
			GPIO = GPIOI;
			break;
#endif
#ifdef GPIOJ
		case 9: //PJ
			pin -= 144;
			GPIO = GPIOJ;
			break;
#endif
#ifdef GPIOK
		case 10: //PK
			pin -= 160;
			GPIO = GPIOK;
			break;
#endif
		}

  		HAL_GPIO_WritePin(GPIO, PinToGPIO_Pin(pin), value? GPIO_PIN_SET : GPIO_PIN_RESET);
	}

	void Stm32HalDigitalService::ScheduleRecurringInterrupt(unsigned short pin, ICallBack *callBack)
	{
		//TODO
		//Setup Interrupt
	}

	void Stm32HalDigitalService::ScheduleNextInterrupt(unsigned short pin, ICallBack *callBack)
	{
		//TODO
	}
}