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
		std::map<const communication_send_callback_id_t, communication_send_callback_t> _sendCallBackMap;
		communication_send_callback_id_t _nextSendCallBackId = 0;

	public:
		AggregateCommunicationService();
		AggregateCommunicationService(ICommunicationService *services[], size_t servicesLength);
		~AggregateCommunicationService();

		void AddService(ICommunicationService *service);
		communication_send_callback_id_t RegisterSendCallBack(communication_send_callback_t sendCallBack);
		void UnRegisterSendCallBack(communication_send_callback_id_t sendCallBackId);
		void Send(const void *data, size_t length);
	};
}
#endif
