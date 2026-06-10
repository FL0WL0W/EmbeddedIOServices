#include "PinDirection.h"
#include "IPwmService.h"
#include "ATTiny427_ExpanderService.h"

#ifndef PWMSERVICE_ATTINT427EXPANDER_H
#define PWMSERVICE_ATTINT427EXPANDER_H

#define OFFSET_TCA			0x0A00
#define ADDRESS_TCA_CTRLA 	(OFFSET_TCA + 0x00)
#define ADDRESS_TCA_CTRLB 	(OFFSET_TCA + 0x01)
#define ADDRESS_TCA_CTRLD 	(OFFSET_TCA + 0x04)
#define ADDRESS_TCA_PERBUFL 	(OFFSET_TCA + 0x36)
#define ADDRESS_TCA_PERBUFH 	(OFFSET_TCA + 0x37)
#define ADDRESS_TCA_CMP0BUFL	(OFFSET_TCA + 0x38)
#define ADDRESS_TCA_CMP0BUFH	(OFFSET_TCA + 0x39)
#define ADDRESS_TCA_CMP1BUFL	(OFFSET_TCA + 0x3A)
#define ADDRESS_TCA_CMP1BUFH	(OFFSET_TCA + 0x3B)
#define ADDRESS_TCA_CMP2BUFL	(OFFSET_TCA + 0x3C)
#define ADDRESS_TCA_CMP2BUFH	(OFFSET_TCA + 0x3D)

#define OFFSET_PORTMUX				0x05E0
#define ADDRESS_PORTMUX_TCAROUTEA	(OFFSET_PORTMUX + 0x04)

namespace EmbeddedIOServices
{
	class PwmService_ATTiny427Expander : public EmbeddedIOServices::IPwmService
	{
	protected:
		const ATTiny427_ExpanderComm _comm;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _TCA_CTRLA;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _TCA_CTRLB;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _TCA_CTRLD;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _TCA_PERBUFL;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _TCA_PERBUFH;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _TCA_CMP0BUFL;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _TCA_CMP0BUFH;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _TCA_CMP1BUFL;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _TCA_CMP1BUFH;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _TCA_CMP2BUFL;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _TCA_CMP2BUFH;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _PORTMUX_TCAROUTEA;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _VPORTA_DIR;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _VPORTB_DIR;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _VPORTC_DIR;

		void SetupTCADiv8Bit(uint16_t minFrequency);
		void SetupTCADiv16Bit(uint16_t minFrequency);

	public:
		PwmService_ATTiny427Expander(ATTiny427_ExpanderService *aTTiny427ExpanderService);
		void InitPin(pwmpin_t pin, EmbeddedIOServices::PinDirection direction, uint16_t minFrequency);
		EmbeddedIOServices::PwmValue ReadPin(pwmpin_t pin);
		void WritePin(pwmpin_t pin, EmbeddedIOServices::PwmValue value);
	};
}
#endif