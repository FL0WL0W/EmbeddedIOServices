#include "IDigitalService.h"
#include "ATTiny427_ExpanderService.h"
#include <forward_list>

#ifndef DIGITALSERVICE_ATTINY427EXPANDER_H
#define DIGITALSERVICE_ATTINY427EXPANDER_H

#define OFFSET_VPORTA 		0x0000
#define ADDRESS_VPORTA_DIR 	(OFFSET_VPORTA + 0x00)
#define ADDRESS_VPORTA_OUT 	(OFFSET_VPORTA + 0x01)
#define ADDRESS_VPORTA_IN  	(OFFSET_VPORTA + 0x03)

#define OFFSET_VPORTB 		0x0004
#define ADDRESS_VPORTB_DIR 	(OFFSET_VPORTB + 0x00)
#define ADDRESS_VPORTB_OUT 	(OFFSET_VPORTB + 0x01)
#define ADDRESS_VPORTB_IN  	(OFFSET_VPORTB + 0x03)

#define OFFSET_VPORTC 		0x0008
#define ADDRESS_VPORTC_DIR 	(OFFSET_VPORTC + 0x00)
#define ADDRESS_VPORTC_OUT 	(OFFSET_VPORTC + 0x01)
#define ADDRESS_VPORTC_IN  	(OFFSET_VPORTC + 0x03)

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
		DigitalPin_ATTiny427Expander DigitalPin;
		callback_t CallBack;

		DigitalInterrupt_ATTiny427Expander(DigitalPin_ATTiny427Expander digitalPin, callback_t callBack) : DigitalPin(digitalPin), CallBack(callBack) { }
	};

	typedef std::forward_list<DigitalInterrupt_ATTiny427Expander> DigitalInterruptList_ATTiny427Expander;

	class DigitalService_ATTiny427Expander : public IDigitalService
	{
	protected:
		const ATTiny427_ExpanderComm _comm;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _VPORTA_DIR;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _VPORTB_DIR;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _VPORTC_DIR;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _VPORTA_OUT;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _VPORTB_OUT;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _VPORTC_OUT;
		ATTiny427_ExpanderService::ATTiny427_ExpanderPoller * const _PORTA_Poller;
		ATTiny427_ExpanderService::ATTiny427_ExpanderPoller * const _PORTB_Poller;
		ATTiny427_ExpanderService::ATTiny427_ExpanderPoller * const _PORTC_Poller;
		DigitalInterruptList_ATTiny427Expander _PORTAInterruptList;
		DigitalInterruptList_ATTiny427Expander _PORTBInterruptList;
		DigitalInterruptList_ATTiny427Expander _PORTCInterruptList;
		
		uint8_t _PORTA_IN = 0b00000000;
		uint8_t _PORTB_IN = 0b00000000;
		uint8_t _PORTC_IN = 0b00000000;
	public:
		DigitalService_ATTiny427Expander(ATTiny427_ExpanderService *aTTiny427ExpanderService);
		~DigitalService_ATTiny427Expander();
		void InitPin(digitalpin_t pin, PinDirection direction);
		bool ReadPin(digitalpin_t pin);
		void WritePin(digitalpin_t pin, bool value);
		void AttachInterrupt(digitalpin_t pin, callback_t callBack);
		void DetachInterrupt(digitalpin_t pin);

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
