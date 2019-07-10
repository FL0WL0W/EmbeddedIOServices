#include "HardwareAbstraction/HardwareAbstractionCollection.h"
#include "IFloatInputService.h"
#include "math.h"
#include "Packed.h"
#include "Interpolation.h"
#include "stdint.h"

#if !defined(FLOATINPUTSERVICE_DUTYCYCLEPIN_H) && defined(IFLOATINPUTSERVICE_H) && defined(IPWMSERVICE_H)
#define FLOATINPUTSERVICE_DUTYCYCLEPIN_H
namespace IOServices
{	
	class FloatInputService_DutyCyclePin : public IFloatInputService
	{
	protected:
		HardwareAbstraction::IPwmService *_pwmService;		
		uint16_t _pin;
		uint16_t _minFrequency;
	public:
		FloatInputService_DutyCyclePin(HardwareAbstraction::IPwmService *pwmService, const uint16_t pin, const uint16_t minFrequency);

		void ReadValue() override;
	};
}
#endif
