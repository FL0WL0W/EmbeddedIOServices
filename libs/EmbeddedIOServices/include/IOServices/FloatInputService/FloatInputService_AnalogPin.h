#include "HardwareAbstraction/HardwareAbstractionCollection.h"
#include "IFloatInputService.h"
#include "math.h"
#include "Packed.h"
#include "Interpolation.h"
#include "stdint.h"

#if !defined(FLOATINPUTSERVICE_ANALOGPIN_H) && defined(IFLOATINPUTSERVICE_H) && defined(IANALOGSERVICE_H)
#define FLOATINPUTSERVICE_ANALOGPIN_H
namespace IOServices
{	
	class FloatInputService_AnalogPin : public IFloatInputService
	{
	protected:
		HardwareAbstraction::IAnalogService *_analogService;		
		uint16_t _pin;
	public:
		FloatInputService_AnalogPin(HardwareAbstraction::IAnalogService *analogService, const uint16_t pin);

		void ReadValue() override;
	};
}
#endif
