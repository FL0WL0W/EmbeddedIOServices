#include "ICommunicationService.h"

#if __has_include("usbd_cdc_if.h") && !defined(STM32HALCOMMUNICATIONSERVICE_CDC_H)
#define STM32HALCOMMUNICATIONSERVICE_CDC_H
#include "usbd_cdc_if.h"
namespace Stm32
{
	class STM32HalCommunicationService_CDC : public EmbeddedIOServices::ICommunicationService
	{
	private: 
		static STM32HalCommunicationService_CDC *Instance;
		static int8_t CDCReceive(uint8_t* data, uint32_t *length);
		static int8_t CDCTransmitCplt(uint8_t *Buf, uint32_t *Len, uint8_t epnum);
	public:
		STM32HalCommunicationService_CDC();
		~STM32HalCommunicationService_CDC();
		void Send(const void *data, size_t length) override;
		void Flush();
	};
}
#endif