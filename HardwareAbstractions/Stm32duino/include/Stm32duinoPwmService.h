#include "PinDirection.h"
#include "IPwmService.h"
#include "stdint.h"

#ifndef STM32DUINOPWMSERVICE_H
#define STM32DUINOPWMSERVICE_H

namespace Stm32
{
	class Stm32duinoPwmService : public EmbeddedIOServices::IPwmService
	{
	public:
		void InitPin(uint16_t pin, EmbeddedIOServices::PinDirection direction, uint16_t minFrequency);
		EmbeddedIOServices::PwmValue ReadPin(uint16_t pin);
		void WritePin(uint16_t pin, EmbeddedIOServices::PwmValue value);
	};
}
#endif
