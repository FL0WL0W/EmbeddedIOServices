#include "VariableBus/FloatVariableService/FloatVariableService_Static.h"
#include "Service/HardwareAbstractionServiceBuilder.h"
#include "Service/VariableBusServiceBuilderRegister.h"
#include "Service/ServiceBuilder.h"

using namespace HardwareAbstraction;
using namespace Service;

#ifdef FLOATVARIABLESERVICE_STATIC_H
namespace VariableBus
{
	FloatVariableService_Static::FloatVariableService_Static(const float &staticValue)
	{
		_staticValue = staticValue;
		Value = _staticValue;
	}
	
	void FloatVariableService_Static::ReadValue()
	{
		Value = _staticValue;
	}

	void FloatVariableService_Static::BuildFloatVariableService_Static(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
	{
        const uint32_t variableId = ServiceBuilder::CastAndOffset<uint16_t>(config, sizeOut);
		const float staticValue = ServiceBuilder::CastAndOffset<float>(config, sizeOut);

		FloatVariableService_Static *floatVariableService = new FloatVariableService_Static(staticValue);
        serviceLocator->LocateAndCast<CallBackGroup>(TICK_CALL_BACK_GROUP)->Add(new CallBack<FloatVariableService_Static>(floatVariableService, &FloatVariableService_Static::ReadValue));

        serviceLocator->Register(VARIABLE_BUS, variableId, &floatVariableService->Value);
	}
}
#endif
