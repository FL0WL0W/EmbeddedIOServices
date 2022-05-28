#include "IDigitalService.h"

#ifndef DIGITALSERVICE_H
#define DIGITALSERVICE_H
namespace EmbeddedIOServices
{
	class DigitalService : public IDigitalService
	{
	public:
		void InitPin(digitalpin_t pin, PinDirection direction);
		bool ReadPin(digitalpin_t pin);
		void WritePin(digitalpin_t pin, bool value);
		void AttachInterrupt(digitalpin_t pin, callback_t callBack);
		void DetachInterrupt(digitalpin_t pin);
	};
}
#endif
