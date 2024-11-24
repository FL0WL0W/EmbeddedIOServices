#include "ICommunicationService.h"
#include "esp_http_server.h"

#ifndef ESP32IDFCOMMUNICATIONSERVICE_WEBSOCKET_H
#define ESP32IDFCOMMUNICATIONSERVICE_WEBSOCKET_H
namespace Esp32
{
	class Esp32IdfCommunicationService_WebSocket : public EmbeddedIOServices::ICommunicationService
	{
	protected:
		uint8_t buf[1024];
		httpd_handle_t _server;
		std::list<int> _fds;
	public:
        Esp32IdfCommunicationService_WebSocket(httpd_handle_t server, const char *uri);
        void Send(const void *data, size_t length);
	};
}
#endif
