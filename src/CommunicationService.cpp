#include <algorithm>
#include <stdint.h>
#include "ICommunicationService.h"

#ifdef ICOMMUNICATIONSERVICE_H
namespace EmbeddedIOServices
{	
	void ICommunicationService::RegisterReceiveCallBack(communication_receive_callback_t *receiveCallBack)
	{
		_receiveCallBackList.push_back(receiveCallBack);
	}

	void ICommunicationService::UnRegisterReceiveCallBack(communication_receive_callback_t *receiveCallBack)
	{
		_receiveCallBackList.remove(receiveCallBack);
	}

	size_t ICommunicationService::Receive(void *data, size_t length)
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
			const size_t handledThisTime = (**next)(reinterpret_cast<uint8_t *>(data) + handled, length - handled);
			//add to the amount of data handled
			handled += handledThisTime;
			//if data was handled, go back to the beginning of the callback list
			if(handledThisTime > 0)
				next = begin;
			//other wise increment the looping iterator
			else
				next++;
		}

		//return the amount of data handled
		return handled;
	}
}
#endif
