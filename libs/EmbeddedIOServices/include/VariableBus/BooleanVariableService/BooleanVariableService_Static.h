#include "HardwareAbstraction/HardwareAbstractionCollection.h"
#include "Service/ServiceLocator.h"

#if !defined(BOOLEANVARIABLESERVICE_STATIC_H)
#define BOOLEANVARIABLESERVICE_STATIC_H
namespace VariableBus
{
	class BooleanVariableService_Static
	{
	protected:
        bool _staticValue = false;
	public:		
		bool Value = false;
        BooleanVariableService_Static(const bool &staticValue);
		virtual void ReadValue();

		static void BuildBooleanVariableService_Static(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut);
	};
}
#endif
