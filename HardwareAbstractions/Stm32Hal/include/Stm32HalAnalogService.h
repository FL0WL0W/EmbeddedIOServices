#include "HardwareAbstraction/IAnalogService.h"
#include "Stm32HalConf.h"
#include "Stm32HalGpio.h"
#include "stdint.h"

#ifndef STM32HALANALOGSERVICE_H
#define STM32HALANALOGSERVICE_H

namespace Stm32
{
	class Stm32HalAnalogService : public HardwareAbstraction::IAnalogService
	{
	public:
		Stm32HalAnalogService();
		void InitPin(uint16_t pin);
		float ReadPin(uint16_t pin);
	};
}

#endif