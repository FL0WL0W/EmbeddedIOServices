#include "IAnalogService.h"
#include "ATTiny427_ExpanderService.h"
#include "stddef.h"

#ifndef ANALOGSERVICE_ATTINY427EXPANDER_H
#define ANALOGSERVICE_ATTINY427EXPANDER_H

#define ADDRESS_ANALOG_ENABLE_LOW	0x1C
#define ADDRESS_ANALOG_ENABLE_HIGH	0x1D
#define ADDRESS_ANALOG_ACCUMULATE	0x1E
#define ADDRESS_ANALOG_VALUE_START	0x340D

#define OFFSET_ADC0 			0x600
#define ADDRESS_ADC0_CTRLA 		(OFFSET_ADC0 + 0x00)
#define ADDRESS_ADC0_CTRLB 		(OFFSET_ADC0 + 0x01)
#define ADDRESS_ADC0_CTRLC 		(OFFSET_ADC0 + 0x02)
#define ADDRESS_ADC0_INTCTRL	(OFFSET_ADC0 + 0x04)
#define ADDRESS_ADC0_CTRLE 		(OFFSET_ADC0 + 0x08)
#define ADDRESS_ADC0_CTRLF 		(OFFSET_ADC0 + 0x09)
#define ADDRESS_ADC0_COMMAND 	(OFFSET_ADC0 + 0x0A)

namespace EmbeddedIOServices
{
	typedef int8_t AnalogChannel_ATTiny427Expander;
	
	class AnalogService_ATTiny427Expander : public IAnalogService
	{
	protected:
		ATTiny427_ExpanderService * const _aTTiny427ExpanderService;
		ATTiny427_ExpanderService::ATTiny427_ExpanderPoller * const _poller;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _accumulate;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _analogEnableLow;
		ATTiny427_ExpanderService::Attiny427_ExpanderRegister & _analogEnableHigh;
		volatile uint16_t _analogValues[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		volatile uint8_t _seq = 0;

	public:
		AnalogService_ATTiny427Expander(ATTiny427_ExpanderService *aTTiny427ExpanderService, uint8_t accumulate);
		~AnalogService_ATTiny427Expander();
		void InitPin(analogpin_t pin);
		float ReadPin(analogpin_t pin);

		static inline AnalogChannel_ATTiny427Expander PinToAnalogChannel(analogpin_t pin)
		{
			switch(pin)
			{
				case 1:
					return 1;
				case 2:
					return 2;
				case 3:
					return 3;
				case 4:
					return 4;
				case 5:
					return 5;
				case 6:
					return 6;
				case 7:
					return 7;
				case 13:
					return 8;
				case 12:
					return 9;
				case 9:
					return 10;
				case 8:
					return 11;
				case 16:
					return 12;
				case 17:
					return 13;
				case 18:
					return 14;
				case 19:
					return 15;
				default:
					return -1;
			}
		}
	};
}
#endif
