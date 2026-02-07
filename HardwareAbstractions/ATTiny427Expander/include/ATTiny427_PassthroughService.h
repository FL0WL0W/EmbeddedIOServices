#include "ATTiny427_EVSYSService.h"

#ifndef ATTINY427_PASSTHROUGHSERVICE_H
#define ATTINY427_PASSTHROUGHSERVICE_H

#define OFFSET_PORTA 		0x0400
#define ADDRESS_PORTA_PINCTRL(pin) 	(OFFSET_PORTA + 0x00 + (pin))

#define OFFSET_PORTB 		0x0420
#define ADDRESS_PORTB_PINCTRL(pin) 	(OFFSET_PORTB + 0x00 + (pin))

#define OFFSET_PORTC 		0x0440
#define ADDRESS_PORTC_PINCTRL(pin) 	(OFFSET_PORTC + 0x00 + (pin))

#define OFFSET_PORTMUX				0x05E0
#define ADDRESS_PORTMUX_EVSYSROUTEA	(OFFSET_PORTMUX + 0x00)
#define ADDRESS_PORTMUX_CCLROUTEA	(OFFSET_PORTMUX + 0x01)

#define OFFSET_AC0 				0x0680
#define ADDRESS_AC0_CTRLA		(OFFSET_AC0 + 0x00)
#define ADDRESS_AC0_MUXCTRLA	(OFFSET_AC0 + 0x02)	
#define ADDRESS_AC0_DACREF		(OFFSET_AC0 + 0x04)

#define OFFSET_CCL 			0x01C0
#define ADDRESS_CCL_CTRLA 		(OFFSET_CCL + 0x00)
#define ADDRESS_CCL_LUT0CTRLA	(OFFSET_CCL + 0x08)
#define ADDRESS_CCL_LUT0CTRLB	(OFFSET_CCL + 0x09)
#define ADDRESS_CCL_LUT0CTRLC	(OFFSET_CCL + 0x0A)
#define ADDRESS_CCL_TRUTH0		(OFFSET_CCL + 0x0B)
#define ADDRESS_CCL_LUT1CTRLA	(OFFSET_CCL + 0x0C)
#define ADDRESS_CCL_LUT1CTRLB	(OFFSET_CCL + 0x0D)
#define ADDRESS_CCL_LUT1CTRLC	(OFFSET_CCL + 0x0E)
#define ADDRESS_CCL_TRUTH1		(OFFSET_CCL + 0x0F)
#define ADDRESS_CCL_LUT2CTRLA	(OFFSET_CCL + 0x10)
#define ADDRESS_CCL_LUT2CTRLB	(OFFSET_CCL + 0x11)
#define ADDRESS_CCL_LUT2CTRLC	(OFFSET_CCL + 0x12)
#define ADDRESS_CCL_TRUTH2		(OFFSET_CCL + 0x13)
#define ADDRESS_CCL_LUT3CTRLA	(OFFSET_CCL + 0x14)
#define ADDRESS_CCL_LUT3CTRLB	(OFFSET_CCL + 0x15)
#define ADDRESS_CCL_LUT3CTRLC	(OFFSET_CCL + 0x16)
#define ADDRESS_CCL_TRUTH3		(OFFSET_CCL + 0x17)

namespace EmbeddedIOServices
{
	class ATTiny427_PassthroughService
	{
    protected:
        ATTiny427_ExpanderService * const _aTTiny427ExpanderService;
        ATTiny427_EVSYSService * const _aTTiny427EVSYSService;
        ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _PORTMUX_EVSYSROUTEA;
        ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _PORTMUX_CCLROUTEA;
        ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _PORTA_PINCTRL_2;
        ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _PORTA_PINCTRL_7;
        ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _PORTB_PINCTRL_2;
        ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _PORTB_PINCTRL_7;
        ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _PORTC_PINCTRL_2;
        ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _AC0_CTRLA;
        ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _CCL_CTRLA;
        ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _CCL_LUT0CTRLA;
        ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _CCL_LUT1CTRLA;
        ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _CCL_LUT2CTRLA;
        ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _CCL_LUT3CTRLA;

        void _setupACPassthrough(digitalpin_t pinIn, digitalpin_t pinOut, bool inverted);

	public:
        ATTiny427_PassthroughService(ATTiny427_ExpanderService *aTTiny427ExpanderService, ATTiny427_EVSYSService *aTTiny427EVSYSService);

		bool InitPassthrough(digitalpin_t pinIn, digitalpin_t pinOut, bool inverted, bool useAC = false);
		void DeinitPassthrough(digitalpin_t pinOut);
	};
}
#endif
