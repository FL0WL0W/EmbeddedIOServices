#include "Variables/Variable_StaticScalar.h"
#include "Service/HardwareAbstractionServiceBuilder.h"
#include "Service/EmbeddedVariablesRegister.h"
#include "Service/IService.h"

using namespace HardwareAbstraction;
using namespace Service;

#ifdef VARIABLE_STATICSCALAR_H
namespace Variables
{
	Variable_StaticScalar::Variable_StaticScalar(ScalarVariable *variable, const ScalarVariable &staticValue)
	{
		_staticValue = staticValue;
		_variable = variable;
		*_variable = _staticValue;
	}
	
	void Variable_StaticScalar::TranslateValue()
	{
		*_variable = _staticValue;
	}

	IVariable *Variable_StaticScalar::Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
	{
		ScalarVariable *variable = GetOrCreateVariable<ScalarVariable>(serviceLocator, Service::IService::CastAndOffset<uint16_t>(config, sizeOut));
		const ScalarVariable staticValue = IService::CastAndOffset<ScalarVariable>(config, sizeOut);
		
		Variable_StaticScalar *variableService = new Variable_StaticScalar(variable, staticValue);
        serviceLocator->LocateAndCast<CallBackGroup>(MAIN_LOOP_CALL_BACK_GROUP)->Add(new CallBack<Variable_StaticScalar>(variableService, &Variable_StaticScalar::TranslateValue));

		return variableService;
	}
	ISERVICE_REGISTERFACTORY_CPP(Variable_StaticScalar, 13)
}
#endif
