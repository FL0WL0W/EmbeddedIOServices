#include "Esp32IdfCommunicationService_Socket.h"
#include "lwip/sockets.h"
#include "esp_log.h"

using namespace EmbeddedIOServices;

namespace Esp32
{
	Esp32IdfCommunicationService_Socket::Esp32IdfCommunicationService_Socket(uint16_t port, size_t rx_buffer_size)
		: _rx_buffer_size(rx_buffer_size)
	{
		struct sockaddr_storage dest_addr;
		struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
		dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
		dest_addr_ip4->sin_family = AF_INET;
		dest_addr_ip4->sin_port = htons(port);

		_listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
		if (_listen_sock < 0) 
		{
			ESP_LOGE("CommunicationService_Socket", "Unable to create socket: errno %d", errno);
			return;
		}
		int opt = 1;
		setsockopt(_listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

		int err = bind(_listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
		if (err != 0) 
		{
			ESP_LOGE("CommunicationService_Socket", "Socket unable to bind: errno %d", errno);
			close(_listen_sock);
			return;
		}

		err = listen(_listen_sock, 1);
		if (err != 0) 
		{
			ESP_LOGE("CommunicationService_Socket", "Error occurred during listen: errno %d", errno);
			close(_listen_sock);
			return;
		}
		// TODO probably need to stop these tasks when service is deleted
		xTaskCreate([] (void *arg)
		{
			Esp32IdfCommunicationService_Socket *commService = reinterpret_cast<Esp32IdfCommunicationService_Socket *>(arg);
			int keepAlive = 1;
			int keepIdle = 5;
			int keepInterval = 5;
			int keepCount = 3;
			int noDelay = 1;
			int tos = IPTOS_LOWDELAY;

			while (1) 
			{
				struct sockaddr_storage source_addr;
				socklen_t addr_len = sizeof(source_addr);
				int sock = accept(commService->_listen_sock, (struct sockaddr *)&source_addr, &addr_len);
				if (sock < 0) {
					ESP_LOGE("CommunicationService_Socket", "Unable to accept connection: errno %d", errno);
					continue;
				}

				// Set tcp keepalive option
				setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof(int));
				setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &keepIdle, sizeof(int));
				setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &keepInterval, sizeof(int));
				setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &keepCount, sizeof(int));
				// set no delay
				setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &noDelay, sizeof(int));
				setsockopt(sock, IPPROTO_IP, IP_TOS, &tos, sizeof(int));

				struct sockAndThis 
				{
					int sock;
					Esp32IdfCommunicationService_Socket *commService;
				};

				sockAndThis *sockArg = new sockAndThis({
					sock = sock,
					commService = commService
				});

				xTaskCreate([] (void *arg) 
				{
					sockAndThis *sockArg = reinterpret_cast<sockAndThis *>(arg);
					int sock = sockArg->sock;
					Esp32IdfCommunicationService_Socket *commService = sockArg->commService;

					commService->_socks.push_back(sock);

					int len;
				uint8_t rx_buffer[commService->_rx_buffer_size];
					while(1)
					{
						len = recv(sock, rx_buffer, sizeof(rx_buffer), 0);
						if (len == 0 || len == -1) {
							ESP_LOGW("CommunicationService_Socket", "Connection closed");
							goto sock_read_cleanup;
						} else if (len < 0) {
							ESP_LOGE("CommunicationService_Socket", "Error occurred during receiving: errno %d", len);
						} else {
							commService->Receive(rx_buffer, len);
						}
					}

sock_read_cleanup:
					commService->_socks.remove(sock);
					shutdown(sock, 0);
					close(sock);
					vTaskDelete(NULL);
					delete sockArg;

				}, "CommunicationService_Socket_Read", commService->_rx_buffer_size + 2048, sockArg, 10, NULL);
			}
			
			close(commService->_listen_sock);
			vTaskDelete(NULL);
		}, "CommunicationService_Socket", 4096, this, 10, NULL);
	}
	
	void Esp32IdfCommunicationService_Socket::Send(const void *data, size_t length)
	{
		for(auto next = _socks.begin(); next != _socks.end(); ++next)
		{
			send(*next, data, length, 0);
			next++;
		}
	}
}