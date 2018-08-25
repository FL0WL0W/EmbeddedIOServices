#include "IOServices/StepperOutputService/IStepperOutputService.h"

#ifdef ISTEPPEROUTPUTSERVICE_H
namespace IOServices
{
	IStepperOutputService* IStepperOutputService::CreateStepperOutputService(const HardwareAbstraction::HardwareAbstractionCollection *hardwareAbstractionCollection, void *config, unsigned int *size)
	{
		unsigned char stepperServiceId = *((unsigned char*)config);
		config = ((unsigned char *)config + 1);
		*size = sizeof(unsigned char);
		
		IStepperOutputService *outputService = 0;
		
		switch (stepperServiceId)
		{
		}
		
		return outputService;
	}
}
#endif