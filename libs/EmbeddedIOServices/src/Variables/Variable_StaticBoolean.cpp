#include "Variables/Variable_StaticBoolean.h"
#include "Service/HardwareAbstractionServiceBuilder.h"
#include "Service/EmbeddedVariablesRegister.h"
#include "Service/IService.h"

using namespace HardwareAbstraction;
using namespace Service;

#ifdef VARIABLE_STATICBOOLEAN_H
namespace Variables
{
	Variable_StaticBoolean::Variable_StaticBoolean(const bool &staticValue)
	{
		_staticValue = staticValue;
		Value = _staticValue;
	}
	
	void Variable_StaticBoolean::TranslateValue()
	{
		Value = _staticValue;
	}

	IVariable *Variable_StaticBoolean::Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
	{
        const uint32_t variableId = IService::CastAndOffset<uint16_t>(config, sizeOut);
		const bool staticValue = static_cast<bool>(IService::CastAndOffset<uint8_t>(config, sizeOut));

		Variable_StaticBoolean *variableService = new Variable_StaticBoolean(staticValue);
        serviceLocator->LocateAndCast<CallBackGroup>(TICK_CALL_BACK_GROUP)->Add(new CallBack<Variable_StaticBoolean>(variableService, &Variable_StaticBoolean::TranslateValue));

        serviceLocator->Register(BUILDER_VARIABLE, variableId, &variableService->Value);

		return variableService;
	}
	ISERVICE_REGISTERFACTORY_CPP(Variable_StaticBoolean, 13)
}
#endif
