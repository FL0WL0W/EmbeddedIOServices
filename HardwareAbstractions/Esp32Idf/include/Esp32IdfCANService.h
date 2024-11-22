#include "PinDirection.h"
#include "ICANService.h"
#include "hal/gpio_hal.h"
#include "esp_intr_alloc.h"

#ifndef ESP32IDFCANSERVICE_H
#define ESP32IDFCANSERVICE_H

namespace Esp32
{
	class Esp32IdfCANService : public EmbeddedIOServices::ICANService
	{
	protected: 
	public:
		Esp32IdfCANService();
        void Send(const CANIdentifier_t identifier, const CANData_t data);
	};
}

#endif