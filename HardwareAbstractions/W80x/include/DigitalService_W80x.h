#include "IDigitalService.h"
#include "wm_regs.h"
#include <forward_list>

#ifndef DIGITALSERVICE_W80X_H
#define DIGITALSERVICE_W80X_H
namespace EmbeddedIOServices
{
	struct DigitalInterrupt 
	{
		uint32_t GPIOPin;
		callback_t CallBack;

		DigitalInterrupt(uint32_t gpioPin, callback_t callBack) : GPIOPin(gpioPin), CallBack(callBack) { }
	};

	typedef std::forward_list<DigitalInterrupt> DigitalInterruptList;

	class DigitalService_W80x : public IDigitalService
	{
	public:
		void InitPin(digitalpin_t pin, PinDirection direction);
		__attribute__((section(".interrupt"))) bool ReadPin(digitalpin_t pin);
		__attribute__((section(".interrupt"))) void WritePin(digitalpin_t pin, bool value);
		void AttachInterrupt(digitalpin_t pin, callback_t callBack);
		void DetachInterrupt(digitalpin_t pin);

		static DigitalInterruptList GPIOAInterruptList;
		static DigitalInterruptList GPIOBInterruptList;
		__attribute__((section(".interrupt"))) static uint32_t PinToGPIOPin(digitalpin_t pin);
	};
}
#endif
