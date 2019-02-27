#include "Stm32HalGpio.h"

namespace Stm32
{
    uint16_t PinToGPIO_Pin(uint16_t pin)
    {
        switch ((pin - 1) % 16)
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
        }
    }

    GPIO_TypeDef *PinToGPIO(uint16_t pin)
    {
		switch((pin - 1) / 16)
		{
#ifdef GPIOA
		case 0: //PA
			return GPIOA;
#endif
#ifdef GPIOB
		case 1: //PB
			return GPIOB;
#endif
#ifdef GPIOC
		case 2: //PC
			return GPIOC;
#endif
#ifdef GPIOD
		case 3: //PD
			return GPIOD;
#endif
#ifdef GPIOE
		case 4: //PE
			return GPIOE;
#endif
#ifdef GPIOF
		case 5: //PF
			return GPIOF;
#endif
#ifdef GPIOG
		case 6: //PG
			return GPIOG;
#endif
#ifdef GPIOH
		case 7: //PH
			return GPIOH;
#endif
#ifdef GPIOI
		case 8: //PI
			return GPIOI;
#endif
#ifdef GPIOJ
		case 9: //PJ
			return GPIOJ;
#endif
#ifdef GPIOK
		case 10: //PK
			return GPIOK;
#endif
		}
    }
    
    void EnableGPIOClock(uint16_t pin)
    {
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
		switch((pin - 1) / 16)
		{
#ifdef GPIOA
		case 0: //PA
  			__HAL_RCC_GPIOA_CLK_ENABLE();
            break;
#endif
#ifdef GPIOB
		case 1: //PB
  			__HAL_RCC_GPIOB_CLK_ENABLE();
            break;
#endif
#ifdef GPIOC
		case 2: //PC
  			__HAL_RCC_GPIOC_CLK_ENABLE();
            break;
#endif
#ifdef GPIOD
		case 3: //PD
  			__HAL_RCC_GPIOD_CLK_ENABLE();
            break;
#endif
#ifdef GPIOE
		case 4: //PE
  			__HAL_RCC_GPIOE_CLK_ENABLE();
            break;
#endif
#ifdef GPIOF
		case 5: //PF
  			__HAL_RCC_GPIOF_CLK_ENABLE();
            break;
#endif
#ifdef GPIOG
		case 6: //PG
  			__HAL_RCC_GPIOG_CLK_ENABLE();
            break;
#endif
#ifdef GPIOH
		case 7: //PH
  			__HAL_RCC_GPIOH_CLK_ENABLE();
            break;
#endif
#ifdef GPIOI
		case 8: //PI
  			__HAL_RCC_GPIOI_CLK_ENABLE();
            break;
#endif
#ifdef GPIOJ
		case 9: //PJ
  			__HAL_RCC_GPIOJ_CLK_ENABLE();
            break;
#endif
#ifdef GPIOK
		case 10: //PK
  			__HAL_RCC_GPIO_CLK_ENABLE();
            break;
#endif
		}
    }
}