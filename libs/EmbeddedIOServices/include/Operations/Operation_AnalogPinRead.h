#include "Operations/IOperation.h"
#include "Service/IService.h"
#include "Service/ServiceLocator.h"
#include "Service/HardwareAbstractionServiceBuilder.h"
#include "Packed.h"
#include "Interpolation.h"
#include "VariableType.h"

#ifndef OPERATION_ANALOGPINREAD_H
#define OPERATION_ANALOGPINREAD_H
namespace Operations
{
	class Operation_AnalogPinRead : public IOperation<float>
	{
	protected:
		HardwareAbstraction::IAnalogService *_analogService;
		uint16_t _pin;
	public:		
        Operation_AnalogPinRead( HardwareAbstraction::IAnalogService *analogService, const uint16_t pin);

		float Execute() override;

		static IOperationBase *Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut);
		ISERVICE_REGISTERFACTORY_H
	};
}
#endif