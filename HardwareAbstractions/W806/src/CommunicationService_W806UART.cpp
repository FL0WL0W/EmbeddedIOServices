#include "CommunicationService_W806UART.h"
#include <cstring>

#ifdef COMMUNICATIONSERVICE_W806UART_H
namespace EmbeddedIOServices
{
	CommunicationService_W806UART *CommunicationService_W806UART::CommunicationService_W806UART0 = 0;
	CommunicationService_W806UART *CommunicationService_W806UART::CommunicationService_W806UART1 = 0;
	CommunicationService_W806UART *CommunicationService_W806UART::CommunicationService_W806UART2 = 0;
	CommunicationService_W806UART *CommunicationService_W806UART::CommunicationService_W806UART3 = 0;
	CommunicationService_W806UART *CommunicationService_W806UART::CommunicationService_W806UART4 = 0;
	CommunicationService_W806UART *CommunicationService_W806UART::CommunicationService_W806UART5 = 0;

	CommunicationService_W806UART *CommunicationService_W806UART::Create(
		uint8_t uart, 
		size_t fifoSize=1024,
		uint32_t baud=115200, 
		uint32_t wordLength=UART_WORDLENGTH_8B, 
		uint32_t stopBits=UART_STOPBITS_1, 
		uint32_t parity=UART_PARITY_NONE
	)
	{
		CommunicationService_W806UART *ret;
		switch (uart)
		{
		case 0:
			if(CommunicationService_W806UART0 != 0)
				return ret;
			CommunicationService_W806UART0 = ret = new CommunicationService_W806UART(fifoSize);
    		RCC->CLK_EN |= RCC_CLK_EN_UART0;
			ret->_huart.Instance = UART0;
			GPIOB->AF_SEL |= ((1<<19) | (1<<20));
			GPIOB->AF_S0 &= ~((1<<19) | (1<<20));
			GPIOB->AF_S1 &= ~((1<<19) | (1<<20));
			csi_vic_set_prio(UART0_IRQn, 7);
			csi_vic_enable_irq(UART0_IRQn);
			break;
		case 1:
			if(CommunicationService_W806UART1 != 0)
				return ret;
			CommunicationService_W806UART1 = ret = new CommunicationService_W806UART(fifoSize);
    		RCC->CLK_EN |= RCC_CLK_EN_UART1;
			ret->_huart.Instance = UART1;
			GPIOB->AF_SEL |= ((1<<6) | (1<<7));
			GPIOB->AF_S0 &= ~((1<<6) | (1<<7));
			GPIOB->AF_S1 &= ~((1<<6) | (1<<7));
			csi_vic_set_prio(UART1_IRQn, 7);
			csi_vic_enable_irq(UART1_IRQn);
			break;
		case 2:
			if(CommunicationService_W806UART2 != 0)
				return ret;
			CommunicationService_W806UART2 = ret = new CommunicationService_W806UART(fifoSize);
    		RCC->CLK_EN |= RCC_CLK_EN_UART2;
			ret->_huart.Instance = UART2;
			csi_vic_set_prio(UART2_5_IRQn, 7);
			csi_vic_enable_irq(UART2_5_IRQn);
			GPIOB->AF_SEL |= ((1<<2) | (1<<3));
			GPIOB->AF_S0 &= ~((1<<2) | (1<<3));
			GPIOB->AF_S1 |= ((1<<2) | (1<<3));
			break;
		case 3:
			if(CommunicationService_W806UART3 != 0)
				return ret;
			CommunicationService_W806UART3 = ret = new CommunicationService_W806UART(fifoSize);
    		RCC->CLK_EN |= RCC_CLK_EN_UART3;
			ret->_huart.Instance = UART3;
			csi_vic_set_prio(UART2_5_IRQn, 7);
			csi_vic_enable_irq(UART2_5_IRQn);
			GPIOB->AF_SEL |= ((1<<0) | (1<<1));
			GPIOB->AF_S0 &= ~((1<<0) | (1<<1));
			GPIOB->AF_S1 |= ((1<<0) | (1<<1));
			break;
		case 4:
			if(CommunicationService_W806UART4 != 0)
				return ret;
			CommunicationService_W806UART4 = ret = new CommunicationService_W806UART(fifoSize);
    		RCC->CLK_EN |= RCC_CLK_EN_UART4;
			ret->_huart.Instance = UART4;
			csi_vic_set_prio(UART2_5_IRQn, 7);
			csi_vic_enable_irq(UART2_5_IRQn);
			GPIOB->AF_SEL |= ((1<<4) | (1<<5));
			GPIOB->AF_S0 &= ~((1<<4) | (1<<5));
			GPIOB->AF_S1 |= ((1<<4) | (1<<5));
			break;
		case 5:
			if(CommunicationService_W806UART5 != 0)
				return ret;
			CommunicationService_W806UART5 = ret = new CommunicationService_W806UART(fifoSize);
    		RCC->CLK_EN |= RCC_CLK_EN_UART5;
			ret->_huart.Instance = UART5;
			csi_vic_set_prio(UART2_5_IRQn, 7);
			csi_vic_enable_irq(UART2_5_IRQn);
			GPIOA->AF_SEL |= ((1<<12) | (1<<13));
			GPIOA->AF_S0 |= ((1<<12) | (1<<13));
			GPIOA->AF_S1 &= ~((1<<12) | (1<<13));
			break;
		
		default:
			return ret;
		}
		RCC->CLK_EN |= RCC_CLK_EN_GPIO;

		ret->_huart.Init.BaudRate = baud;
		ret->_huart.Init.WordLength = wordLength;
		ret->_huart.Init.StopBits = stopBits;
		ret->_huart.Init.Parity = parity;
		ret->_huart.Init.Mode = UART_MODE_TX | UART_MODE_RX;
		ret->_huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;

		if (HAL_UART_Init(&ret->_huart) != HAL_OK)
		{
			delete ret;
			switch (uart)
			{
			case 0:
				return CommunicationService_W806UART0 = ret = 0;
			case 1:
				return CommunicationService_W806UART1 = ret = 0;
			case 2:
				return CommunicationService_W806UART2 = ret = 0;
			case 3:
				return CommunicationService_W806UART3 = ret = 0;
			case 4:
				return CommunicationService_W806UART4 = ret = 0;
			case 5:
				return CommunicationService_W806UART5 = ret = 0;
			default:
				return ret;
			}
		}

		HAL_UART_Init(&ret->_huart);
		HAL_UART_Receive_IT(&ret->_huart, ret->_buf, 0);

		return ret;
	}
	CommunicationService_W806UART::CommunicationService_W806UART(size_t fifoSize) : _fifo(fifoSize)
	{
	}
	void CommunicationService_W806UART::Send(const void *data, size_t length)
	{
		HAL_UART_Transmit(&_huart, (uint8_t *)data, length, 1000);
	}
	void CommunicationService_W806UART::UART0_IRQHandler()
	{
    	HAL_UART_IRQHandler(&CommunicationService_W806UART0->_huart);
	}
	void CommunicationService_W806UART::UART1_IRQHandler()
	{
    	HAL_UART_IRQHandler(&CommunicationService_W806UART1->_huart);
	}
	void CommunicationService_W806UART::UART2_5_IRQHandler()
	{
    	HAL_UART_IRQHandler(&CommunicationService_W806UART2->_huart);
    	HAL_UART_IRQHandler(&CommunicationService_W806UART3->_huart);
    	HAL_UART_IRQHandler(&CommunicationService_W806UART4->_huart);
    	HAL_UART_IRQHandler(&CommunicationService_W806UART5->_huart);
	}
	void CommunicationService_W806UART::RxCpltCallback() 
	{
		_fifo.Write(_huart.pRxBuffPtr, _huart.RxXferCount);
	}
	void CommunicationService_W806UART::Flush()
	{
		const size_t len = _fifo.Length();
		void *buf = new uint8_t[len];
		_fifo.Peak(buf, len);
		_fifo.Pop(Receive(buf, len));

		delete buf;
	}
}
extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	switch ((size_t)huart->Instance)
	{
	case UART0_BASE:
		return EmbeddedIOServices::CommunicationService_W806UART::CommunicationService_W806UART0->RxCpltCallback();
	case UART1_BASE:
		return EmbeddedIOServices::CommunicationService_W806UART::CommunicationService_W806UART1->RxCpltCallback();
	case UART2_BASE:
		return EmbeddedIOServices::CommunicationService_W806UART::CommunicationService_W806UART2->RxCpltCallback();
	case UART3_BASE:
		return EmbeddedIOServices::CommunicationService_W806UART::CommunicationService_W806UART3->RxCpltCallback();
	case UART4_BASE:
		return EmbeddedIOServices::CommunicationService_W806UART::CommunicationService_W806UART4->RxCpltCallback();
	case UART5_BASE:
		return EmbeddedIOServices::CommunicationService_W806UART::CommunicationService_W806UART5->RxCpltCallback();
	}
}
extern "C" __attribute__((isr)) void UART0_IRQHandler(void)
{
    EmbeddedIOServices::CommunicationService_W806UART::UART0_IRQHandler();
}
extern "C" __attribute__((isr)) void UART1_IRQHandler(void)
{
    EmbeddedIOServices::CommunicationService_W806UART::UART1_IRQHandler();
}
extern "C" __attribute__((isr)) void UART2_5_IRQHandler(void)
{
    EmbeddedIOServices::CommunicationService_W806UART::UART2_5_IRQHandler();
}
#endif
