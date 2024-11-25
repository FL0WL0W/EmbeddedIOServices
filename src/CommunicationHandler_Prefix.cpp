#include <algorithm>
#include <cstring>
#include "CommunicationHandler_Prefix.h"

#ifdef COMMUNICATIONHANDLER_PREFIX_H
namespace EmbeddedIOServices
{	
	communication_prefix_receive_callback_id_t CommunicationHandler_Prefix::RegisterReceiveCallBack(communication_receive_callback_t receiveCallBack, const void *prefix, const size_t prefixLength, const bool handlesData)
	{
		_receiveCallBackMap.insert({_nextId, communication_prefix_receive_callback_t(
				receiveCallBack,
				prefix,
				prefixLength,
				handlesData
			)
		});
		return _nextId++;
	}

	void CommunicationHandler_Prefix::UnRegisterReceiveCallBack(communication_prefix_receive_callback_id_t receiveCallBackId)
	{
		_receiveCallBackMap.erase(receiveCallBackId);
	}

	void CommunicationHandler_Prefix::UnRegisterReceiveCallBack(const void *prefix, const size_t prefixLength)
	{
		//grab const iterators of the beginning and ending of the callback list
		auto begin = _receiveCallBackMap.begin();
		auto end = _receiveCallBackMap.end();

		while(true)
		{
			auto find = std::find_if(begin, end, [&](std::pair<communication_prefix_receive_callback_id_t, communication_prefix_receive_callback_t> next) {
				return prefixLength == next.second.PrefixLength && std::memcmp(prefix, next.second.Prefix, prefixLength) == 0;
			});

			if(find == end)
				return;

			_receiveCallBackMap.erase(find);
		}
	}

	size_t CommunicationHandler_Prefix::Receive(communication_send_callback_t sendCallBack, const void *data, size_t length)
	{
		size_t handled = 0;
		for (auto next = _receiveCallBackMap.begin(); next != _receiveCallBackMap.end() && length > handled; ++next) 
		{
			if(next->second.PrefixLength <= length && std::strncmp(reinterpret_cast<const char *>(data) + handled, reinterpret_cast<const char *>(next->second.Prefix), next->second.PrefixLength) == 0)
			{
				//call the callback
				const size_t handledThisTime = next->second.CallBack(
					[sendCallBack](const void *data, size_t length) { sendCallBack(data, length); },
					reinterpret_cast<const uint8_t *>(data) + handled + next->second.PrefixLength, 
					length - handled - next->second.PrefixLength
				);
				//if data was handled, go back to the beginning of the callback list
				if(!next->second.HandlesData || handledThisTime > 0)
				{
					//add to the amount of data handled
					handled += handledThisTime + next->second.PrefixLength;
					next = _receiveCallBackMap.begin();
					continue;
				}
			}
		}

		//return the amount of data handled
		return handled;
	}
}
#endif
