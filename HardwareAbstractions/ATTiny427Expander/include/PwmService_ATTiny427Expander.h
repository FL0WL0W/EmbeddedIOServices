#include "PinDirection.h"
#include "IPwmService.h"
#include "ATTiny427_ExpanderService.h"

#ifndef PWMSERVICE_ATTINT427EXPANDER_H
#define PWMSERVICE_ATTINT427EXPANDER_H

#define OFFSET_TCA			0x0A00
#define ADDRESS_TCA_CTRLA 	(OFFSET_TCA + 0x00)
#define ADDRESS_TCA_CTRLB 	(OFFSET_TCA + 0x01)
#define ADDRESS_TCA_PERBUF 	(OFFSET_TCA + 0x36)
#define ADDRESS_TCA_CMP0BUF	(OFFSET_TCA + 0x38)
#define ADDRESS_TCA_CMP1BUF	(OFFSET_TCA + 0x3A)
#define ADDRESS_TCA_CMP2BUF	(OFFSET_TCA + 0x3C)

#define OFFSET_PORTMUX				0x05E0
#define ADDRESS_PORTMUX_TCAROUTEA	(OFFSET_PORTMUX + 0x04)

namespace EmbeddedIOServices
{
	class PwmService_ATTiny427Expander : public EmbeddedIOServices::IPwmService
	{
	protected:
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _TCA_CTRLA;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _TCA_CTRLB;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _TCA_PERBUF;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _TCA_CMP0BUF;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _TCA_CMP1BUF;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _TCA_CMP2BUF;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _PORTMUX_TCAROUTEA;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _VPORTB_DIR;

	public:
		PwmService_ATTiny427Expander(ATTiny427_ExpanderService *aTTiny427ExpanderService);
		void InitPin(pwmpin_t pin, EmbeddedIOServices::PinDirection direction, uint16_t minFrequency);
		EmbeddedIOServices::PwmValue ReadPin(pwmpin_t pin);
		void WritePin(pwmpin_t pin, EmbeddedIOServices::PwmValue value);
	};
}
#endif