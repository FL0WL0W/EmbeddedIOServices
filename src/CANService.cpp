#include <algorithm>
#include "ICANService.h"

#ifdef ICANSERVICE_H
namespace EmbeddedIOServices
{	
	can_receive_callback_map_t::iterator ICANService::RegisterReceiveCallBack(const uint32_t identifier, can_receive_callback_t receiveCallBack)
	{
		//add too callback map
		return _receiveCallBackMap.insert(can_receive_callback_map_t::value_type(
			identifier,
			receiveCallBack
		));
	}

	void ICANService::UnRegisterReceiveCallBack(can_receive_callback_map_t::iterator receiveCallBack)
	{
		_receiveCallBackMap.erase(receiveCallBack);
	}

	void ICANService::UnRegisterReceiveCallBack(const uint32_t identifier)
	{
		//grab const iterators of the ending of the callback map
		const can_receive_callback_map_t::iterator end = _receiveCallBackMap.end();

		//find all callbacks matching identifier
		can_receive_callback_map_t::iterator next = _receiveCallBackMap.find(identifier);

		//loop through matching callbacks
		while(next != end && next->first == identifier)
		{
			//erase from map
			_receiveCallBackMap.erase(next++);
		}
	}

	void ICANService::Receive(const uint32_t identifier, const uint8_t data[8])
	{
		//grab const iterators of the ending of the callback map
		const can_receive_callback_map_t::iterator end = _receiveCallBackMap.end();

		//find all callbacks matching identifier
		can_receive_callback_map_t::iterator next = _receiveCallBackMap.find(identifier);

		//loop through matching callbacks
		while(next != end && next->first == identifier)
		{
			//call the callback
			next->second( [this](const uint32_t identifier, const uint8_t data[8]) { Send(identifier, data); }, data );

			//increment the looping iterator
			next++;
		}
	}
}
#endif
