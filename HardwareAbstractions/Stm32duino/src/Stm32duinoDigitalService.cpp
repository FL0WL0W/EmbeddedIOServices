#include <Arduino.h>
#include <stdint.h>
#include "Stm32duinoDigitalService.h"

using namespace EmbeddedIOServices;

namespace Stm32
{
	void Stm32duinoDigitalService::InitPin(uint16_t pin, PinDirection direction)
	{		
		if (pin == 0xFFFF)
			return;

		pinMode(pin, OUTPUT);
	}
	
	bool Stm32duinoDigitalService::ReadPin(uint16_t pin)
	{
		if (pin == 0xFFFF)
			return false;
		
		return digitalRead(pin);
	}
	
	void Stm32duinoDigitalService::WritePin(uint16_t pin, bool value)
	{
		if (pin == 0xFFFF)
			return;
		
  	digitalWrite(pin, value);
	}

	void Stm32duinoDigitalService::AttachInterrupt(uint16_t pin, std::function<void()> callBack)
	{
		if (pin == 0xFFFF)
			return;
		
		attachInterrupt(pin, callBack, CHANGE);
	}

	void Stm32duinoDigitalService::DetachInterrupt(uint16_t pin)
	{
		if (pin == 0xFFFF)
			return;
		
		detachInterrupt(pin);
	}
}
