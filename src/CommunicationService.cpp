#include <algorithm>
#include <stdint.h>
#include "ICommunicationService.h"

#ifdef ICOMMUNICATIONSERVICE_H
namespace EmbeddedIOServices
{	
	void ICommunicationService::RegisterHandler(CommunicationHandler *handler)
	{
		_handlerList.push_back(handler);
	}

	void ICommunicationService::UnRegisterHandler(CommunicationHandler *handler)
	{
		_handlerList.remove(handler);
	}

	void ICommunicationService::Receive(void *data, size_t length)
	{
		CommunicationHandlerList::iterator next = _handlerList.begin();
		CommunicationHandlerList::iterator end = _handlerList.end();
		while(next != end)
		{
			if((*next)->CallBack(data, length))
				break;
			next++;
		}
	}
}
#endif
