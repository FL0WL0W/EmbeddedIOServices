#include "Stm32HalConf.h"

#ifndef STM32HALGPIO_H
#define STM32HALGPIO_H

namespace Stm32
{
    unsigned short PinToGPIO_Pin(unsigned char pin);
    GPIO_TypeDef *PinToGPIO(unsigned char pin);
    void EnableGPIOClock(unsigned char pin);
}

#endif