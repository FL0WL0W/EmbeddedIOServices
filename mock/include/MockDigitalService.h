#include "IDigitalService.h"

#ifndef MOCKDIGITALSERVICE_H
#define MOCKDIGITALSERVICE_H
namespace EmbeddedIOServices
{
	class MockDigitalService : public IDigitalService
	{
	public:
		MOCK_METHOD2(InitPin, void(uint16_t, PinDirection));
		MOCK_METHOD1(ReadPin, bool(uint16_t));
		MOCK_METHOD2(WritePin, void(uint16_t, bool));
		MOCK_METHOD2(AttachInterrupt, void(uint16_t, std::function<void()>));
		MOCK_METHOD1(DetachInterrupt, void(uint16_t));
	};
}
#endif