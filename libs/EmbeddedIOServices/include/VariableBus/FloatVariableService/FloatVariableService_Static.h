#include "HardwareAbstraction/HardwareAbstractionCollection.h"
#include "Service/ServiceLocator.h"

#if !defined(FLOATVARIABLESERVICE_STATIC_H)
#define FLOATVARIABLESERVICE_STATIC_H
namespace VariableBus
{
	class FloatVariableService_Static
	{
	protected:
        float _staticValue = 0;
	public:		
		float Value = 0;
        FloatVariableService_Static(const float &staticValue);
		virtual void ReadValue();

		static void BuildFloatVariableService_Static(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut);
	};
}
#endif
