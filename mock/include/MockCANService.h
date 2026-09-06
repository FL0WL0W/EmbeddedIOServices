#include "ICANService.h"

#ifndef MOCKCANSERVICE_H
#define MOCKCANSERVICE_H
namespace EmbeddedIOServices
{
	class MockCANService : public ICANService
	{
	public:
		MOCK_METHOD(
			void,
			Send,
			(const CANIdentifier_t identifier,
			 const CANData_t data,
			 const uint8_t dataLength,
			 can_send_completion_callback_t completion),
			(override));
	};
}
#endif
