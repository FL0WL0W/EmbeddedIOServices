#include "stdint.h"

#ifndef IPWMSERVICE_H
#define IPWMSERVICE_H
namespace EmbeddedIOServices
{
	typedef uint16_t pwmpin_t;

	struct PwmValue
	{
		float Period;
		float PulseWidth;

		bool operator==(const PwmValue& rhs) const
		{
			return Period == rhs.Period
				&& PulseWidth == rhs.PulseWidth;
		}
	};

	class IPwmService
	{
	public:
		virtual void InitPin(pwmpin_t pin, PinDirection direction, uint16_t minFreqeuncy) = 0;  //pin 0 should be for "null"
		virtual PwmValue ReadPin(pwmpin_t pin) = 0; //pin 0 should be for "null"
		virtual void WritePin(pwmpin_t pin, PwmValue value) = 0; //pin 0 should be for "null"
	};
}
#endif
