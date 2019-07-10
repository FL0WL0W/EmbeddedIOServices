#include "IOServices/FloatInputService/FloatInputService_FrequencyPin.h"

using namespace HardwareAbstraction;

#ifdef FLOATINPUTSERVICE_FREQUENCYPIN_H
namespace IOServices
{
	FloatInputService_FrequencyPin::FloatInputService_FrequencyPin(IPwmService *pwmService, const uint16_t pin, const uint16_t minFrequency)
	{
		_pwmService = pwmService;
		_pin = pin;
		_minFrequency = minFrequency;

		_pwmService->InitPin(_pin, In, _minFrequency);
	}

	void FloatInputService_FrequencyPin::ReadValue()
	{
		Value = 1 / _pwmService->ReadPin(_pin).Period;
	}
}
#endif
