#include "Operations/IOperation.h"
#include "Service/IService.h"
#include "Service/ServiceLocator.h"
#include "Service/HardwareAbstractionServiceBuilder.h"
#include "Packed.h"
#include "Interpolation.h"
#include "VariableType.h"

#ifndef OPERATION_DIGITALPINWRITE_H
#define OPERATION_DIGITALPINWRITE_H
namespace Operations
{
	class Operation_DigitalPinWrite : public IOperation<void, bool>
	{
	protected:
		HardwareAbstraction::IDigitalService *_digitalService;
		uint16_t _pin;
		bool _normalOn;
		bool _highZ;
	public:		
        Operation_DigitalPinWrite(HardwareAbstraction::IDigitalService *digitalService, uint16_t pin, const bool normalOn, const bool highZ);

		void Execute(bool x) override;

		static IOperationBase *Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut);
		ISERVICE_REGISTERFACTORY_H
	};
}
#endif