#include "Operations/IOperation.h"
#include "Service/IService.h"
#include "Service/ServiceLocator.h"
#include "Packed.h"
#include "Interpolation.h"
#include "VariableType.h"

#ifndef OPERATION_FAULTDETECTION_H
#define OPERATION_FAULTDETECTION_H
namespace Operations
{
	PACK(
	struct Operation_FaultDetectionConfig
	{
	public:
		constexpr const unsigned int Size() const
		{
			return sizeof(Operation_FaultDetectionConfig);
		}
		
		float MinValue;
		float MaxValue;
		float DefaultValue;
	});

	class Operation_FaultDetection : public IOperation<float, float>
	{
	protected:
		const Operation_FaultDetectionConfig *_config;
	public:		
        Operation_FaultDetection(const Operation_FaultDetectionConfig * const &config);

		float Execute(float x) override;

		static IOperationBase *Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut);
		ISERVICE_REGISTERFACTORY_H
	};
}
#endif