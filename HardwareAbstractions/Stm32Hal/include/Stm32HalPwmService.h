#include "PinDirection.h"
#include "IPwmService.h"
#include "Stm32HalGpio.h"
#include "stdint.h"

#ifndef STM32HALPWMSERVICE_H
#define STM32HALPWMSERVICE_H

namespace Stm32
{
	class Stm32HalPwmService : public EmbeddedIOServices::IPwmService
	{
	public:
		void InitPin(uint16_t pin, EmbeddedIOServices::PinDirection direction, uint16_t minFrequency);
		EmbeddedIOServices::PwmValue ReadPin(uint16_t pin);
		void WritePin(uint16_t pin, EmbeddedIOServices::PwmValue value);
	};
}
#endif