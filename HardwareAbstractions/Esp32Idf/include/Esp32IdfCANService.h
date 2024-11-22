#include "PinDirection.h"
#include "ICANService.h"

#ifndef ESP32IDFCANSERVICE_H
#define ESP32IDFCANSERVICE_H

namespace Esp32
{
	class Esp32IdfCANService : public EmbeddedIOServices::ICANService
	{
	public:
		Esp32IdfCANService();
        void Send(const EmbeddedIOServices::CANIdentifier_t identifier, const EmbeddedIOServices::CANData_t data);
	};
}

#endif