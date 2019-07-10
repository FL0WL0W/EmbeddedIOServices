#include "HardwareAbstraction/HardwareAbstractionCollection.h"
#include "Service/ServiceLocator.h"

#if !defined(FLOATVARIABLESERVICE_INPUT_H)
#define FLOATVARIABLESERVICE_INPUT_H
namespace VariableBus
{
	class FloatVariableService_Input
	{
	public:		
		static void BuildFloatVariableService_Input(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut);
	};
}
#endif
