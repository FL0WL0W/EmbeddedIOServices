#include "PinDirection.h"
#include "IPwmService.h"
#include "stdint.h"

#ifndef ESP32IDFPWMSERVICE_H
#define ESP32IDFPWMSERVICE_H

namespace Esp32
{
	class Esp32IdfPwmService : public EmbeddedIOServices::IPwmService
	{
	public:
		void InitPin(uint16_t pin, EmbeddedIOServices::PinDirection direction, uint16_t minFrequency);
		EmbeddedIOServices::PwmValue ReadPin(uint16_t pin);
		void WritePin(uint16_t pin, EmbeddedIOServices::PwmValue value);
	};
}
#endif