#include "IOServices/StepperOutputService/IStepperOutputService.h"
#include "IOServices/StepperOutputService/StepperOutputService_StepDirectionControl.h"

#ifdef ISTEPPEROUTPUTSERVICE_H
namespace IOServices
{
	IStepperOutputService* IStepperOutputService::CreateStepperOutputService(const HardwareAbstraction::HardwareAbstractionCollection *hardwareAbstractionCollection, void *config, unsigned int *sizeOut)
	{
		unsigned char stepperServiceId = *((unsigned char*)config);
		config = ((unsigned char *)config + 1);
		*sizeOut = sizeof(unsigned char);
		
		IStepperOutputService *outputService = 0;
		
		switch (stepperServiceId)
		{
#ifdef STEPPEROUTPUTSERVICE_STEPDIRECTIONCONTROL_H
		case 1:
			StepperOutputService_StepDirectionControlConfig *stepperConfig = StepperOutputService_StepDirectionControlConfig::Cast(config);
			*sizeOut += stepperConfig->Size();

			config = (void*)((unsigned char *)config + stepperConfig->Size());
			unsigned int subSize = 0;
			IBooleanOutputService *stepBooleanOutputService = IBooleanOutputService::CreateBooleanOutputService(hardwareAbstractionCollection, config, &subSize);
			*sizeOut += subSize;

			config = (void*)((unsigned char *)config + subSize);
			IBooleanOutputService *directionBooleanOutputService = IBooleanOutputService::CreateBooleanOutputService(hardwareAbstractionCollection, config, &subSize);
			*sizeOut += subSize;

			outputService = new StepperOutputService_StepDirectionControl(hardwareAbstractionCollection, stepperConfig, stepBooleanOutputService, directionBooleanOutputService);
			break;
#endif
			
#ifdef STEPPEROUTPUTSERVICE_COILCONTROL_H
		case 2:
			outputService = new StepperOutputService_CoilControl(hardwareAbstractionCollection, config);
			break;
#endif
		}
		
		return outputService;
	}
}
#endif