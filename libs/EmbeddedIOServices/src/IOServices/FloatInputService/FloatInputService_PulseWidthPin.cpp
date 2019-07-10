#include "IOServices/FloatInputService/FloatInputService_PulseWidthPin.h"

using namespace HardwareAbstraction;

#ifdef FLOATINPUTSERVICE_PULSEWIDTHPIN_H
namespace IOServices
{
	FloatInputService_PulseWidthPin::FloatInputService_PulseWidthPin(IPwmService *pwmService, const uint16_t pin, const uint16_t minFrequency)
	{
		_pwmService = pwmService;
		_pin = pin;
		_minFrequency = minFrequency;

		_pwmService->InitPin(_pin, In, _minFrequency);
	}

	void FloatInputService_PulseWidthPin::ReadValue()
	{
		Value = _pwmService->ReadPin(_pin).PulseWidth;
	}
}
#endif
