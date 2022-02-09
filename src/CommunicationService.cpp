#include <algorithm>
#include <stdint.h>
#include "ICommunicationService.h"

#ifdef ICOMMUNICATIONSERVICE_H
namespace EmbeddedIOServices
{	
	void ICommunicationService::RegisterHandler(communication_callback_t *handler)
	{
		_handlerList.push_back(handler);
	}

	void ICommunicationService::UnRegisterHandler(communication_callback_t *handler)
	{
		_handlerList.remove(handler);
	}

	size_t ICommunicationService::Receive(void *data, size_t length)
	{
		CommunicationHandlerList::iterator next = _handlerList.begin();
		CommunicationHandlerList::iterator end = _handlerList.end();
		size_t handled = 0;
		while(next != end && length > handled)
		{
			handled += (**next)(reinterpret_cast<void *>(reinterpret_cast<uint8_t *>(data) + handled), length - handled);
			next++;
		}

		return handled;
	}
}
#endif
