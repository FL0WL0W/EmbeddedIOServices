#include "HardwareAbstraction/HardwareAbstractionCollection.h"
#include "Service/ServiceLocator.h"
#include "Variables/IVariable.h"
#include "ScalarVariable.h"

#if !defined(VARIABLE_STATICSCALAR_H)
#define VARIABLE_STATICSCALAR_H
namespace Variables
{
	class Variable_StaticScalar : public IVariable
	{
	protected:
		ScalarVariable *_variable;
        ScalarVariable _staticValue;
	public:		
        Variable_StaticScalar(ScalarVariable *variable, const ScalarVariable &staticValue);
		void TranslateValue() override;

		static IVariable *Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut);
		ISERVICE_REGISTERFACTORY_H
	};
}
#endif
