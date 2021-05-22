#include "ICallBack.h"
#include "stdint.h"

#ifndef ICOMMUNICATIONSERVICE_H
#define ICOMMUNICATIONSERVICE_H

namespace EmbeddedIOServices
{
	class ICommunicationService
	{
	public:
		uint32_t Available();
		void Receive(uint8_t *buffer, uint32_t length);
		void Transmit(uint8_t *buffer, uint32_t length);
	};
}
#endif
