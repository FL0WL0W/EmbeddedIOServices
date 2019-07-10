#include "VariableBus/BooleanVariableService/BooleanVariableService_Static.h"
#include "Service/HardwareAbstractionServiceBuilder.h"
#include "Service/VariableBusServiceBuilderRegister.h"
#include "Service/ServiceBuilder.h"

using namespace HardwareAbstraction;
using namespace Service;

#ifdef BOOLEANVARIABLESERVICE_STATIC_H
namespace VariableBus
{
	BooleanVariableService_Static::BooleanVariableService_Static(const bool &staticValue)
	{
		_staticValue = staticValue;
		Value = _staticValue;
	}
	
	void BooleanVariableService_Static::ReadValue()
	{
		Value = _staticValue;
	}

	void BooleanVariableService_Static::BuildBooleanVariableService_Static(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
	{
        const uint32_t variableId = ServiceBuilder::CastAndOffset<uint16_t>(config, sizeOut);
		const bool staticValue = static_cast<bool>(ServiceBuilder::CastAndOffset<uint8_t>(config, sizeOut));

		BooleanVariableService_Static *booleanVariableService = new BooleanVariableService_Static(staticValue);
        serviceLocator->LocateAndCast<CallBackGroup>(TICK_CALL_BACK_GROUP)->Add(new CallBack<BooleanVariableService_Static>(booleanVariableService, &BooleanVariableService_Static::ReadValue));

        serviceLocator->Register(VARIABLE_BUS, variableId, &booleanVariableService->Value);
	}
}
#endif
