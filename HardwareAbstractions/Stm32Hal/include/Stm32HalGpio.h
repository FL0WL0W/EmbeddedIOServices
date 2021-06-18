#include "Stm32HalConf.h"
#include "stdint.h"
#include <functional>

#ifndef STM32HALGPIO_H
#define STM32HALGPIO_H

namespace Stm32
{
    extern std::function<void()> ExternatlInterruptCallback[16];

    void EnableGPIOClock(uint16_t pin);
    void attachInterrupt(uint16_t pin, std::function<void()> callBack);
    void detachInterrupt(uint16_t pin);
    
    constexpr uint16_t PinToGPIO_Pin(uint16_t pin)
    {
        switch (pin % 16)
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

        //satisfy the compile warnings. should probably check this after calling the function to make sure we actually returned something
        return 0;
    }
    
    constexpr GPIO_TypeDef *PinToGPIO(uint16_t pin)
    {
		switch(pin  / 16)
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

        //satisfy the compile warnings. should probably check this after calling the function to make sure we actually returned something
        return 0;
    }
}

#endif