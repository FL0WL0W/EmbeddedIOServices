#include "IAnalogService.h"
#include "ATTiny427ExpanderUpdateService.h"
#include "stddef.h"

#ifndef ANALOGSERVICE_ATTINY427EXPANDER_H
#define ANALOGSERVICE_ATTINY427EXPANDER_H
namespace EmbeddedIOServices
{
	typedef uint8_t AnalogChannel_ATTiny427Expander;
	
	class AnalogService_ATTiny427Expander : public IAnalogService
	{
	protected:
		ATTiny427Expander_Registers *_registers;
	public:
		AnalogService_ATTiny427Expander(ATTiny427Expander_Registers *registers);
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
