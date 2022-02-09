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
		const communication_receive_callback_list_t::iterator begin = _handlerList.begin();
		const communication_receive_callback_list_t::iterator end = _handlerList.end();
		communication_receive_callback_list_t::iterator next = begin;
		size_t handled = 0;
		while(next != end && length > handled)
		{
			const size_t handledThisTime = (**next)(reinterpret_cast<void *>(reinterpret_cast<uint8_t *>(data) + handled), length - handled);
			handled += handledThisTime;
			if(handledThisTime > 0)
				next = begin;
			else
				next++;
		}

		return handled;
	}
}
#endif
