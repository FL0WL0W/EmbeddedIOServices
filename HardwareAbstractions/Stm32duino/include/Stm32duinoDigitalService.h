#include "PinDirection.h"
#include "IDigitalService.h"
#include "Stm32duinoConf.h"
#include "stdint.h"

#ifndef STM32DUINODIGITALSERVICE_H
#define STM32DUINODIGITALSERVICE_H

namespace Stm32
{
	class Stm32duinoDigitalService : public EmbeddedIOServices::IDigitalService
	{
	public:
		void InitPin(uint16_t pin, EmbeddedIOServices::PinDirection direction);
		bool ReadPin(uint16_t pin);
		void WritePin(uint16_t pin, bool value);
		void AttachInterrupt(uint16_t pin, std::function<void()>);
		void DetachInterrupt(uint16_t pin);
	};
}

#endif
