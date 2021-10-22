#include "stdint.h"

#ifndef IANALOGSERVICE_H
#define IANALOGSERVICE_H
namespace EmbeddedIOServices
{
	typedef uint16_t analogpin_t;

	class IAnalogService
	{
	public:
		virtual void InitPin(analogpin_t pin) = 0; //pin 0 should be for "null"
		virtual float ReadPin(analogpin_t pin) = 0; //pin 0 should be for "null". returns voltage
	};
}
#endif
