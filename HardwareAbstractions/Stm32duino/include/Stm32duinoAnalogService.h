#include "IAnalogService.h"
#include "Stm32duinoConf.h"
#include "stdint.h"

#ifndef STM32DUINOANALOGSERVICE_H
#define STM32DUINOANALOGSERVICE_H

namespace Stm32
{
	class Stm32duinoAnalogService : public EmbeddedIOServices::IAnalogService
	{
	public:
		void InitPin(uint16_t pin);
		float ReadPin(uint16_t pin);
	};
}

#endif
