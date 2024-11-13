#include "IDigitalService.h"
#include "ATTiny427ExpanderUpdateService.h"
#include <forward_list>

#ifndef DIGITALSERVICE_ATTINY427EXPANDER_H
#define DIGITALSERVICE_ATTINY427EXPANDER_H
namespace EmbeddedIOServices
{
	enum DigitalPort_ATTiny427Expander : uint8_t
	{
		PORTA = 0,
		PORTB = 1,
		PORTC = 2
	};

	typedef uint8_t DigitalPin_ATTiny427Expander;
	
	struct DigitalInterrupt_ATTiny427Expander
	{
		DigitalPort_ATTiny427Expander DigitalPort;
		DigitalPin_ATTiny427Expander DigitalPin;
		callback_t CallBack;

		DigitalInterrupt_ATTiny427Expander(DigitalPort_ATTiny427Expander digitalPort, DigitalPin_ATTiny427Expander digitalPin, callback_t callBack) : DigitalPort(digitalPort), DigitalPin(digitalPin), CallBack(callBack) { }
	};

	typedef std::forward_list<DigitalInterrupt_ATTiny427Expander> DigitalInterruptList_ATTiny427Expander;

	class DigitalService_ATTiny427Expander : public IDigitalService
	{
	protected:
		DigitalInterruptList_ATTiny427Expander _interruptList;
		ATTiny427Expander_Registers *_registers;

		uint8_t _previousPORTA_IN = 0b00000000;
		uint8_t _previousPORTB_IN = 0b00000000;
		uint8_t _previousPORTC_IN = 0b00000000;
	public:
		DigitalService_ATTiny427Expander(ATTiny427Expander_Registers *registers);
		void InitPin(digitalpin_t pin, PinDirection direction);
		bool ReadPin(digitalpin_t pin);
		void WritePin(digitalpin_t pin, bool value);
		void AttachInterrupt(digitalpin_t pin, callback_t callBack);
		void DetachInterrupt(digitalpin_t pin);
		bool InitPassthrough(digitalpin_t pinIn, digitalpin_t pinOut, bool inverted);
		void DeinitPassthrough(digitalpin_t pinOut);
		void Update();

		static inline DigitalPin_ATTiny427Expander PinToDigitalPin(digitalpin_t pin)
		{
			return 1 << (pin % 8);
		}
		static inline DigitalPort_ATTiny427Expander PinToDigitalPort(digitalpin_t pin)
		{
			return static_cast<DigitalPort_ATTiny427Expander>(pin>>3);
		}
	};
}
#endif
