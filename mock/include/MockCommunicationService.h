#include "ICommunicationService.h"

#ifndef MOCKCOMMUNICATIONSERVICE_H
#define MOCKCOMMUNICATIONSERVICE_H
namespace EmbeddedIOServices
{
	class MockCommunicationService : public ICommunicationService
	{
	public:
		MOCK_METHOD2(Send, void(const void *data, size_t length));
	};
}
#endif