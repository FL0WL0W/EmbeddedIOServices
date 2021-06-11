#include "Stm32HalConf.h"
#include "ICallBack.h"
#include "stdint.h"

#ifndef STM32HALGPIO_H
#define STM32HALGPIO_H

namespace Stm32
{
    extern EmbeddedIOServices::ICallBack *ExternatlInterrupt0Callback;
    extern EmbeddedIOServices::ICallBack *ExternatlInterrupt1Callback;
    extern EmbeddedIOServices::ICallBack *ExternatlInterrupt2Callback;
    extern EmbeddedIOServices::ICallBack *ExternatlInterrupt3Callback;
    extern EmbeddedIOServices::ICallBack *ExternatlInterrupt4Callback;
    extern EmbeddedIOServices::ICallBack *ExternatlInterrupt5Callback;
    extern EmbeddedIOServices::ICallBack *ExternatlInterrupt6Callback;
    extern EmbeddedIOServices::ICallBack *ExternatlInterrupt7Callback;
    extern EmbeddedIOServices::ICallBack *ExternatlInterrupt8Callback;
    extern EmbeddedIOServices::ICallBack *ExternatlInterrupt9Callback;
    extern EmbeddedIOServices::ICallBack *ExternatlInterrupt10Callback;
    extern EmbeddedIOServices::ICallBack *ExternatlInterrupt11Callback;
    extern EmbeddedIOServices::ICallBack *ExternatlInterrupt12Callback;
    extern EmbeddedIOServices::ICallBack *ExternatlInterrupt13Callback;
    extern EmbeddedIOServices::ICallBack *ExternatlInterrupt14Callback;
    extern EmbeddedIOServices::ICallBack *ExternatlInterrupt15Callback;
    
    extern bool ExternatlInterrupt0DeleteOnExecution;
    extern bool ExternatlInterrupt1DeleteOnExecution;
    extern bool ExternatlInterrupt2DeleteOnExecution;
    extern bool ExternatlInterrupt3DeleteOnExecution;
    extern bool ExternatlInterrupt4DeleteOnExecution;
    extern bool ExternatlInterrupt5DeleteOnExecution;
    extern bool ExternatlInterrupt6DeleteOnExecution;
    extern bool ExternatlInterrupt7DeleteOnExecution;
    extern bool ExternatlInterrupt8DeleteOnExecution;
    extern bool ExternatlInterrupt9DeleteOnExecution;
    extern bool ExternatlInterrupt10DeleteOnExecution;
    extern bool ExternatlInterrupt11DeleteOnExecution;
    extern bool ExternatlInterrupt12DeleteOnExecution;
    extern bool ExternatlInterrupt13DeleteOnExecution;
    extern bool ExternatlInterrupt14DeleteOnExecution;
    extern bool ExternatlInterrupt15DeleteOnExecution;

    void EnableGPIOClock(uint16_t pin);
    void EnableInterrupt(uint16_t pin, EmbeddedIOServices::ICallBack *callBack, bool deleteOnExecution);
    void DisableInterrupt(uint16_t pin);
    
    constexpr uint16_t PinToGPIO_Pin(uint16_t pin)
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

        //satisfy the compile warnings. should probably check this after calling the function to make sure we actually returned something
        return 0;
    }
    
    constexpr GPIO_TypeDef *PinToGPIO(uint16_t pin)
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

        //satisfy the compile warnings. should probably check this after calling the function to make sure we actually returned something
        return 0;
    }
}

#endif