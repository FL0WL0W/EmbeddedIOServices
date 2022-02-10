#include "STM32HalCommunicationService_CDC.h"

#ifdef STM32HALCOMMUNICATIONSERVICE_CDC_H
extern USBD_HandleTypeDef hUsbDeviceFS;
namespace Stm32
{
	STM32HalCommunicationService_CDC *STM32HalCommunicationService_CDC_Instance;
	int8_t CDCReceive(uint8_t* data, uint32_t *length)
	{
		USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &data[0]);
		USBD_CDC_ReceivePacket(&hUsbDeviceFS);
		STM32HalCommunicationService_CDC_Instance->Receive(data, *length);
		return (USBD_OK);
	}

	STM32HalCommunicationService_CDC::STM32HalCommunicationService_CDC()
	{
		STM32HalCommunicationService_CDC_Instance = this;
		USBD_Interface_fops_FS.Receive = CDCReceive;
	}

	STM32HalCommunicationService_CDC::~STM32HalCommunicationService_CDC()
	{
		USBD_Interface_fops_FS.Receive = 0;
	}

	void STM32HalCommunicationService_CDC::Send(void *data, size_t length)
	{
		CDC_Transmit_FS(reinterpret_cast<uint8_t *>(data), length);
	}
}
#endif