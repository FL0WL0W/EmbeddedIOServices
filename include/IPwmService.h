#include "stdint.h"
#include "PinDirection.h"

#ifndef IPWMSERVICE_H
#define IPWMSERVICE_H
namespace EmbeddedIOServices
{
	typedef uint16_t pwmpin_t;

	/**
	 * @brief Structure that contains the details of the PWM (Period, PulseWdith)
	 */
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
		/**
		 * @brief This method initializes the specified pin as a PWM input or output pin.
		 * @param pin the pin number to be initialized. Pin number 0xFFFF is considered a null pin
		 * @param direction the direction to initialize the pin as input or output
		 * @param minFreqeuncy the minimum frequency the pwm pin will need to operate at
		 */
		virtual void InitPin(pwmpin_t pin, PinDirection direction, uint16_t minFreqeuncy) = 0;

		/**
		 * @brief This method reads the PWM state of the specified pin.
		 * @param pin the pin number to be read. Pin number 0xFFFF is considered a null pin
		 * @return PwmValue value of the pin state
		 */
		virtual PwmValue ReadPin(pwmpin_t pin) = 0;

		/**
		 * @brief This method writes the pwm state to the specified pin.
		 * @param pin the pin number to be written. Pin number 0xFFFF is considered a null pin
		 * @param value PwmValue value of the pin state
		 */
		virtual void WritePin(pwmpin_t pin, PwmValue value) = 0;
	};
}
#endif
