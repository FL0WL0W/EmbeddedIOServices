#include "ICommunicationService.h"

#ifndef MOCKTIMERSERVICE_H
#define MOCKTIMERSERVICE_H
namespace EmbeddedIOServices
{
	class MockCommunicationService : public ICommunicationService
	{
	public:
		MOCK_METHOD2(Send, void(void *data, size_t length));
	};
}
#endif