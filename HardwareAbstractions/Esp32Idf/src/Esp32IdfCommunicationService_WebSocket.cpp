#include "Esp32IdfCommunicationService_WebSocket.h"
#include "esp_log.h"

using namespace EmbeddedIOServices;

namespace Esp32
{
	Esp32IdfCommunicationService_WebSocket::Esp32IdfCommunicationService_WebSocket(httpd_handle_t server, const char *uri) : _server(server)
	{
		const httpd_uri_t ws = {
			.uri        = uri,
			.method     = HTTP_GET,
			.handler    = [](httpd_req_t *req) 
			{
				Esp32IdfCommunicationService_WebSocket *commService = reinterpret_cast<Esp32IdfCommunicationService_WebSocket *>(req->user_ctx);
				if (req->method == HTTP_GET) {
					commService->_fds.push_back(httpd_req_to_sockfd(req));
					return ESP_OK;
				}
				httpd_ws_frame_t ws_pkt;
				memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
        		ws_pkt.payload = commService->buf;
				esp_err_t ret = httpd_ws_recv_frame(req, &ws_pkt, 1024);
				if (ret == ESP_OK) 
				{
					if(ws_pkt.type == HTTPD_WS_TYPE_BINARY)
						commService->Receive(ws_pkt.payload, ws_pkt.len);
					if(ws_pkt.type == HTTPD_WS_TYPE_CLOSE)
						commService->_fds.remove(httpd_req_to_sockfd(req));
				}

				return ESP_OK;
			},
			.user_ctx   = this,
			.is_websocket = true,
			.handle_ws_control_frames = true
		};

        httpd_register_uri_handler(_server, &ws);
	}
	
	void Esp32IdfCommunicationService_WebSocket::Send(const void *data, size_t length)
	{
		for(auto next = _fds.begin(); next != _fds.end(); ++next)
		{
			httpd_ws_frame_t *ws_pkt = reinterpret_cast<httpd_ws_frame_t *>(calloc(1, sizeof(httpd_ws_frame_t)));
			ws_pkt->len = length;
			ws_pkt->type = HTTPD_WS_TYPE_BINARY;
			ws_pkt->payload = reinterpret_cast<uint8_t*>(malloc(length));
			memcpy(ws_pkt->payload, data, length);
    		httpd_ws_send_data_async(_server, (*next), ws_pkt, [](esp_err_t err, int socket, void *arg) 
			{ 
				httpd_ws_frame_t *ws_pkt = reinterpret_cast<httpd_ws_frame_t *>(arg);
				free(ws_pkt->payload);
				free(ws_pkt);
			}, ws_pkt);
		}
	}
}