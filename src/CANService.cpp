#include <algorithm>
#include "ICANService.h"

#ifdef ICANSERVICE_H
namespace EmbeddedIOServices
{	
	can_receive_callback_id_t ICANService::RegisterReceiveCallBack(const CANIdentifier_t identifier, const CANIdentifier_t mask, can_receive_mask_callback_t receiveCallBack)
	{
		const can_receive_callback_mask_t can_receive_callback_mask = { identifier, mask, receiveCallBack };
		_receiveCallBackMaskMap.insert({ _nextId, can_receive_callback_mask });
		return _nextId++;
	}

	can_receive_callback_id_t ICANService::RegisterReceiveCallBack(const CANIdentifier_t identifier, can_receive_callback_t receiveCallBack)
	{
		std::shared_ptr<can_receive_callback_t> receiveCallBackShared = std::make_shared<can_receive_callback_t>(receiveCallBack);
		_receiveCallBackIdentifierIndex.insert({ identifier, receiveCallBackShared });
		_receiveCallBackMap.insert({ _nextId, receiveCallBackShared });
		return _nextId++;
	}

	void ICANService::UnRegisterReceiveCallBack(can_receive_callback_id_t receiveCallBackId)
	{
		_receiveCallBackMaskMap.erase(receiveCallBackId);

		auto find = _receiveCallBackMap.find(receiveCallBackId);
		if (find != _receiveCallBackMap.end()) 
		{
			for (auto next = _receiveCallBackIdentifierIndex.begin(); next != _receiveCallBackIdentifierIndex.end(); ++next) 
			{
				if (next->second == find->second) 
				{
					next = _receiveCallBackIdentifierIndex.erase(next);
				}
			}
			_receiveCallBackMap.erase(find);
		}
	}

	void ICANService::UnRegisterReceiveCallBack(const CANIdentifier_t identifier, const CANIdentifier_t mask)
	{
		for (auto next = _receiveCallBackMaskMap.begin(); next != _receiveCallBackMaskMap.end(); ++next) 
		{
			if (next->second.Identifier == identifier && next->second.Mask == mask) 
			{
				next = _receiveCallBackMaskMap.erase(next);
			}
		}
	}

	void ICANService::UnRegisterReceiveCallBack(const CANIdentifier_t identifier)
	{
		for (auto find = _receiveCallBackIdentifierIndex.find(identifier); find != _receiveCallBackIdentifierIndex.end() && find->first == identifier; ++find) 
		{
			for (auto next = _receiveCallBackMap.begin(); next != _receiveCallBackMap.end(); ++next) 
			{
				if (next->second == find->second) 
				{
					next = _receiveCallBackMap.erase(next);
				}
			}
			find = _receiveCallBackIdentifierIndex.erase(find);
		}
	}

	void ICANService::Receive(const CANIdentifier_t identifier, const CANData_t data, const uint8_t dataLength)
	{
		const can_send_callback_t send = [this](const CANIdentifier_t identifier, const CANData_t data, const uint8_t dataLength) { Send(identifier, data, dataLength); };
		
		for (auto find = _receiveCallBackIdentifierIndex.find(identifier); find != _receiveCallBackIdentifierIndex.end() && find->first == identifier; ++find) 
		{
			(*find->second)(send, data, dataLength );
		}

		for (auto next = _receiveCallBackMaskMap.begin(); next != _receiveCallBackMaskMap.end(); ++next) 
		{
			if ((next->second.Identifier & next->second.Mask) == (identifier & next->second.Mask))
			{
				next->second.CallBack(send, identifier, data, dataLength);
			}
		}
	}
}
#endif
