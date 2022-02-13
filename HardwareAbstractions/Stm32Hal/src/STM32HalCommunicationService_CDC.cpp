#include "STM32HalCommunicationService_CDC.h"
#include "usbd_cdc_if.h"
#include <cstring>

#ifdef STM32HALCOMMUNICATIONSERVICE_CDC_H
extern USBD_HandleTypeDef hUsbDeviceFS;
extern uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];
extern uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];
namespace Stm32
{
	size_t UserRxBufferStart = 0;
	volatile size_t UserRxBufferEnd = 0;
	volatile size_t UserRXBufferEndSkipped = 0;
	size_t UserTXBufferLength = 0;

	STM32HalCommunicationService_CDC *STM32HalCommunicationService_CDC::Instance;
	int8_t STM32HalCommunicationService_CDC::CDCReceive(uint8_t* data, uint32_t *length)
	{
		if(UserRxBufferEnd + CDC_DATA_FS_OUT_PACKET_SIZE > APP_RX_DATA_SIZE) {
			UserRXBufferEndSkipped = APP_RX_DATA_SIZE - UserRxBufferEnd;
			UserRxBufferEnd = 0;
		}
		UserRxBufferEnd += *length;
		USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &UserRxBufferFS[UserRxBufferEnd]);
		USBD_CDC_ReceivePacket(&hUsbDeviceFS);
		return (USBD_OK);
	}

	void STM32HalCommunicationService_CDC::Flush()
	{
		size_t rxBufferEnd = UserRxBufferEnd;
		size_t rxBufferEndSkipped = UserRXBufferEndSkipped;
		while(rxBufferEnd != UserRxBufferEnd || rxBufferEndSkipped != UserRXBufferEndSkipped)
		{
			rxBufferEnd = UserRxBufferEnd;
			rxBufferEndSkipped = UserRXBufferEndSkipped;
		}

		//if buffer wrapped around, we need to call the 
		if(rxBufferEnd < UserRxBufferStart)
		{
			const size_t len = APP_RX_DATA_SIZE - rxBufferEndSkipped - UserRxBufferStart;
			if(len > 0)
				Receive(&UserRxBufferFS[UserRxBufferStart], len);
			UserRxBufferStart = 0;
		}

		const size_t len = rxBufferEnd - UserRxBufferStart;
		if(len > 0)
			UserRxBufferStart = rxBufferEnd - (len - Receive(&UserRxBufferFS[UserRxBufferStart], len));

		CDC_Transmit_FS(UserTxBufferFS, UserTXBufferLength);
		UserTXBufferLength = 0;
	}

	STM32HalCommunicationService_CDC::STM32HalCommunicationService_CDC()
	{
		Instance = this;
		USBD_Interface_fops_FS.Receive = CDCReceive;
	}

	STM32HalCommunicationService_CDC::~STM32HalCommunicationService_CDC()
	{
		USBD_Interface_fops_FS.Receive = 0;
	}

	void STM32HalCommunicationService_CDC::Send(void *data, size_t length)
	{
		std::memcpy(&UserTxBufferFS[UserTXBufferLength], data, length);
		UserTXBufferLength += length;
	}
}
#endif