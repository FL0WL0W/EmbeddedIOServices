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
			GPIOB->AF_SEL |= ((1<<19) | (1<<20));
			GPIOB->AF_S0 &= ~((1<<19) | (1<<20));
			GPIOB->AF_S1 &= ~((1<<19) | (1<<20));
			csi_vic_enable_irq(UART0_IRQn);
			csi_vic_set_prio(UART0_IRQn, 3);
			break;
		case 1:
			if(CommunicationService_W80xUART1 != 0)
				ret = CommunicationService_W80xUART1;
			else
				CommunicationService_W80xUART1 = ret = new CommunicationService_W80xUART(rxFifoSize, txFifoSize);
    		RCC->CLK_EN |= RCC_CLK_EN_UART1 | RCC_CLK_EN_GPIO;
			GPIOB->AF_SEL |= ((1<<6) | (1<<7));
			GPIOB->AF_S0 &= ~((1<<6) | (1<<7));
			GPIOB->AF_S1 &= ~((1<<6) | (1<<7));
			csi_vic_set_prio(UART1_IRQn, 3);
			csi_vic_enable_irq(UART1_IRQn);
			break;
		case 2:
			if(CommunicationService_W80xUART2 != 0)
				ret = CommunicationService_W80xUART2;
			else
				CommunicationService_W80xUART2 = ret = new CommunicationService_W80xUART(rxFifoSize, txFifoSize);
    		RCC->CLK_EN |= RCC_CLK_EN_UART2 | RCC_CLK_EN_GPIO;
			GPIOB->AF_SEL |= ((1<<2) | (1<<3));
			GPIOB->AF_S0 &= ~((1<<2) | (1<<3));
			GPIOB->AF_S1 |= ((1<<2) | (1<<3));
			csi_vic_set_prio(UART2_5_IRQn, 3);
			csi_vic_enable_irq(UART2_5_IRQn);
			break;
		case 3:
			if(CommunicationService_W80xUART3 != 0)
				ret = CommunicationService_W80xUART3;
			else
				CommunicationService_W80xUART3 = ret = new CommunicationService_W80xUART(rxFifoSize, txFifoSize);
    		RCC->CLK_EN |= RCC_CLK_EN_UART3 | RCC_CLK_EN_GPIO;
			GPIOB->AF_SEL |= ((1<<0) | (1<<1));
			GPIOB->AF_S0 &= ~((1<<0) | (1<<1));
			GPIOB->AF_S1 |= ((1<<0) | (1<<1));
			csi_vic_set_prio(UART2_5_IRQn, 3);
			csi_vic_enable_irq(UART2_5_IRQn);
			break;
		case 4:
			if(CommunicationService_W80xUART4 != 0)
				ret = CommunicationService_W80xUART4;
			else
				CommunicationService_W80xUART4 = ret = new CommunicationService_W80xUART(rxFifoSize, txFifoSize);
    		RCC->CLK_EN |= RCC_CLK_EN_UART4 | RCC_CLK_EN_GPIO;
			GPIOB->AF_SEL |= ((1<<4) | (1<<5));
			GPIOB->AF_S0 &= ~((1<<4) | (1<<5));
			GPIOB->AF_S1 |= ((1<<4) | (1<<5));
			csi_vic_set_prio(UART2_5_IRQn, 3);
			csi_vic_enable_irq(UART2_5_IRQn);
			break;
		case 5:
			if(CommunicationService_W80xUART5 != 0)
				ret = CommunicationService_W80xUART5;
			else
				CommunicationService_W80xUART5 = ret = new CommunicationService_W80xUART(rxFifoSize, txFifoSize);
    		RCC->CLK_EN |= RCC_CLK_EN_UART5 | RCC_CLK_EN_GPIO;
			GPIOA->AF_SEL |= ((1<<12) | (1<<13));
			GPIOA->AF_S0 |= ((1<<12) | (1<<13));
			GPIOA->AF_S1 &= ~((1<<12) | (1<<13));
			csi_vic_set_prio(UART2_5_IRQn, 3);
			csi_vic_enable_irq(UART2_5_IRQn);
			break;
		default:
			return ret;
		}

		ret->_uart = ((USART_TypeDef *)UART0_BASE + 0x200 * uart);
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
	void CommunicationService_W80xUART::ReceiveInt()
	{
		while((CommunicationService_W80xUART0->_uart->FIFOS & UART_FIFOS_RFC) > 0)
		{
			const uint8_t b = _uart->RDW;
			_rxFifo.Write(&b, 1);
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
		_uart->INTM |= (UART_INTM_TL | UART_INTM_TEMPT);
		while(_txFifo.Length() > 0)
		{
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
		const uint32_t flags = CommunicationService_W80xUART0->_uart->INTS;
		CommunicationService_W80xUART0->_uart->INTS = 0xFFFFFFFF;
		if((flags & (UART_INTS_RL | UART_INTS_RTO)) != 0)
			CommunicationService_W80xUART0->ReceiveInt();
		if((flags & (UART_INTS_TL | UART_INTS_TEMPT)) != 0)
			CommunicationService_W80xUART0->FlushTransmit();
	}
	void CommunicationService_W80xUART::UART1_IRQHandler()
	{
		const uint32_t flags = CommunicationService_W80xUART1->_uart->INTS;
		CommunicationService_W80xUART1->_uart->INTS = 0xFFFFFFFF;
		if((flags & (UART_INTS_RL | UART_INTS_RTO)) != 0)
			CommunicationService_W80xUART1->ReceiveInt();
		if((flags & (UART_INTS_TL | UART_INTS_TEMPT)) != 0)
			CommunicationService_W80xUART1->FlushTransmit();
	}
	void CommunicationService_W80xUART::UART2_5_IRQHandler()
	{
		const uint32_t flags2 = CommunicationService_W80xUART2->_uart->INTS;
		CommunicationService_W80xUART2->_uart->INTS = 0xFFFFFFFF;
		const uint32_t flags3 = CommunicationService_W80xUART3->_uart->INTS;
		CommunicationService_W80xUART3->_uart->INTS = 0xFFFFFFFF;
		const uint32_t flags4 = CommunicationService_W80xUART4->_uart->INTS;
		CommunicationService_W80xUART4->_uart->INTS = 0xFFFFFFFF;
		const uint32_t flags5 = CommunicationService_W80xUART5->_uart->INTS;
		CommunicationService_W80xUART5->_uart->INTS = 0xFFFFFFFF;

		if((flags2 & (UART_INTS_RL | UART_INTS_RTO)) != 0)
			CommunicationService_W80xUART2->ReceiveInt();
		if((flags3 & (UART_INTS_RL | UART_INTS_RTO)) != 0)
			CommunicationService_W80xUART3->ReceiveInt();
		if((flags4 & (UART_INTS_RL | UART_INTS_RTO)) != 0)
			CommunicationService_W80xUART4->ReceiveInt();
		if((flags5 & (UART_INTS_RL | UART_INTS_RTO)) != 0)
			CommunicationService_W80xUART5->ReceiveInt();

		if((flags2 & (UART_INTS_TL | UART_INTS_TEMPT)) != 0)
			CommunicationService_W80xUART2->FlushTransmit();
		if((flags3 & (UART_INTS_TL | UART_INTS_TEMPT)) != 0)
			CommunicationService_W80xUART3->FlushTransmit();
		if((flags4 & (UART_INTS_TL | UART_INTS_TEMPT)) != 0)
			CommunicationService_W80xUART4->FlushTransmit();
		if((flags5 & (UART_INTS_TL | UART_INTS_TEMPT)) != 0)
			CommunicationService_W80xUART5->FlushTransmit();
	}
}

using namespace EmbeddedIOServices;

extern "C" __attribute__((isr)) void UART0_IRQHandler(void)
{
    CommunicationService_W80xUART::UART0_IRQHandler();
}
extern "C" __attribute__((isr)) void UART1_IRQHandler(void)
{
    CommunicationService_W80xUART::UART1_IRQHandler();
}
extern "C" __attribute__((isr)) void UART2_5_IRQHandler(void)
{
    CommunicationService_W80xUART::UART2_5_IRQHandler();
}
#endif
