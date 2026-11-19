#include "ICommunicationService.h"
#include <vector>

#ifndef AGGREGATECOMMUNICATIONSERVICE_H
#define AGGREGATECOMMUNICATIONSERVICE_H
namespace EmbeddedIOServices
{
	typedef uint32_t communication_send_callback_id_t;

	class AggregateCommunicationService : public ICommunicationService
	{
	protected:
		struct ServiceRegistration
		{
			ICommunicationService *Service;
			communication_receive_callback_id_t ReceiveCallBackId;
		};

		std::vector<ServiceRegistration> _services;
		communication_send_callback_id_t _nextSendCallBackId = 0;

	public:
		AggregateCommunicationService();
		AggregateCommunicationService(ICommunicationService *services[], size_t servicesLength);
		~AggregateCommunicationService() override;

		void AddService(ICommunicationService *service);
		void Send(const void *data, size_t length) override;
		bool Ready() override;
	};
}
#endif
