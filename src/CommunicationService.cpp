#include <algorithm>
#include "ICommunicationService.h"

#ifdef ICOMMUNICATIONSERVICE_H
namespace EmbeddedIOServices
{	
	communication_receive_callback_id_t ICommunicationService::RegisterReceiveCallBack(communication_receive_callback_t receiveCallBack)
	{
		_receiveCallBackMap.insert({ _nextId, receiveCallBack });
		return _nextId++;
	}

	void ICommunicationService::UnRegisterReceiveCallBack(communication_receive_callback_id_t receiveCallBackId)
	{
		_receiveCallBackMap.erase(receiveCallBackId);
	}

	size_t ICommunicationService::Receive(const void *data, size_t length)
	{
		//grab const iterators of the beginning and ending of the callback list
		const auto begin = _receiveCallBackMap.begin();
		const auto end = _receiveCallBackMap.end();

		//declare the iterator that will loop through the list
		auto next = begin;
		//declare the variable for the amount of data handled
		size_t handled = 0;
		//while there is still data and not at the end of the callback list
		while(length > handled && next != end)
		{
			//call the callback
			const size_t handledThisTime = next->second(
				[this](const void *data, size_t length) { Send(data, length); },
				reinterpret_cast<const uint8_t *>(data) + handled, 
				length - handled
			);
			//if data was handled, go back to the beginning of the callback list
			if(handledThisTime > 0)
			{
				//add to the amount of data handled
				handled += handledThisTime;
				next = begin;
				continue;
			}
			//other wise increment the looping iterator
			next++;
		}

		//return the amount of data handled
		return handled;
	}
}
#endif
