#include "IOServices/FloatOutputService/IFloatOutputService.h"
#include "IOServices/FloatOutputService/FloatOutputService_PwmPolynomial.h"
#include "IOServices/FloatOutputService/FloatOutputService_PwmInterpolatedTable.h"
#include "IOServices/FloatOutputService/FloatOutputService_StepperPolynomial.h"
#include "IOServices/FloatOutputService/FloatOutputService_StepperInterpolatedTable.h"

#ifdef IFLOATOUTPUTSERVICE_H
namespace IOServices
{
	IFloatOutputService* IFloatOutputService::CreateFloatOutputService(const HardwareAbstraction::HardwareAbstractionCollection *hardwareAbstractionCollection, void *config, unsigned int *sizeOut)
	{
		unsigned char outputServiceId = *((unsigned char*)config);
		config = ((unsigned char *)config + 1);
		*sizeOut = sizeof(unsigned char);
		
		IFloatOutputService *outputService = 0;
		
		switch (outputServiceId)
		{
#ifdef FLOATOUTPUTSERVICE_PWMPOLYNOMIAL_H
		case 1:
			{
				FloatOutputService_PwmPolynomialConfig<3> *pwmConfig = FloatOutputService_PwmPolynomialConfig<3>::Cast((unsigned char*)config);
				*sizeOut += pwmConfig->Size();
				outputService = new FloatOutputService_PwmPolynomial<3>(hardwareAbstractionCollection, pwmConfig);
				break;
			}
#endif
			
#ifdef FLOATOUTPUTSERVICE_STEPPERPOLYNOMIAL_H
		case 2:
			{
				FloatOutputService_StepperPolynomialConfig<3> *stepperConfig = FloatOutputService_StepperPolynomialConfig<3>::Cast((unsigned char*)config);
				*sizeOut += stepperConfig->Size();
				outputService = new FloatOutputService_StepperPolynomial<3>(hardwareAbstractionCollection, stepperConfig);
				break;
			}
#endif

#ifdef FLOATOUTPUTSERVICE_PWMINTERPOLATEDTABLE_H
		case 3:
			{
				FloatOutputService_PwmInterpolatedTableConfig *pwmConfig = FloatOutputService_PwmInterpolatedTableConfig::Cast((unsigned char*)config);
				*sizeOut += pwmConfig->Size();
				outputService = new FloatOutputService_PwmInterpolatedTable(hardwareAbstractionCollection, pwmConfig);
				break;
			}
#endif

#ifdef FLOATOUTPUTSERVICE_STEPPERINTERPOLATEDTABLE_H
		case 4:
			{
				FloatOutputService_StepperInterpolatedTableConfig *stepperConfig = FloatOutputService_StepperInterpolatedTableConfig::Cast((unsigned char*)config);
				*sizeOut += stepperConfig->Size();
				outputService = new FloatOutputService_StepperInterpolatedTable(hardwareAbstractionCollection, stepperConfig);
				break;
			}
#endif
		}
		
		return outputService;
	}
}
#endif