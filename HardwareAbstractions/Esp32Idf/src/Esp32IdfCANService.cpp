#include "Esp32IdfCANService.h"
#include "esp_err.h"
#include "freertos/task.h"

#include <algorithm>
#include <new>
#include <utility>

using namespace EmbeddedIOServices;

namespace Esp32
{
	Esp32IdfCANService::Esp32IdfCANService(const Esp32IdfCANServiceChannelConfig configs[SOC_TWAI_CONTROLLER_NUM])
	{
		for(uint8_t i = 0; i < SOC_TWAI_CONTROLLER_NUM; i++)
		{
			if(!configs[i].enabled)
			{
				_twai_handles[i] = 0;
				continue;
			}
			ESP_ERROR_CHECK(twai_driver_install_v2(&configs[i].g_config, &configs[i].t_config, &configs[i].f_config, &_twai_handles[i]));
			ESP_ERROR_CHECK(twai_reconfigure_alerts_v2(
				_twai_handles[i],
				configs[i].g_config.alerts_enabled |
					TWAI_ALERT_TX_SUCCESS |
					TWAI_ALERT_TX_FAILED |
					TWAI_ALERT_BUS_OFF,
				nullptr));
			ESP_ERROR_CHECK(twai_start_v2(_twai_handles[i]));

			_receiveTaskArgs[i] = { this, i };
			_transmitTaskArgs[i] = { this, i };
			const UBaseType_t transmitQueueDepth = std::max<UBaseType_t>(
				1,
				configs[i].g_config.tx_queue_len);
			_transmitQueues[i] = xQueueCreate(
				transmitQueueDepth,
				sizeof(TransmitRequest*));
			ESP_ERROR_CHECK(_transmitQueues[i] == nullptr
				? ESP_ERR_NO_MEM
				: ESP_OK);

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

			xTaskCreate([] (void *arg)
			{
				TransmitTaskArg *transmitTaskArg =
					reinterpret_cast<TransmitTaskArg *>(arg);
				Esp32IdfCANService *service = transmitTaskArg->canService;
				const uint8_t channel = transmitTaskArg->i;
				while (1)
				{
					TransmitRequest *request = nullptr;
					if (xQueueReceive(
							service->_transmitQueues[channel],
							&request,
							portMAX_DELAY) != pdTRUE ||
						request == nullptr)
					{
						continue;
					}

					const esp_err_t transmitResult = twai_transmit_v2(
						service->_twai_handles[channel],
						&request->Message,
						portMAX_DELAY);
					if (transmitResult == ESP_OK)
					{
						uint32_t alerts = 0;
						do
						{
							if (twai_read_alerts_v2(
									service->_twai_handles[channel],
									&alerts,
									portMAX_DELAY) != ESP_OK)
							{
								alerts = TWAI_ALERT_TX_FAILED;
							}
						} while ((alerts & (TWAI_ALERT_TX_SUCCESS |
							TWAI_ALERT_TX_FAILED | TWAI_ALERT_BUS_OFF)) == 0);
					}

					auto completion = std::move(request->Completion);
					delete request;
					if (completion)
						completion();
				}
			}, "Esp32IdfCANTx", 4096, &_transmitTaskArgs[i], 10, NULL);
		}
	}
	
	void Esp32IdfCANService::Send(const CANIdentifier_t identifier, const CANData_t data, const uint8_t dataLength, can_send_completion_callback_t completion)
	{
		if(identifier.CANBusNumber >= SOC_TWAI_CONTROLLER_NUM ||
			_twai_handles[identifier.CANBusNumber] == 0 ||
			_transmitQueues[identifier.CANBusNumber] == nullptr)
			return;

		const twai_message_t message = 
		{
			.extd = identifier.CANIdentifier >= (1 << 11),
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
		TransmitRequest *request = new (std::nothrow) TransmitRequest{
			message,
			std::move(completion)};
		if (request == nullptr)
			return;
		if (xQueueSend(
				_transmitQueues[identifier.CANBusNumber],
				&request,
				0) != pdTRUE)
		{
			delete request;
		}
	}
}
