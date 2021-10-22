#include "stdint.h"

#ifndef IANALOGSERVICE_H
#define IANALOGSERVICE_H
typedef uint16_t analogpin_t;

namespace EmbeddedIOServices
{
	class IAnalogService
	{
	public:
		virtual void InitPin(analogpin_t pin) = 0; //pin 0 should be for "null"
		virtual float ReadPin(analogpin_t pin) = 0; //pin 0 should be for "null". returns voltage
	};
}
#endif
