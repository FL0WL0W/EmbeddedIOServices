#include "PinDirection.h"
#include "CallBack.h"
#include "stdint.h"

#ifndef IDIGITALSERVICE_H
#define IDIGITALSERVICE_H
namespace EmbeddedIOServices
{
	typedef uint16_t digitalpin_t;

	class IDigitalService
	{
	public:
		/**
		 * @brief This method initializes the specified pin as a digital input or output pin.
		 * @param pin the pin number to be initialized. Pin number 0xFFFF is considered a null pin
		 * @param direction the direction to initialize the pin as input or output
		 */
		virtual void InitPin(digitalpin_t pin, PinDirection direction) = 0;

		/**
		 * @brief This method reads the digital state of the specified pin.
		 * @param pin the pin number to be read. Pin number 0xFFFF is considered a null pin
		 * @return boolean value of the pin state
		 */
		virtual bool ReadPin(digitalpin_t pin) = 0;

		/**
		 * @brief This method writes the digital state to the specified pin.
		 * @param pin the pin number to be written. Pin number 0xFFFF is considered a null pin
		 * @param value boolean value of the pin state
		 */
		virtual void WritePin(digitalpin_t pin, bool value) = 0;

		/**
		 * @brief This method attaches a callback as an interrupt to the specified pin.
		 * There can only be one callback attached per pin
		 * @param pin the pin number to attach the interrupt to. Pin number 0xFFFF is considered a null pin
		 * @param callBack the function to be called whenever the pin changes state
		 */
		virtual void AttachInterrupt(digitalpin_t pin, callback_t callBack) = 0;

		/**
		 * @brief This method detaches the callback from the specified pin.
		 * @param pin the pin number to detach the interrupt to. Pin number 0xFFFF is considered a null pin
		 */
		virtual void DetachInterrupt(digitalpin_t pin) = 0;
	};
}
#endif
