#include "stdint.h"

#ifndef IANALOGSERVICE_H
#define IANALOGSERVICE_H
namespace EmbeddedIOServices
{
	typedef uint16_t analogpin_t;

	class IAnalogService
	{
	public:
		/**
		 * @brief This method initializes the specified pin as an analog input pin.
		 * @param pin the pin number to be initialized. Pin number 0xFFFF is considered a null pin
		 */
		virtual void InitPin(analogpin_t pin) = 0;
		
		/**
		 * @brief This method reads the voltage of the specified pin.
		 * @param pin the pin number to be read. Pin number 0xFFFF is considered a null pin
		 * @return float value of the voltage
		 */
		virtual float ReadPin(analogpin_t pin) = 0;
	};
}
#endif
