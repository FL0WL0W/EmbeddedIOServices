#include <stdint.h>
#include <Arduino.h>
#include "Stm32duinoDigitalService.h"

using namespace EmbeddedIOServices;

namespace Stm32
{
	void Stm32duinoDigitalService::InitPin(uint16_t pin, PinDirection direction)
	{		
		if (pin == 0xFFFF)
			return;

		pinMode(pin, direction == In ? INPUT : OUTPUT);
	}
	
	bool Stm32duinoDigitalService::ReadPin(uint16_t pin)
	{
		if (pin == 0xFFFF)
			return false;
		
		return digitalRead(pin) == HIGH;
	}
	
	void Stm32duinoDigitalService::WritePin(uint16_t pin, bool value)
	{
		if (pin == 0xFFFF)
			return;

    if(value)
  	  digitalWrite(pin, HIGH);
    else
      digitalWrite(pin, LOW);
	}

	void Stm32duinoDigitalService::ScheduleRecurringInterrupt(uint16_t pin, ICallBack *callBack)
	{
    attachInterrupt(pin, std::bind([](ICallBack *CallBack) { CallBack->Execute(); }, callBack), CHANGE);
	}

	void Stm32duinoDigitalService::ScheduleNextInterrupt(uint16_t pin, ICallBack *callBack)
	{
    attachInterrupt(pin, std::bind([](ICallBack *CallBack) { CallBack->Execute(); delete CallBack; }, callBack), CHANGE);
	}
}
