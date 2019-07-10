#include "VariableBus/BooleanVariableService/BooleanVariableService_Input.h"
#include "IOServices/BooleanInputService/IBooleanInputService.h"
#include "Service/HardwareAbstractionServiceBuilder.h"
#include "Service/VariableBusServiceBuilderRegister.h"
#include "Service/ServiceBuilder.h"

using namespace HardwareAbstraction;
using namespace IOServices;
using namespace Service;

#ifdef BOOLEANVARIABLESERVICE_INPUT_H
namespace VariableBus
{
	void BooleanVariableService_Input::BuildBooleanVariableService_Input(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
	{
        const uint32_t variableId = ServiceBuilder::CastAndOffset<uint16_t>(config, sizeOut);

		IBooleanInputService *booleanInputService = IBooleanInputService::CreateBooleanInputService(serviceLocator, config, sizeOut);
        serviceLocator->LocateAndCast<CallBackGroup>(TICK_CALL_BACK_GROUP)->Add(new CallBack<IBooleanInputService>(booleanInputService, &IBooleanInputService::ReadValue));

        serviceLocator->Register(VARIABLE_BUS, variableId, &booleanInputService->Value);
	}
}
#endif
