#include "HardwareAbstraction/HardwareAbstractionCollection.h"
#include "Service/ServiceLocator.h"
#include "Variables/IVariable.h"

#if !defined(VARIABLE_STATICFLOAT_H)
#define VARIABLE_STATICFLOAT_H
namespace Variables
{
	//todo: move this to operations as a template
	class Variable_StaticFloat : public IVariable
	{
	protected:
        float _staticValue = 0;
	public:		
		float Value = 0;
        Variable_StaticFloat(const float &staticValue);
		void TranslateValue() override;

		static IVariable *Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut);
		ISERVICE_REGISTERFACTORY_H
	};
}
#endif
