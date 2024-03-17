#include "ICANService.h"

#ifndef MOCKCANSERVICE_H
#define MOCKCANSERVICE_H
namespace EmbeddedIOServices
{
	class MockCANService : public ICANService
	{
	public:
		MOCK_METHOD2(Send, void(const uint32_t identifier, const uint8_t data[8]));
	};
}
#endif