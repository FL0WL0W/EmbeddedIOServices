#include "AggregateCommunicationService.h"

#ifdef AGGREGATECOMMUNICATIONSERVICE_H
namespace EmbeddedIOServices
{
	AggregateCommunicationService::AggregateCommunicationService()
	{
	}

	AggregateCommunicationService::AggregateCommunicationService(ICommunicationService *services[], size_t servicesLength)
	{
		for(size_t i = 0; i < servicesLength; ++i)
		{
			AddService(services[i]);
		}
	}

	AggregateCommunicationService::~AggregateCommunicationService()
	{
		for(const auto& service : _services)
		{
			service.Service->UnRegisterReceiveCallBack(service.ReceiveCallBackId);
		}
	}

	void AggregateCommunicationService::AddService(ICommunicationService *service)
	{
		if(service == nullptr)
		{
			return;
		}

		const communication_receive_callback_id_t receiveCallBackId = service->RegisterReceiveCallBack(
			[this](communication_send_callback_t send, const void *data, size_t length) {
				Receive(data, length);
				return 0;
			}
		);

		_services.push_back({ service, receiveCallBackId });
	}

	communication_send_callback_id_t AggregateCommunicationService::RegisterSendCallBack(communication_send_callback_t sendCallBack)
	{
		_sendCallBackMap.insert({ _nextSendCallBackId, sendCallBack });
		return _nextSendCallBackId++;
	}

	void AggregateCommunicationService::UnRegisterSendCallBack(communication_send_callback_id_t sendCallBackId)
	{
		_sendCallBackMap.erase(sendCallBackId);
	}

	void AggregateCommunicationService::Send(const void *data, size_t length)
	{
		for(const auto& sendCallBack : _sendCallBackMap)
		{
			sendCallBack.second(data, length);
		}

		for(const auto& service : _services)
		{
			service.Service->Send(data, length);
		}
	}
}
#endif
