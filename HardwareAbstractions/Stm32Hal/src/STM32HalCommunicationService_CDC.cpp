#include "STM32HalCommunicationService_CDC.h"
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
	volatile size_t UserTxBufferStart = 0;
	volatile size_t UserTxBufferEnd = 0;

	STM32HalCommunicationService_CDC *STM32HalCommunicationService_CDC::Instance;
	int8_t STM32HalCommunicationService_CDC::CDCReceive(uint8_t* data, uint32_t *length)
	{
		UserRxBufferEnd += *length;
		const size_t userRXBufferEndSkipped = APP_RX_DATA_SIZE - UserRxBufferEnd;
		if(userRXBufferEndSkipped < CDC_DATA_FS_OUT_PACKET_SIZE) {
			UserRXBufferEndSkipped = userRXBufferEndSkipped;
			UserRxBufferEnd = CDC_DATA_FS_OUT_PACKET_SIZE;
		}
		USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &UserRxBufferFS[UserRxBufferEnd]);
		USBD_CDC_ReceivePacket(&hUsbDeviceFS);
		return (USBD_OK);
	}
	
	int8_t STM32HalCommunicationService_CDC::CDCTransmitCplt(uint8_t *Buf, uint32_t *Len, uint8_t epnum)
	{
		uint16_t sendAmount = UserTxBufferEnd - UserTxBufferStart;
		if(sendAmount > CDC_DATA_FS_IN_PACKET_SIZE)
			sendAmount = CDC_DATA_FS_IN_PACKET_SIZE;
		if(sendAmount + UserTxBufferStart > APP_TX_DATA_SIZE)
			sendAmount = APP_TX_DATA_SIZE - UserTxBufferStart;
		if(sendAmount > 0)
		{
			size_t oldTxBufferStart = UserTxBufferStart;
			UserTxBufferStart += sendAmount;
			if(UserTxBufferStart >= APP_TX_DATA_SIZE)
				UserTxBufferStart = 0;
			if(USBD_OK != CDC_Transmit_FS(&UserTxBufferFS[oldTxBufferStart], sendAmount))
				UserTxBufferStart = oldTxBufferStart;
		}
		return USBD_OK;
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
			size_t len = APP_RX_DATA_SIZE - rxBufferEndSkipped - UserRxBufferStart;
			if(len > 0)
				len = len - Receive(&UserRxBufferFS[UserRxBufferStart], len);
			UserRxBufferStart = CDC_DATA_FS_OUT_PACKET_SIZE - len;
			std::memcpy(&UserRxBufferFS[UserRxBufferStart], &UserRxBufferFS[APP_RX_DATA_SIZE - rxBufferEndSkipped - len], len);
		}

		const size_t len = rxBufferEnd - UserRxBufferStart;
		if(len > 0)
			UserRxBufferStart = rxBufferEnd - (len - Receive(&UserRxBufferFS[UserRxBufferStart], len));

		CDCTransmitCplt(0,0,0);
	}

	STM32HalCommunicationService_CDC::STM32HalCommunicationService_CDC()
	{
		Instance = this;
		USBD_Interface_fops_FS.Receive = CDCReceive;
		USBD_Interface_fops_FS.TransmitCplt = CDCTransmitCplt;
	}

	STM32HalCommunicationService_CDC::~STM32HalCommunicationService_CDC()
	{
		USBD_Interface_fops_FS.Receive = 0;
	}

	void STM32HalCommunicationService_CDC::Send(const void *data, size_t length)
	{
		uint16_t sendAmount = length;
		if(UserTxBufferEnd + sendAmount > APP_TX_DATA_SIZE)
			sendAmount = APP_TX_DATA_SIZE - UserTxBufferEnd;

		std::memcpy(&UserTxBufferFS[UserTxBufferEnd], data, sendAmount);
		UserTxBufferEnd = (UserTxBufferEnd + sendAmount) % APP_TX_DATA_SIZE;
		length -= sendAmount;
		if(length > 0)
		{
			std::memcpy(&UserTxBufferFS[0], &reinterpret_cast<const uint8_t *>(data)[sendAmount], length);
			UserTxBufferEnd = length;
		}
	}
}
#endif