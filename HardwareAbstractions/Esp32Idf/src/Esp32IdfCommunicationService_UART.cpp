#include "Esp32IdfCommunicationService_UART.h"
#include "esp_log.h"
#include "soc/uart_reg.h"

using namespace EmbeddedIOServices;

namespace Esp32
{
	Esp32IdfCommunicationService_UART::Esp32IdfCommunicationService_UART(uart_port_t uart_num, size_t rx_buffer_size)
		: _uart_num(uart_num), _rx_buffer_size(rx_buffer_size)
	{
		if (!uart_is_driver_installed(uart_num))
			ESP_ERROR_CHECK(uart_driver_install(uart_num, rx_buffer_size, 0, 0, NULL, ESP_INTR_FLAG_IRAM));
		ESP_ERROR_CHECK(uart_set_rx_full_threshold(uart_num, 1));

		xTaskCreate([](void *arg)
		{
			Esp32IdfCommunicationService_UART *commService = reinterpret_cast<Esp32IdfCommunicationService_UART *>(arg);
			uint8_t rx_buffer[commService->_rx_buffer_size];

			while (1)
			{
				// Read data from the UART. have to do this weird read stuff below because read bytes tries to get all the bytes requested before timing out. not just what's available
				size_t len = uart_read_bytes(commService->_uart_num, rx_buffer, 1, pdMS_TO_TICKS(1000));
				size_t bufferedLen = 0;
				uart_get_buffered_data_len(commService->_uart_num, &bufferedLen);
				if (bufferedLen > 0)
				{
					len += uart_read_bytes(commService->_uart_num, rx_buffer + 1, bufferedLen > (commService->_rx_buffer_size - 1) ? (commService->_rx_buffer_size - 1) : bufferedLen, pdMS_TO_TICKS(1000));
				}
				// Write data to functions
				if (len)
				{
					commService->Receive(rx_buffer, len);
				}
			}
		}, "CommunicationService_UART_READ", rx_buffer_size + 2048, this, 10, NULL);
	}

	void Esp32IdfCommunicationService_UART::Send(const void *data, size_t length)
	{
		uart_write_bytes(_uart_num, data, length);
	}
}
