#include "HardwareAbstraction/IAnalogService.h"
#include "Stm32HalConf.h"

#ifndef STM32HALANALOGSERVICE_H
#define STM32HALANALOGSERVICE_H

namespace Stm32
{
	class Stm32HalAnalogService : public HardwareAbstraction::IAnalogService
	{
	public:
		Stm32HalAnalogService();
		void InitPin(unsigned short pin);
		float ReadPin(unsigned short pin);
	};
}

#endif