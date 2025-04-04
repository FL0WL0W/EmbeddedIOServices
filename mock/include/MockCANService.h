#include "ICANService.h"

#ifndef MOCKCANSERVICE_H
#define MOCKCANSERVICE_H
namespace EmbeddedIOServices
{
	class MockCANService : public ICANService
	{
	public:
		MOCK_METHOD3(Send, void(const CANIdentifier_t identifier, const CANData_t data, const uint8_t dataLength));
	};
}
#endif