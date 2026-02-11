#include "ICommunicationService.h"
#include "driver/uart.h"

#ifndef ESP32IDFCOMMUNICATIONSERVICE_UART_H
#define ESP32IDFCOMMUNICATIONSERVICE_UART_H
namespace Esp32
{
	class Esp32IdfCommunicationService_UART : public EmbeddedIOServices::ICommunicationService
	{
	protected:
		uart_port_t _uart_num;
		size_t _rx_buffer_size;
	public:
        Esp32IdfCommunicationService_UART(uart_port_t uart_num, size_t rx_buffer_size);
        void Send(const void *data, size_t length);
	};
}
#endif
