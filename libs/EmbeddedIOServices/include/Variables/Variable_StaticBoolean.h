#include "HardwareAbstraction/HardwareAbstractionCollection.h"
#include "Service/ServiceLocator.h"
#include "Variables/IVariable.h"

#if !defined(VARIABLE_STATICBOOLEAN_H)
#define VARIABLE_STATICBOOLEAN_H
namespace Variables
{
	class Variable_StaticBoolean : public IVariable
	{
	protected:
        bool _staticValue = false;
	public:		
		bool Value = false;
        Variable_StaticBoolean(const bool &staticValue);
		virtual void TranslateValue();

		static IVariable *Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut);
		ISERVICE_REGISTERFACTORY_H
	};
}
#endif
