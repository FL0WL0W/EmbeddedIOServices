#include "PinDirection.h"
#include "stdint.h"
#include <functional>

#ifndef IDIGITALSERVICE_H
#define IDIGITALSERVICE_H
namespace EmbeddedIOServices
{
	class IDigitalService
	{
	public:
		virtual void InitPin(uint16_t pin, PinDirection direction) = 0; //pin 0 should be for "null"
		virtual bool ReadPin(uint16_t pin) = 0; //pin 0 should be for "null"
		virtual void WritePin(uint16_t pin, bool value) = 0; //pin 0 should be for "null"
		virtual void AttachInterrupt(uint16_t pin, std::function<void()>) = 0;
		virtual void DetachInterrupt(uint16_t pin) = 0;
	};
}
#endif
