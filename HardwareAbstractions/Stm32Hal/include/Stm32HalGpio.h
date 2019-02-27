#include "Stm32HalConf.h"

#ifndef STM32HALGPIO_H
#define STM32HALGPIO_H

namespace Stm32
{
    uint16_t PinToGPIO_Pin(uint16_t pin);
    GPIO_TypeDef *PinToGPIO(uint16_t pin);
    void EnableGPIOClock(uint16_t pin);
}

#endif