#include "CommunicationService_W80xUART.h"
#include <cstring>

#define RCC   	((RCC_TypeDef *)RCC_BASE)
#define GPIOA   ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB   ((GPIO_TypeDef *)GPIOB_BASE)
#define DMA		((DMA_TypeDef *)DMA_BASE)

#ifdef COMMUNICATIONSERVICE_W80XUART_H
namespace EmbeddedIOServices
{
	CommunicationService_W80xUART *CommunicationService_W80xUART::CommunicationService_W80xUART0 = 0;
	CommunicationService_W80xUART *CommunicationService_W80xUART::CommunicationService_W80xUART1 = 0;
	CommunicationService_W80xUART *CommunicationService_W80xUART::CommunicationService_W80xUART2 = 0;
	CommunicationService_W80xUART *CommunicationService_W80xUART::CommunicationService_W80xUART3 = 0;
	CommunicationService_W80xUART *CommunicationService_W80xUART::CommunicationService_W80xUART4 = 0;
	CommunicationService_W80xUART *CommunicationService_W80xUART::CommunicationService_W80xUART5 = 0;

	CommunicationService_W80xUART::CommunicationService_W80xUART(size_t rxFifoSize, size_t txFifoSize) :
		_rxFifo(rxFifoSize),
		_txFifo(txFifoSize)
	{}
	CommunicationService_W80xUART *CommunicationService_W80xUART::Create(
		uint8_t uart,
		uint8_t pinTX,
		uint8_t pinRX,
		size_t rxFifoSize=1024,
		size_t txFifoSize=1024,
		uint32_t baud=115200, 
		uint32_t wordLength=UART_WORDLENGTH_8B, 
		uint32_t stopBits=UART_STOPBITS_1, 
		uint32_t parity=UART_PARITY_NONE
	)
	{
		CommunicationService_W80xUART *ret = 0;

		switch (uart)
		{
		case 0:
			if(CommunicationService_W80xUART0 != 0)
				ret = CommunicationService_W80xUART0;
			else
				CommunicationService_W80xUART0 = ret = new CommunicationService_W80xUART(rxFifoSize, txFifoSize);

    		RCC->CLK_EN |= RCC_CLK_EN_UART0 | RCC_CLK_EN_GPIO;
			__HAL_AFIO_REMAP_UART0_TX((pinTX > 32? GPIOB : GPIOA), 1 << (pinTX % 32));
			__HAL_AFIO_REMAP_UART0_RX((pinRX > 32? GPIOB : GPIOA), 1 << (pinRX % 32));
			csi_vic_set_prio(UART0_IRQn, 3);
			csi_vic_enable_irq(UART0_IRQn);
			break;
		case 1:
			if(CommunicationService_W80xUART1 != 0)
				ret = CommunicationService_W80xUART1;
			else
				CommunicationService_W80xUART1 = ret = new CommunicationService_W80xUART(rxFifoSize, txFifoSize);
    		RCC->CLK_EN |= RCC_CLK_EN_UART1 | RCC_CLK_EN_GPIO;
			__HAL_AFIO_REMAP_UART1_TX((pinTX > 32? GPIOB : GPIOA), 1 << (pinTX % 32));
			__HAL_AFIO_REMAP_UART1_RX((pinRX > 32? GPIOB : GPIOA), 1 << (pinRX % 32));
			csi_vic_set_prio(UART1_IRQn, 3);
			csi_vic_enable_irq(UART1_IRQn);
			break;
		case 2:
			if(CommunicationService_W80xUART2 != 0)
				ret = CommunicationService_W80xUART2;
			else
				CommunicationService_W80xUART2 = ret = new CommunicationService_W80xUART(rxFifoSize, txFifoSize);
    		RCC->CLK_EN |= RCC_CLK_EN_UART2 | RCC_CLK_EN_GPIO;
			__HAL_AFIO_REMAP_UART2_TX((pinTX > 32? GPIOB : GPIOA), 1 << (pinTX % 32));
			__HAL_AFIO_REMAP_UART2_RX((pinRX > 32? GPIOB : GPIOA), 1 << (pinRX % 32));
			csi_vic_set_prio(UART2_5_IRQn, 3);
			csi_vic_enable_irq(UART2_5_IRQn);
			break;
		case 3:
			if(CommunicationService_W80xUART3 != 0)
				ret = CommunicationService_W80xUART3;
			else
				CommunicationService_W80xUART3 = ret = new CommunicationService_W80xUART(rxFifoSize, txFifoSize);
    		RCC->CLK_EN |= RCC_CLK_EN_UART3 | RCC_CLK_EN_GPIO;
			__HAL_AFIO_REMAP_UART3_TX((pinTX > 32? GPIOB : GPIOA), 1 << (pinTX % 32));
			__HAL_AFIO_REMAP_UART3_RX((pinRX > 32? GPIOB : GPIOA), 1 << (pinRX % 32));
			csi_vic_set_prio(UART2_5_IRQn, 3);
			csi_vic_enable_irq(UART2_5_IRQn);
			break;
		case 4:
			if(CommunicationService_W80xUART4 != 0)
				ret = CommunicationService_W80xUART4;
			else
				CommunicationService_W80xUART4 = ret = new CommunicationService_W80xUART(rxFifoSize, txFifoSize);
    		RCC->CLK_EN |= RCC_CLK_EN_UART4 | RCC_CLK_EN_GPIO;
			__HAL_AFIO_REMAP_UART4_TX((pinTX > 32? GPIOB : GPIOA), 1 << (pinTX % 32));
			__HAL_AFIO_REMAP_UART4_RX((pinRX > 32? GPIOB : GPIOA), 1 << (pinRX % 32));
			csi_vic_set_prio(UART2_5_IRQn, 3);
			csi_vic_enable_irq(UART2_5_IRQn);
			break;
		case 5:
			if(CommunicationService_W80xUART5 != 0)
				ret = CommunicationService_W80xUART5;
			else
				CommunicationService_W80xUART5 = ret = new CommunicationService_W80xUART(rxFifoSize, txFifoSize);
    		RCC->CLK_EN |= RCC_CLK_EN_UART5 | RCC_CLK_EN_GPIO;
			__HAL_AFIO_REMAP_UART5_TX((pinTX > 32? GPIOB : GPIOA), 1 << (pinTX % 32));
			__HAL_AFIO_REMAP_UART5_RX((pinRX > 32? GPIOB : GPIOA), 1 << (pinRX % 32));
			csi_vic_set_prio(UART2_5_IRQn, 3);
			csi_vic_enable_irq(UART2_5_IRQn);
			break;
		default:
			return ret;
		}

		ret->_uart = ((USART_TypeDef *)(UART0_BASE + 0x200 * uart));
		uint32_t apbclk = (480 / (0xFF & RCC->CLK_DIV)) / (0xFF & (RCC->CLK_DIV >> 16));
		apbclk = apbclk * 1000000;
		uint32_t value = (apbclk / (16 * baud) - 1) |
				(((apbclk % (baud * 16)) * 16 / (baud * 16)) << 16);
		ret->_uart->BAUDR = value;
		ret->_uart->DMAC = (4 << 3) | UART_DMAC_RTOE;
		ret->_uart->FC = UART_HWCONTROL_NONE;
		ret->_uart->INTM &= ~(UART_INTM_RTO | UART_INTM_RL);
		ret->_uart->FIFOC = UART_FIFOC_RFL_4 | UART_FIFOC_TFL_4;
		ret->_uart->LC = UART_MODE_TX_RX | wordLength | stopBits | parity | UART_LC_RE | UART_LC_TE;
		ret->_uart->INTS = 0xFFFFFFFF;

		return ret;
	}
	void CommunicationService_W80xUART::Interrupt()
	{
		//clear all flags that we don't care about
		_uart->INTS = ~(UART_INTS_RL | UART_INTS_RTO | UART_INTS_TL | UART_INTS_TEMPT);
		const bool receiveFlagsSet = (_uart->INTS & (UART_INTS_RL | UART_INTS_RTO)) != 0;
		const bool transmitFlagsSet = (_uart->INTS & (UART_INTS_TL | UART_INTS_TEMPT)) != 0;
		const bool transmitInterruptEnabled = !((_uart->INTM & UART_INTM_TL) || (_uart->INTM & UART_INTM_TEMPT));
		//receive interrupt
		if(receiveFlagsSet)
		{
			//clear receive flags
			_uart->INTS = (UART_INTS_RL | UART_INTS_RTO);
			//receive all bytes from hardware fifo and put them in software fifo
			while((_uart->FIFOS & UART_FIFOS_RFC) > 0)
			{
				const uint8_t b = _uart->RDW;
				_rxFifo.Write(&b, 1);
			}
		}
		//transmit interrupt
		if(transmitFlagsSet && transmitInterruptEnabled)
		{
			//clear transmit flags
			_uart->INTS = (UART_INTS_TL | UART_INTS_TEMPT);
			FlushTransmit();
		}
	}
	void CommunicationService_W80xUART::Send(const void *data, size_t length)
	{
		_txFifo.Write(data, length);
		FlushTransmit();
	}
	void CommunicationService_W80xUART::FlushReceive()
	{
		const size_t len = _rxFifo.Length();
		uint8_t buf[len];
		_rxFifo.Peak(buf, len);
		const size_t handled = Receive(buf, len);
		if(handled > 0)
			_rxFifo.Pop(handled);
	}
	void CommunicationService_W80xUART::FlushTransmit()
	{
		//disable interrupt
		_uart->INTM |= (UART_INTM_TL | UART_INTM_TEMPT);
		//put all bytes from software fifo into hardware fifo
		while(_txFifo.Length() > 0)
		{
			//if the transmit fifo is full, let the interrupt pick this back up when it is empty or below the limit
			if((_uart->FIFOS & UART_FIFOS_TFC) >= UART_FIFO_FULL)
			{
				_uart->INTM &= ~(UART_INTM_TL | UART_INTM_TEMPT);
				return;
			}
			uint8_t b;
			_txFifo.Read(&b, 1);
			_uart->TDW = b;
		}
	}
	void CommunicationService_W80xUART::UART0_IRQHandler()
	{
		if(CommunicationService_W80xUART0 != 0)
			CommunicationService_W80xUART0->Interrupt();
	}
	void CommunicationService_W80xUART::UART1_IRQHandler()
	{
		if(CommunicationService_W80xUART1 != 0)
			CommunicationService_W80xUART1->Interrupt();
	}
	void CommunicationService_W80xUART::UART2_5_IRQHandler()
	{
		if(CommunicationService_W80xUART2 != 0 && CommunicationService_W80xUART2->_uart->INTS != 0)
			CommunicationService_W80xUART2->Interrupt();
		if(CommunicationService_W80xUART3 != 0 && CommunicationService_W80xUART3->_uart->INTS != 0)
			CommunicationService_W80xUART3->Interrupt();
		if(CommunicationService_W80xUART4 != 0 && CommunicationService_W80xUART4->_uart->INTS != 0)
			CommunicationService_W80xUART4->Interrupt();
		if(CommunicationService_W80xUART5 != 0 && CommunicationService_W80xUART5->_uart->INTS != 0)
			CommunicationService_W80xUART5->Interrupt();
	}
}

using namespace EmbeddedIOServices;

extern "C" __attribute__((section(".interrupt")))  __attribute__((isr)) void UART0_IRQHandler(void)
{
    CommunicationService_W80xUART::UART0_IRQHandler();
}
extern "C" __attribute__((section(".interrupt")))  __attribute__((isr)) void UART1_IRQHandler(void)
{
    CommunicationService_W80xUART::UART1_IRQHandler();
}
extern "C" __attribute__((section(".interrupt")))  __attribute__((isr)) void UART2_5_IRQHandler(void)
{
    CommunicationService_W80xUART::UART2_5_IRQHandler();
}
#endif
