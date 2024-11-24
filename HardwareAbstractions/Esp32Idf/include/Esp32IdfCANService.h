#include "PinDirection.h"
#include "ICANService.h"
#include "driver/twai.h"
#include "soc/soc_caps.h"

#ifndef ESP32IDFCANSERVICE_H
#define ESP32IDFCANSERVICE_H

namespace Esp32
{
	struct Esp32IdfCANServiceChannelConfig
	{
		twai_timing_config_t t_config;
		twai_filter_config_t f_config;
		twai_general_config_t g_config;
	};

	class Esp32IdfCANService : public EmbeddedIOServices::ICANService
	{
	protected:
		struct ReceiveTaskArg 
		{
			Esp32IdfCANService *canService;
			uint8_t i;
		};
		ReceiveTaskArg _receiveTaskArgs[SOC_TWAI_CONTROLLER_NUM];
		twai_handle_t _twai_handles[SOC_TWAI_CONTROLLER_NUM];
	public:
		Esp32IdfCANService(const Esp32IdfCANServiceChannelConfig configs[SOC_TWAI_CONTROLLER_NUM]);
        void Send(const EmbeddedIOServices::CANIdentifier_t identifier, const EmbeddedIOServices::CANData_t data);
	};
}

#endif