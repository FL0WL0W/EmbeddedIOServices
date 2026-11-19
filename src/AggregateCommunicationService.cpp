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
			[this](ICommunicationService *sendService, const void *data, size_t length) {
				Receive(data, length);
				return 0;
			}
		);

		_services.push_back({ service, receiveCallBackId });
	}

	void AggregateCommunicationService::Send(const void *data, size_t length)
	{
		for(const auto& service : _services)
		{
			service.Service->Send(data, length);
		}
	}

	bool AggregateCommunicationService::Ready()
	{
		for(const auto& service : _services)
		{
			if(!service.Service->Ready())
				return false;
		}
		return true;
	}
}
#endif
