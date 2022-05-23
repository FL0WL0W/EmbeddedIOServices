#include "CommunicationService_W806UART.h"

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
		USART_TypeDef *uart, 
		size_t fifoSize=1024,
		uint32_t baud=115200, 
		uint32_t wordLength=UART_WORDLENGTH_8B, 
		uint32_t stopBits=UART_STOPBITS_1, 
		uint32_t parity=UART_PARITY_NONE
	)
	{
		CommunicationService_W806UART *ret;
		switch (*(size_t*)uart)
		{
		case UART0_BASE:
			if(CommunicationService_W806UART0 != 0)
				return ret;
			CommunicationService_W806UART0 = ret = new CommunicationService_W806UART(fifoSize);
			break;
		case UART1_BASE:
			if(CommunicationService_W806UART1 != 0)
				return ret;
			CommunicationService_W806UART1 = ret = new CommunicationService_W806UART(fifoSize);
			break;
		case UART2_BASE:
			if(CommunicationService_W806UART2 != 0)
				return ret;
			CommunicationService_W806UART2 = ret = new CommunicationService_W806UART(fifoSize);
			break;
		case UART3_BASE:
			if(CommunicationService_W806UART3 != 0)
				return ret;
			CommunicationService_W806UART3 = ret = new CommunicationService_W806UART(fifoSize);
			break;
		case UART4_BASE:
			if(CommunicationService_W806UART4 != 0)
				return ret;
			CommunicationService_W806UART4 = ret = new CommunicationService_W806UART(fifoSize);
			break;
		case UART5_BASE:
			if(CommunicationService_W806UART5 != 0)
				return ret;
			CommunicationService_W806UART5 = ret = new CommunicationService_W806UART(fifoSize);
			break;
		
		default:
			return ret;
		}

		ret->_huart.Instance = uart;
		ret->_huart.Init.BaudRate = baud;
		ret->_huart.Init.WordLength = wordLength;
		ret->_huart.Init.StopBits = stopBits;
		ret->_huart.Init.Parity = parity;
		ret->_huart.Init.Mode = UART_MODE_TX | UART_MODE_RX;
		ret->_huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;

		if (HAL_UART_Init(&ret->_huart) != HAL_OK)
		{
			delete ret;
			switch (*(size_t*)uart)
			{
			case UART0_BASE:
				return CommunicationService_W806UART0 = ret = 0;
			case UART1_BASE:
				return CommunicationService_W806UART1 = ret = 0;
			case UART2_BASE:
				return CommunicationService_W806UART2 = ret = 0;
			case UART3_BASE:
				return CommunicationService_W806UART3 = ret = 0;
			case UART4_BASE:
				return CommunicationService_W806UART4 = ret = 0;
			case UART5_BASE:
				return CommunicationService_W806UART5 = ret = 0;
			default:
				return ret;
			}
		}

		return ret;
	}
	CommunicationService_W806UART::CommunicationService_W806UART(size_t fifoSize) : _fifo(fifoSize)
	{
		HAL_UART_Receive_IT(&_huart, _buf, 0);
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
		_fifo.Write(_huart.pRxBuffPtr, _huart.RxXferCount)
	}
	void CommunicationService_W806UART::Flush()
	{
		const size_t len = _fifo.Length();
		void *buf = new void[len];
		_fifo.Peak(buf, len);
		_fifo.Pop(Receive(buf, len));

		delete buf;
	}
}
extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	switch (*(size_t*)huart->Instance)
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
