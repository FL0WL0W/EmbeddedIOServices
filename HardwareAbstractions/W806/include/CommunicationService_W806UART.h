#include "ICommunicationService.h"
#include "Fifo.h"
#include "wm_regs.h"

#define UART_WORDLENGTH_5B 		UART_LC_DATAL_5BIT
#define UART_WORDLENGTH_6B 		UART_LC_DATAL_6BIT
#define UART_WORDLENGTH_7B 		UART_LC_DATAL_7BIT
#define UART_WORDLENGTH_8B 		UART_LC_DATAL_8BIT
#define UART_STOPBITS_1         0x0
#define UART_STOPBITS_2         UART_LC_STOP
#define UART_PARITY_NONE        0x0
#define UART_PARITY_EVEN        UART_LC_PCE
#define UART_PARITY_ODD         (UART_LC_PCE | UART_LC_PS)
#define UART_HWCONTROL_NONE     0x00000000U
#define UART_HWCONTROL_RTS      ((uint32_t)UART_FC_AFCE)
#define UART_HWCONTROL_CTS      ((uint32_t)UART_FC_AFCE)
#define UART_HWCONTROL_RTS_CTS  ((uint32_t)UART_FC_AFCE)
#define UART_MODE_RX            ((uint32_t)UART_LC_RE)
#define UART_MODE_TX            ((uint32_t)UART_LC_TE)
#define UART_MODE_TX_RX         ((uint32_t)(UART_LC_RE | UART_LC_TE))
#define UART_FIFO_FULL 			32

#ifndef COMMUNICATIONSERVICE_W806UART_H
#define COMMUNICATIONSERVICE_W806UART_H
namespace EmbeddedIOServices
{
	class CommunicationService_W806UART : public ICommunicationService
	{
	protected:
		Fifo _rxFifo;
		Fifo _txFifo;
		USART_TypeDef *_uart;
		CommunicationService_W806UART(size_t rxFifoSize, size_t txFifoSize);
	public:
		__attribute__((section(".interrupt"))) void ReceiveInt();
        void Send(const void *data, size_t length);
		__attribute__((section(".interrupt"))) void FlushTransmit();
		void FlushReceive();

		__attribute__((section(".interrupt"))) static void UART0_IRQHandler();
		__attribute__((section(".interrupt"))) static void UART1_IRQHandler();
		__attribute__((section(".interrupt"))) static void UART2_5_IRQHandler();

		static CommunicationService_W806UART *Create(
			uint8_t uart, 
			size_t rxFifoSize,
			size_t txFifoSize,
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
