#include "Esp32IdfCANService.h"
#include "esp_err.h"
#include "freertos/task.h"

using namespace EmbeddedIOServices;

namespace Esp32
{
	Esp32IdfCANService::Esp32IdfCANService(const Esp32IdfCANServiceChannelConfig configs[SOC_TWAI_CONTROLLER_NUM])
	{
		for(uint8_t i = 0; i < SOC_TWAI_CONTROLLER_NUM; i++)
		{
			ESP_ERROR_CHECK(twai_driver_install_v2(&configs[i].g_config, &configs[i].t_config, &configs[i].f_config, &_twai_handles[i]));
			ESP_ERROR_CHECK(twai_start_v2(_twai_handles[i]));

			_receiveTaskArgs[i] = { this, i };

			// TODO probably need to stop these tasks when service is deleted
			xTaskCreate([] (void *arg) 
			{
				ReceiveTaskArg *receiveTaskArg = reinterpret_cast<ReceiveTaskArg *>( arg );
				while (1) {
					twai_message_t rx_msg;
					if(twai_receive_v2(receiveTaskArg->canService->_twai_handles[receiveTaskArg->i], &rx_msg, portMAX_DELAY) == ESP_OK)
					{
						receiveTaskArg->canService->Receive({rx_msg.identifier, receiveTaskArg->i}, *reinterpret_cast<CANData_t *>(&rx_msg.data), rx_msg.data_length_code);
					}
				}
			}, "Esp32IdfCANService", 4096, &_receiveTaskArgs[i], 10, NULL);
		}
	}
	
	void Esp32IdfCANService::Send(const CANIdentifier_t identifier, const CANData_t data, const uint8_t dataLength)
	{
		const twai_message_t message = 
		{
			.extd = identifier.CANIdentifier > (1 < 11),
			.identifier = identifier.CANIdentifier,
			.data_length_code = dataLength,
			.data = 
			{ 
				data.Data[0],
				data.Data[1],
				data.Data[2],
				data.Data[3],
				data.Data[4],
				data.Data[5],
				data.Data[6],
				data.Data[7]
			}
		};
		twai_transmit_v2(_twai_handles[identifier.CANBusNumber], &message, 0);
	}
}