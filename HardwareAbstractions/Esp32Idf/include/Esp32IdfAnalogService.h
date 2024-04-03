#include "IAnalogService.h"
#include "stdint.h"

#ifndef ESP32IDFANALOGSERVICE_H
#define ESP32IDFANALOGSERVICE_H

namespace Esp32
{
	class Esp32IdfAnalogService : public EmbeddedIOServices::IAnalogService
	{
	public:
		Esp32IdfAnalogService();
		void InitPin(uint16_t pin);
		float ReadPin(uint16_t pin);
	};
}

#endif