#include "IOServices/StepperOutputService/IStepperOutputService.h"

#ifndef MOCKSTEPPERSERVICE_H
#define MOCKSTEPPERSERVICE_H
namespace HardwareAbstraction
{
	class MockStepperOutputService : public IStepperOutputService
	{
	public:
		MOCK_METHOD1(Step, void(int));
	};
}
#endif