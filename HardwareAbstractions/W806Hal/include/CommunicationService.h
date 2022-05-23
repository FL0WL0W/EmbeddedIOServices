#include "ICommunicationService.h"

#ifndef COMMUNICATIONSERVICE_H
#define COMMUNICATIONSERVICE_H
namespace EmbeddedIOServices
{
	class CommunicationService : public ICommunicationService
	{
	public:
        void Send(const void *data, size_t length);
	};
}
#endif
