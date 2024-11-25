#include "ICommunicationService.h"
#include <list>

#ifndef ESP32IDFCOMMUNICATIONSERVICE_SOCKET_H
#define ESP32IDFCOMMUNICATIONSERVICE_SOCKET_H
namespace Esp32
{
	class Esp32IdfCommunicationService_Socket : public EmbeddedIOServices::ICommunicationService
	{
	protected:
		std::list<int> _socks;
		int _listen_sock;
	public:
        Esp32IdfCommunicationService_Socket(uint16_t port);
        void Send(const void *data, size_t length);
	};
}
#endif
