#include <algorithm>
#include "ICommunicationService.h"

#ifdef ICOMMUNICATIONSERVICE_H
namespace EmbeddedIOServices
{	
	void ICommunicationService::RegisterHandler(ICommunicationHandler *handler)
	{
		if(_receiveCallBackMap.find(handler) != _receiveCallBackMap.end())
			return;

		_receiveCallBackMap.insert(std::pair<void *, communication_receive_callback_t *>(
			handler, 
			new communication_receive_callback_t([handler](communication_send_callback_t sendCallBack, void *data, size_t length) { return handler->Receive(sendCallBack, data, length); })
		));
	}

	void ICommunicationService::UnRegisterHandler(ICommunicationHandler *handler)
	{
		communication_receive_callback_map_t::iterator it = _receiveCallBackMap.find(handler);
		if(it == _receiveCallBackMap.end())
			return;

		delete it->second;
		_receiveCallBackMap.erase(handler);
	}

	void ICommunicationService::RegisterReceiveCallBack(communication_receive_callback_t *receiveCallBack)
	{
		if(_receiveCallBackMap.find(receiveCallBack) != _receiveCallBackMap.end())
			return;

		_receiveCallBackMap.insert(std::pair<void *, communication_receive_callback_t *>(
			receiveCallBack, 
			receiveCallBack
		));
	}

	void ICommunicationService::UnRegisterReceiveCallBack(communication_receive_callback_t *receiveCallBack)
	{
		_receiveCallBackMap.erase(receiveCallBack);
	}

	size_t ICommunicationService::Receive(void *data, size_t length)
	{
		//grab const iterators of the beginning and ending of the callback list
		const communication_receive_callback_map_t::iterator begin = _receiveCallBackMap.begin();
		const communication_receive_callback_map_t::iterator end = _receiveCallBackMap.end();

		//declare the iterator that will loop through the list
		communication_receive_callback_map_t::iterator next = begin;
		//declare the variable for the amount of data handled
		size_t handled = 0;
		//while there is still data and not at the end of the callback list
		while(length > handled && next != end)
		{
			//call the callback
			const size_t handledThisTime = (*next->second)(
				[this](const void *data, size_t length) { Send(data, length); },
				reinterpret_cast<uint8_t *>(data) + handled, 
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
