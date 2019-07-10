#include "HardwareAbstraction/HardwareAbstractionCollection.h"
#include "Service/ServiceLocator.h"

#if !defined(BOOLEANVARIABLESERVICE_INPUT_H)
#define BOOLEANVARIABLESERVICE_INPUT_H
namespace VariableBus
{
	class BooleanVariableService_Input
	{
	public:		
		static void BuildBooleanVariableService_Input(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut);
	};
}
#endif
