#include <algorithm>
#include <cstring>
#include "CommunicationHandler_Prefix.h"

#ifdef COMMUNICATIONHANDLER_PREFIX_H
namespace EmbeddedIOServices
{	
	void CommunicationHandler_Prefix::RegisterReceiveCallBack(communication_receive_callback_t *receiveCallBack, const void *prefix, const size_t prefixLength, const bool handlesData)
	{
		if(_receiveCallBackMap.find(receiveCallBack) != _receiveCallBackMap.end())
			return;

		_receiveCallBackMap.insert(std::pair<communication_receive_callback_t *, communication_prefix_receive_callback_t>(
			receiveCallBack,
			communication_prefix_receive_callback_t(
				prefix,
				prefixLength,
				handlesData
			)
		));
	}

	void CommunicationHandler_Prefix::UnRegisterReceiveCallBack(communication_receive_callback_t *receiveCallBack)
	{
		_receiveCallBackMap.erase(receiveCallBack);
	}

	size_t CommunicationHandler_Prefix::Receive(communication_send_callback_t sendCallBack, void *data, size_t length)
	{
		//grab const iterators of the beginning and ending of the callback list
		const communication_prefix_receive_callback_map_t::iterator begin = _receiveCallBackMap.begin();
		const communication_prefix_receive_callback_map_t::iterator end = _receiveCallBackMap.end();

		//declare the iterator that will loop through the list
		communication_prefix_receive_callback_map_t::iterator next = begin;
		//declare the variable for the amount of data handled
		size_t handled = 0;
		//while there is still data and not at the end of the callback list
		while(length > handled && next != end)
		{
			if(next->second.PrefixLength <= length && std::strncmp(reinterpret_cast<const char *>(data), reinterpret_cast<const char *>(next->second.Prefix), next->second.PrefixLength) == 0)
			{
				//call the callback
				const size_t handledThisTime = (*next->first)(
					[sendCallBack](const void *data, size_t length) { sendCallBack(data, length); },
					reinterpret_cast<uint8_t *>(data) + handled + next->second.PrefixLength, 
					length - handled - next->second.PrefixLength
				);
				//if data was handled, go back to the beginning of the callback list
				if(!next->second.HandlesData || handledThisTime > 0)
				{
					//add to the amount of data handled
					handled += handledThisTime + next->second.PrefixLength;
					next = begin;
					continue;
				}
			}
			next++;
		}

		//return the amount of data handled
		return handled;
	}
}
#endif
