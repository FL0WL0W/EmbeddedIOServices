#include "ICommunicationService.h"
#include "Fifo.h"
#include "wm_uart.h"

#ifndef COMMUNICATIONSERVICE_W80XUART_H
#define COMMUNICATIONSERVICE_W80XUART_H
namespace EmbeddedIOServices
{
	class CommunicationService_W80xUART : public ICommunicationService
	{
	private:
		static CommunicationService_W80xUART *CommunicationService_W80xUART0;
		static CommunicationService_W80xUART *CommunicationService_W80xUART1;
		static CommunicationService_W80xUART *CommunicationService_W80xUART2;
		static CommunicationService_W80xUART *CommunicationService_W80xUART3;
		static CommunicationService_W80xUART *CommunicationService_W80xUART4;
		static CommunicationService_W80xUART *CommunicationService_W80xUART5;
	protected:
		Fifo _rxFifo;
		Fifo _txFifo;
		USART_TypeDef *_uart;
		CommunicationService_W80xUART(size_t rxFifoSize, size_t txFifoSize);
		void Interrupt();
	public:
        void Send(const void *data, size_t length);
		void FlushTransmit();
		void FlushReceive();

		static void UART0_IRQHandler();
		static void UART1_IRQHandler();
		static void UART2_5_IRQHandler();

		static CommunicationService_W80xUART *Create(
			uint8_t uart, 
			size_t rxFifoSize,
			size_t txFifoSize,
			uint32_t baud, 
			uint32_t wordLength, 
			uint32_t stopBits, 
			uint32_t parity
		);
	};
}
#endif
