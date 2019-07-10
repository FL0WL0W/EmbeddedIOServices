#include "IOServices/FloatInputService/FloatInputService_FaultDetectionWrapper.h"

using namespace HardwareAbstraction;

#ifdef FLOATINPUTSERVICE_FAULTDETECTIONWRAPPER_H
namespace IOServices
{
	FloatInputService_FaultDetectionWrapper::FloatInputService_FaultDetectionWrapper(const FloatInputService_FaultDetectionWrapperConfig *config, IFloatInputService *child)
	{
		_config = config;
		_child = child;
	}

	void FloatInputService_FaultDetectionWrapper::ReadValue()
	{
		_child->ReadValue();
		if(_child->Value < _config->MinValue || _child->Value > _config->MaxValue)
		{
			Value = _config->DefaultValue;
		}
		else
		{
			Value = _child->Value;
		}
	}
}
#endif
