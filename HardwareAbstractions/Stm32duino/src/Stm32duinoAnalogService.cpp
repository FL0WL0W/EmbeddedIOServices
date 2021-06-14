#include <stdint.h>
#include <Arduino.h>
#include "Stm32duinoAnalogService.h"

namespace Stm32
{
	void Stm32duinoAnalogService::InitPin(uint16_t pin)
	{
		if (pin == 0xFFFF)
			return;

    pinMode(pin, INPUT);
	}
	
	float Stm32duinoAnalogService::ReadPin(uint16_t pin)
	{
		return analogRead(pin) * ADC_VOLTAGE_CONVERSION_CONSTANT;
	}
}
