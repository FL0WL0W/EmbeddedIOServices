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
		virtual void InitPin(digitalpin_t pin, PinDirection direction) = 0; //pin 0 should be for "null"
		virtual bool ReadPin(digitalpin_t pin) = 0; //pin 0 should be for "null"
		virtual void WritePin(digitalpin_t pin, bool value) = 0; //pin 0 should be for "null"
		virtual void AttachInterrupt(digitalpin_t pin, callback_t) = 0;
		virtual void DetachInterrupt(digitalpin_t pin) = 0;
	};
}
#endif
