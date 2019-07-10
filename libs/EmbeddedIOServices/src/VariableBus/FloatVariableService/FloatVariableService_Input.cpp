#include "VariableBus/FloatVariableService/FloatVariableService_Input.h"
#include "IOServices/FloatInputService/IFloatInputService.h"
#include "Service/HardwareAbstractionServiceBuilder.h"
#include "Service/VariableBusServiceBuilderRegister.h"
#include "Service/ServiceBuilder.h"

using namespace HardwareAbstraction;
using namespace IOServices;
using namespace Service;

#ifdef FLOATVARIABLESERVICE_INPUT_H
namespace VariableBus
{
	void FloatVariableService_Input::BuildFloatVariableService_Input(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
	{
        const uint32_t variableId = ServiceBuilder::CastAndOffset<uint16_t>(config, sizeOut);

		IFloatInputService *floatInputService = IFloatInputService::CreateFloatInputService(serviceLocator, config, sizeOut);
        serviceLocator->LocateAndCast<CallBackGroup>(TICK_CALL_BACK_GROUP)->Add(new CallBack<IFloatInputService>(floatInputService, &IFloatInputService::ReadValue));

        serviceLocator->Register(VARIABLE_BUS, variableId, &floatInputService->Value);
	}
}
#endif
