#include "IOServices/BooleanInputService/IBooleanInputService.h"
#include "IOServices/BooleanOutputService/IBooleanOutputService.h"
#include "IOServices/ButtonService/IButtonService.h"
#include "IOServices/FloatInputService/IFloatInputService.h"
#include "IOServices/FloatOutputService/IFloatOutputService.h"
#include "IOServices/StepperOutputService/IStepperOutputService.h"
#include "Service/ServiceBuilder.h"

using namespace IOServices;

#ifndef IOSERVICESSERVICEBUILDEREGISTER_H
#define IOSERVICESSERVICEBUILDEREGISTER_H

namespace Service
{
	class IOServicesServiceBuilderRegister
	{
	public:
		static void Register(ServiceBuilder *&serviceBuilder);
	};
}
#endif
