#include "ICommunicationService.h"
#include <functional>

#ifndef COMMUNICATIONSERVICESHIM_H
#define COMMUNICATIONSERVICESHIM_H
namespace EmbeddedIOServices
{
	class CommunicationServiceShim : public ICommunicationService
	{
	public:
		std::function<void(const void *data, size_t length)> SendFunction;
		bool IsReady;
		CommunicationServiceShim(std::function<void(const void *data, size_t length)> sendFunction, bool ready = true);

		void Send(const void *data, size_t length) override;
		bool Ready() override;
	};
}
#endif
