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
				esp_err_t ret = httpd_ws_recv_frame(req, &ws_pkt, 0);
				if (ret == ESP_OK) 
				{
					ws_pkt.payload = reinterpret_cast<uint8_t*>(malloc(ws_pkt.len));
					ret = httpd_ws_recv_frame(req, &ws_pkt, ws_pkt.len);
					if (ret == ESP_OK) 
					{
						if(ws_pkt.type == HTTPD_WS_TYPE_BINARY)
							commService->Receive(ws_pkt.payload, ws_pkt.len);
						if(ws_pkt.type == HTTPD_WS_TYPE_CLOSE)
							commService->_fds.remove(httpd_req_to_sockfd(req));
					}
					free(ws_pkt.payload);
				}

				return ESP_OK;
			},
			.user_ctx   = this,
			.is_websocket = true,
			.handle_ws_control_frames = true
		};

        httpd_register_uri_handler(_server, &ws);
	}
	
	void Esp32IdfCommunicationService_WebSocket::Flush()
	{
		if(_txFlushing)
			return;
		_txFlushing = true;
		for(auto next = _fds.begin(); next != _fds.end(); ++next)
		{
			size_t length = _txBufEndIndex;
			length += (length < _txBufStartIndex? sizeof(_txBuf) : 0) - _txBufStartIndex;

			if(length < 1)
			{
				_txFlushing = false;
				return;
			}

			struct ws_send_arg_t
			{
				Esp32IdfCommunicationService_WebSocket *commService;
				httpd_ws_frame_t ws_pkt;
			};
			ws_send_arg_t *ws_send_arg = reinterpret_cast<ws_send_arg_t *>(calloc(1, sizeof(ws_send_arg_t)));
			ws_send_arg->commService = this;
		
			ws_send_arg->ws_pkt.len = length;
			ws_send_arg->ws_pkt.type = HTTPD_WS_TYPE_BINARY;
			ws_send_arg->ws_pkt.payload = reinterpret_cast<uint8_t*>(malloc(ws_send_arg->ws_pkt.len));
			if(_txBufStartIndex + ws_send_arg->ws_pkt.len >= sizeof(_txBuf))
			{
				const size_t lengthToEnd = sizeof(_txBuf) - _txBufStartIndex;
				memcpy(ws_send_arg->ws_pkt.payload, &_txBuf[_txBufStartIndex], lengthToEnd);
				memcpy(&ws_send_arg->ws_pkt.payload[lengthToEnd], _txBuf, ws_send_arg->ws_pkt.len - lengthToEnd);
			}
			else
			{
				memcpy(ws_send_arg->ws_pkt.payload, &_txBuf[_txBufStartIndex], ws_send_arg->ws_pkt.len);
			}
			_txBufStartIndex = (_txBufStartIndex + ws_send_arg->ws_pkt.len) % sizeof(_txBuf);

    		httpd_ws_send_data_async(_server, (*next), &ws_send_arg->ws_pkt, [](esp_err_t err, int socket, void *arg) 
			{ 
				ws_send_arg_t *ws_send_arg = reinterpret_cast<ws_send_arg_t *>(arg);
				free(ws_send_arg->ws_pkt.payload);
				ws_send_arg->commService->_txFlushing = false;
				ws_send_arg->commService->Flush();
				free(ws_send_arg);
			}, ws_send_arg);
		}
	}

	void Esp32IdfCommunicationService_WebSocket::Send(const void *data, size_t length)
	{
		if(_txBufEndIndex + length >= sizeof(_txBuf))
		{
			const size_t lengthToEnd = sizeof(_txBuf) - _txBufEndIndex;
			memcpy(&_txBuf[_txBufEndIndex], data, lengthToEnd);
			memcpy(_txBuf, &reinterpret_cast<const uint8_t *>(data)[lengthToEnd], length - lengthToEnd);
		}
		else
		{
			memcpy(&_txBuf[_txBufEndIndex], data, length);
		}
		_txBufEndIndex = (_txBufEndIndex + length) % sizeof(_txBuf);
		Flush();
	}
}