#include <algorithm>
#include "AggregateCommunicationService.h"
#include "CommunicationService_ISOTP.h"
#include "ICANService.h"

#ifdef ICANSERVICE_H
namespace EmbeddedIOServices
{	
	ICANService::~ICANService()
	{
		for(const auto& aggregateService : _isotpAggregateServices)
		{
			delete aggregateService.Service;
		}

		for(const auto& service : _isotpServices)
		{
			delete service.Service;
		}
	}

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
			for (auto next = _receiveCallBackIdentifierIndex.begin(); next != _receiveCallBackIdentifierIndex.end();) 
			{
				if (next->second == find->second) 
				{
					next = _receiveCallBackIdentifierIndex.erase(next);
					continue;
				}
				++next;
			}
			_receiveCallBackMap.erase(find);
		}
	}

	void ICANService::UnRegisterReceiveCallBack(const CANIdentifier_t identifier, const CANIdentifier_t mask)
	{
		for (auto next = _receiveCallBackMaskMap.begin(); next != _receiveCallBackMaskMap.end();) 
		{
			if (next->second.Identifier == identifier && next->second.Mask == mask) 
			{
				next = _receiveCallBackMaskMap.erase(next);
				continue;
			}
			++next;
		}
	}

	void ICANService::UnRegisterReceiveCallBack(const CANIdentifier_t identifier)
	{
		for (auto find = _receiveCallBackIdentifierIndex.find(identifier); find != _receiveCallBackIdentifierIndex.end() && find->first == identifier;) 
		{
			for (auto next = _receiveCallBackMap.begin(); next != _receiveCallBackMap.end();) 
			{
				if (next->second == find->second) 
				{
					next = _receiveCallBackMap.erase(next);
					continue;
				}
				++next;
			}
			find = _receiveCallBackIdentifierIndex.erase(find);
		}
	}

	ICommunicationService *ICANService::GetISOTPService(const CANIdentifier_t listenIds[], const size_t listenIdsLength, const CANIdentifier_t transmitIds[], const size_t transmitIdsLength)
	{
		std::vector<CANIdentifier_t> listenIdList;
		std::vector<CANIdentifier_t> transmitIdList;

		if(listenIds != nullptr)
		{
			listenIdList.assign(listenIds, listenIds + listenIdsLength);
		}

		if(transmitIds != nullptr)
		{
			transmitIdList.assign(transmitIds, transmitIds + transmitIdsLength);
		}

		for(const auto& aggregateService : _isotpAggregateServices)
		{
			if(aggregateService.ListenIds == listenIdList && aggregateService.TransmitIds == transmitIdList)
			{
				return aggregateService.Service;
			}
		}

		ISOTPAggregateServiceRegistration aggregateService;
		aggregateService.ListenIds = listenIdList;
		aggregateService.TransmitIds = transmitIdList;
		aggregateService.Service = new AggregateCommunicationService();

		for(const CANIdentifier_t listenId : listenIdList)
		{
			CommunicationService_ISOTP *isotpService = nullptr;

			for(const auto& service : _isotpServices)
			{
				if(service.ListenId == listenId)
				{
					isotpService = service.Service;
					break;
				}
			}

			if(isotpService == nullptr)
			{
				const CANIdentifier_t listenIdsForService[1] = { listenId };
				ISOTPServiceRegistration service;
				service.ListenId = listenId;
				service.Service = new CommunicationService_ISOTP(this, listenIdsForService, 1, nullptr, 0);
				isotpService = service.Service;
				_isotpServices.push_back(std::move(service));
			}

			aggregateService.Service->AddService(isotpService);
		}

		for(const CANIdentifier_t transmitId : transmitIdList)
		{
			aggregateService.Service->RegisterSendCallBack([this, transmitId](const void *data, size_t length) {
				CommunicationService_ISOTP::Send(this, transmitId, data, length);
			});
		}

		_isotpAggregateServices.push_back(std::move(aggregateService));
		return _isotpAggregateServices.back().Service;
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
