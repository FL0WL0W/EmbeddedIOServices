#include <algorithm>
#include <cstring>
#include "CommunicationHandler_Prefix.h"

#ifdef COMMUNICATIONHANDLER_PREFIX_H
namespace EmbeddedIOServices
{	
	communication_prefix_receive_callback_list_t::iterator CommunicationHandler_Prefix::RegisterReceiveCallBack(communication_receive_callback_t receiveCallBack, const void *prefix, const size_t prefixLength, const bool handlesData)
	{
		return _receiveCallBackList.insert(_receiveCallBackList.begin(), communication_prefix_receive_callback_t(
				receiveCallBack,
				prefix,
				prefixLength,
				handlesData
			)
		);
	}

	void CommunicationHandler_Prefix::UnRegisterReceiveCallBack(communication_prefix_receive_callback_list_t::iterator receiveCallBackIterator)
	{
		_receiveCallBackList.erase(receiveCallBackIterator);
	}

	void CommunicationHandler_Prefix::UnRegisterReceiveCallBack(const void *prefix, const size_t prefixLength)
	{
		//grab const iterators of the beginning and ending of the callback list
		const communication_prefix_receive_callback_list_t::iterator begin = _receiveCallBackList.begin();
		const communication_prefix_receive_callback_list_t::iterator end = _receiveCallBackList.end();

		//find all callbacks matching identifier
		communication_prefix_receive_callback_list_t::iterator next;
		while(true)
		{
			communication_prefix_receive_callback_list_t::iterator next = std::find_if(begin, end, [&](communication_prefix_receive_callback_t receivePrefixCallBack) {
				return prefixLength == receivePrefixCallBack.PrefixLength && std::memcmp(prefix, receivePrefixCallBack.Prefix, prefixLength) == 0;
			});

			if(next == end)
				return;

			_receiveCallBackList.erase(next);
		}
	}

	size_t CommunicationHandler_Prefix::Receive(communication_send_callback_t sendCallBack, void *data, size_t length)
	{
		//grab const iterators of the beginning and ending of the callback list
		const communication_prefix_receive_callback_list_t::iterator begin = _receiveCallBackList.begin();
		const communication_prefix_receive_callback_list_t::iterator end = _receiveCallBackList.end();

		//declare the iterator that will loop through the list
		communication_prefix_receive_callback_list_t::iterator next = begin;
		//declare the variable for the amount of data handled
		size_t handled = 0;
		//while there is still data and not at the end of the callback list
		while(length > handled && next != end)
		{
			if(next->PrefixLength <= length && std::strncmp(reinterpret_cast<const char *>(data), reinterpret_cast<const char *>(next->Prefix), next->PrefixLength) == 0)
			{
				//call the callback
				const size_t handledThisTime = next->CallBack(
					[sendCallBack](const void *data, size_t length) { sendCallBack(data, length); },
					reinterpret_cast<uint8_t *>(data) + handled + next->PrefixLength, 
					length - handled - next->PrefixLength
				);
				//if data was handled, go back to the beginning of the callback list
				if(!next->HandlesData || handledThisTime > 0)
				{
					//add to the amount of data handled
					handled += handledThisTime + next->PrefixLength;
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
