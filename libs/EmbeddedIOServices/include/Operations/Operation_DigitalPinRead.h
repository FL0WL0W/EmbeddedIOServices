#include "Operations/IOperation.h"
#include "Service/IService.h"
#include "Service/ServiceLocator.h"
#include "Service/HardwareAbstractionServiceBuilder.h"
#include "Packed.h"
#include "Interpolation.h"
#include "VariableType.h"

#ifndef OPERATION_DIGITALPINREAD_H
#define OPERATION_DIGITALPINREAD_H
namespace Operations
{
	class Operation_DigitalPinRead : public IOperation<bool>
	{
	protected:
		HardwareAbstraction::IDigitalService *_digitalService;
		uint16_t _pin;
		bool _inverted;
	public:		
        Operation_DigitalPinRead(HardwareAbstraction::IDigitalService *digitalService, uint16_t pin, bool inverted);

		bool Execute() override;

		static IOperationBase *Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut);
		ISERVICE_REGISTERFACTORY_H
	};
}
#endif