#include "Variables/Variable_StaticFloat.h"
#include "Service/HardwareAbstractionServiceBuilder.h"
#include "Service/EmbeddedVariablesRegister.h"
#include "Service/IService.h"

using namespace HardwareAbstraction;
using namespace Service;

#ifdef VARIABLE_STATICFLOAT_H
namespace Variables
{
	Variable_StaticFloat::Variable_StaticFloat(const float &staticValue)
	{
		_staticValue = staticValue;
		Value = _staticValue;
	}
	
	void Variable_StaticFloat::TranslateValue()
	{
		Value = _staticValue;
	}

	IVariable *Variable_StaticFloat::Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
	{
        const uint32_t variableId = IService::CastAndOffset<uint16_t>(config, sizeOut);
		const float staticValue = IService::CastAndOffset<float>(config, sizeOut);

		Variable_StaticFloat *variableService = new Variable_StaticFloat(staticValue);
        serviceLocator->LocateAndCast<CallBackGroup>(TICK_CALL_BACK_GROUP)->Add(new CallBack<Variable_StaticFloat>(variableService, &Variable_StaticFloat::TranslateValue));

        serviceLocator->Register(BUILDER_VARIABLE, variableId, &variableService->Value);

		return variableService;
	}
	ISERVICE_REGISTERFACTORY_CPP(Variable_StaticFloat, 14)
}
#endif
