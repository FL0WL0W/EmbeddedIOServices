#include "PinDirection.h"
#include "IDigitalService.h"
#include "Esp32IdfConf.h"
#include "stdint.h"

#ifndef ESP32IDFDIGITALSERVICE_H
#define ESP32IDFDIGITALSERVICE_H

namespace Esp32
{
	class Esp32IdfDigitalService : public EmbeddedIOServices::IDigitalService
	{
	public:
		void InitPin(uint16_t pin, EmbeddedIOServices::PinDirection direction);
		bool ReadPin(uint16_t pin);
		void WritePin(uint16_t pin, bool value);
		void AttachInterrupt(uint16_t pin, EmbeddedIOServices::callback_t);
		void DetachInterrupt(uint16_t pin);
	};
}

#endif