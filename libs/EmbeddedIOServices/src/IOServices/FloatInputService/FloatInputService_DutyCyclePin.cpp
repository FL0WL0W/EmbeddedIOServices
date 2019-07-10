#include "IOServices/FloatInputService/FloatInputService_DutyCyclePin.h"

using namespace HardwareAbstraction;

#ifdef FLOATINPUTSERVICE_DUTYCYCLEPIN_H
namespace IOServices
{
	FloatInputService_DutyCyclePin::FloatInputService_DutyCyclePin(IPwmService *pwmService, const uint16_t pin, const uint16_t minFrequency)
	{
		_pwmService = pwmService;
		_pin = pin;
		_minFrequency = minFrequency;

		_pwmService->InitPin(_pin, In, _minFrequency);
	}

	void FloatInputService_DutyCyclePin::ReadValue()
	{
		const PwmValue value = _pwmService->ReadPin(_pin);
		Value = value.PulseWidth / value.Period;
	}
}
#endif
