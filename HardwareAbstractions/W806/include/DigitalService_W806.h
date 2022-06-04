#include "IDigitalService.h"
#include "wm_regs.h"
#include <forward_list>

#ifndef DIGITALSERVICE_W806_H
#define DIGITALSERVICE_W806_H
namespace EmbeddedIOServices
{
	struct DigitalInterrupt 
	{
		uint32_t GPIOPin;
		callback_t CallBack;

		DigitalInterrupt(uint32_t gpioPin, callback_t callBack) : GPIOPin(gpioPin), CallBack(callBack) { }
	};

	typedef std::forward_list<DigitalInterrupt> DigitalInterruptList;

	class DigitalService_W806 : public IDigitalService
	{
	public:
		void InitPin(digitalpin_t pin, PinDirection direction);
		bool ReadPin(digitalpin_t pin);
		void WritePin(digitalpin_t pin, bool value);
		void AttachInterrupt(digitalpin_t pin, callback_t callBack);
		void DetachInterrupt(digitalpin_t pin);

		static DigitalInterruptList GPIOAInterruptList;
		static DigitalInterruptList GPIOBInterruptList;
		static uint32_t PinToGPIOPin(digitalpin_t pin);
	};
}
#endif
