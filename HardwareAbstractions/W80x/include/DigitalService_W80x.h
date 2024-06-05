#include "IDigitalService.h"
#include "wm_regs.h"
#include <forward_list>

#ifndef DIGITALSERVICE_W80X_H
#define DIGITALSERVICE_W80X_H
namespace EmbeddedIOServices
{
	typedef uint32_t GPIOPin_W80x;
	
	struct DigitalInterrupt_W80x
	{
		GPIOPin_W80x GPIOPin;
		callback_t CallBack;

		DigitalInterrupt_W80x(GPIOPin_W80x gpioPin, callback_t callBack) : GPIOPin(gpioPin), CallBack(callBack) { }
	};

	typedef std::forward_list<DigitalInterrupt_W80x> DigitalInterruptList_W80x;

	class DigitalService_W80x : public IDigitalService
	{
	public:
		void InitPin(digitalpin_t pin, PinDirection direction);
		__attribute__((section(".interrupt"))) bool ReadPin(digitalpin_t pin);
		__attribute__((section(".interrupt"))) void WritePin(digitalpin_t pin, bool value);
		void AttachInterrupt(digitalpin_t pin, callback_t callBack);
		void DetachInterrupt(digitalpin_t pin);

		static DigitalInterruptList_W80x GPIOAInterruptList;
		static DigitalInterruptList_W80x GPIOBInterruptList;
		__attribute__((section(".interrupt"))) static GPIOPin_W80x PinToGPIOPin(digitalpin_t pin);
	};
}
#endif
