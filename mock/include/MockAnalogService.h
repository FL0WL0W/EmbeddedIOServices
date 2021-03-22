#include "IAnalogService.h"

#ifndef MOCKANALOGSERVICE_H
#define MOCKANALOGSERVICE_H
namespace EmbeddedIOServices
{
	class MockAnalogService : public IAnalogService
	{
	public:
		MOCK_METHOD1(InitPin, void(uint16_t));
		MOCK_METHOD1(ReadPin, float(uint16_t));
	};
}
#endif