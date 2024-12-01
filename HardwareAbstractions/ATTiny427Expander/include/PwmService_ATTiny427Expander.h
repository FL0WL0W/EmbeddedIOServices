#include "PinDirection.h"
#include "IPwmService.h"
#include "ATTiny427ExpanderUpdateService.h"

#ifndef PWMSERVICE_ATTINT427EXPANDER_H
#define PWMSERVICE_ATTINT427EXPANDER_H

namespace EmbeddedIOServices
{
	class PwmService_ATTiny427Expander : public EmbeddedIOServices::IPwmService
	{
	protected:
		ATTiny427Expander_Registers *_registers;
	public:
		PwmService_ATTiny427Expander(ATTiny427Expander_Registers *registers);
		void InitPin(uint16_t pin, EmbeddedIOServices::PinDirection direction, uint16_t minFrequency);
		EmbeddedIOServices::PwmValue ReadPin(uint16_t pin);
		void WritePin(uint16_t pin, EmbeddedIOServices::PwmValue value);
	};
}
#endif