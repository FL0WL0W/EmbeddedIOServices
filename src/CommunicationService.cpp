#include <algorithm>
#include "ICommunicationService.h"

#ifdef ICOMMUNICATIONSERVICE_H
namespace EmbeddedIOServices
{	
	communication_receive_callback_list_t::iterator ICommunicationService::RegisterReceiveCallBack(communication_receive_callback_t receiveCallBack)
	{
		return _receiveCallBackList.insert(_receiveCallBackList.end(), receiveCallBack);
	}

	void ICommunicationService::UnRegisterReceiveCallBack(communication_receive_callback_list_t::iterator receiveCallBackIterator)
	{
		_receiveCallBackList.erase(receiveCallBackIterator);
	}

	size_t ICommunicationService::Receive(const void *data, size_t length)
	{
		//grab const iterators of the beginning and ending of the callback list
		const communication_receive_callback_list_t::iterator begin = _receiveCallBackList.begin();
		const communication_receive_callback_list_t::iterator end = _receiveCallBackList.end();

		//declare the iterator that will loop through the list
		communication_receive_callback_list_t::iterator next = begin;
		//declare the variable for the amount of data handled
		size_t handled = 0;
		//while there is still data and not at the end of the callback list
		while(length > handled && next != end)
		{
			//call the callback
			const size_t handledThisTime = (*next)(
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
