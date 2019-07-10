#include "IOServices/FloatInputService/FloatInputService_AnalogPin.h"

using namespace HardwareAbstraction;

#ifdef FLOATINPUTSERVICE_ANALOGPIN_H
namespace IOServices
{
	FloatInputService_AnalogPin::FloatInputService_AnalogPin(IAnalogService *analogService, const uint16_t pin)
	{
		_analogService = analogService;
		_pin = pin;

		_analogService->InitPin(_pin);
	}

	void FloatInputService_AnalogPin::ReadValue()
	{
		Value = _analogService->ReadPin(_pin);
	}
}
#endif
