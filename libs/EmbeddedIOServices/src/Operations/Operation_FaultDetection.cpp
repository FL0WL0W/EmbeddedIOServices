#include "Variables/Variable_Operation.h"
#include "Operations/Operation_FaultDetection.h"

#ifdef OPERATION_FAULTDETECTION_H
namespace Operations
{
	Operation_FaultDetection::Operation_FaultDetection(const Operation_FaultDetectionConfig * const &config)
	{
		_config = config;
	}

	float Operation_FaultDetection::Execute(float x)
	{
		if(x < _config->MinValue || x > _config->MaxValue)
			return _config->DefaultValue;
		return x;
	}

	IOperationBase *Operation_FaultDetection::Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
	{
		const Operation_FaultDetectionConfig *faultConfig = reinterpret_cast<const Operation_FaultDetectionConfig *>(config);
		sizeOut += faultConfig->Size();
		return new Operation_FaultDetection(faultConfig);
	}
	
	IOPERATION_REGISTERFACTORY_CPP(Operation_FaultDetection, 9, float, float)
}
#endif