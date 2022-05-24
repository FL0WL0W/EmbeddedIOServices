#include "ICommunicationService.h"
#include "Fifo.h"
#include "wm_uart.h"

#ifndef COMMUNICATIONSERVICE_W806UART_H
#define COMMUNICATIONSERVICE_W806UART_H
namespace EmbeddedIOServices
{
	class CommunicationService_W806UART : public ICommunicationService
	{
	protected:
		CommunicationService_W806UART(size_t fifoSize);
		UART_HandleTypeDef _huart;
		uint8_t _buf[UART_FIFO_FULL] = {0};
		Fifo _fifo;
	public:
        void Send(const void *data, size_t length);
		void RxCpltCallback();
		void Flush();

		static void UART0_IRQHandler();
		static void UART1_IRQHandler();
		static void UART2_5_IRQHandler();

		static CommunicationService_W806UART *Create(
			USART_TypeDef *uart, 
			size_t fifoSize,
			uint32_t baud, 
			uint32_t wordLength, 
			uint32_t stopBits, 
			uint32_t parity
		);
		static CommunicationService_W806UART *CommunicationService_W806UART0;
		static CommunicationService_W806UART *CommunicationService_W806UART1;
		static CommunicationService_W806UART *CommunicationService_W806UART2;
		static CommunicationService_W806UART *CommunicationService_W806UART3;
		static CommunicationService_W806UART *CommunicationService_W806UART4;
		static CommunicationService_W806UART *CommunicationService_W806UART5;
	};
}
#endif
