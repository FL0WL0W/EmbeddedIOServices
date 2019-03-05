#include "Stm32HalConf.h"
#include "HardwareAbstraction/ICallBack.h"
#include "stdint.h"

using namespace HardwareAbstraction;

#ifndef STM32HALGPIO_H
#define STM32HALGPIO_H

namespace Stm32
{
    extern ICallBack *ExternatlInterrupt0Callback;
    extern ICallBack *ExternatlInterrupt1Callback;
    extern ICallBack *ExternatlInterrupt2Callback;
    extern ICallBack *ExternatlInterrupt3Callback;
    extern ICallBack *ExternatlInterrupt4Callback;
    extern ICallBack *ExternatlInterrupt5Callback;
    extern ICallBack *ExternatlInterrupt6Callback;
    extern ICallBack *ExternatlInterrupt7Callback;
    extern ICallBack *ExternatlInterrupt8Callback;
    extern ICallBack *ExternatlInterrupt9Callback;
    extern ICallBack *ExternatlInterrupt10Callback;
    extern ICallBack *ExternatlInterrupt11Callback;
    extern ICallBack *ExternatlInterrupt12Callback;
    extern ICallBack *ExternatlInterrupt13Callback;
    extern ICallBack *ExternatlInterrupt14Callback;
    extern ICallBack *ExternatlInterrupt15Callback;
    
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

    uint16_t PinToGPIO_Pin(uint16_t pin);
    GPIO_TypeDef *PinToGPIO(uint16_t pin);
    void EnableGPIOClock(uint16_t pin);
    void EnableInterrupt(uint16_t pin, ICallBack *callBack, bool deleteOnExecution);
    void DisableInterrupt(uint16_t pin);
}

#endif