#include "AnalogService_ATTiny427Expander.h"

#ifdef ANALOGSERVICE_ATTINY427EXPANDER_H
namespace EmbeddedIOServices
{
	AnalogService_ATTiny427Expander::AnalogService_ATTiny427Expander(ATTiny427Expander_Registers *registers) : _registers(registers)
    {
        _registers->AnalogAccumulate = 0xF; // Accumulate to 16 bits. might adjust this later on.
    }

	void AnalogService_ATTiny427Expander::InitPin(analogpin_t pin)
	{
        const AnalogChannel_ATTiny427Expander AnalogChannel = PinToAnalogChannel(pin);
        _registers->AnalogEnable |= 0x1 << AnalogChannel;
	}
	
	float AnalogService_ATTiny427Expander::ReadPin(analogpin_t pin)
	{
        const AnalogChannel_ATTiny427Expander AnalogChannel = PinToAnalogChannel(pin);
        return _registers->AnalogValues[AnalogChannel - 1] * (5.0f / (((1 << 12) * (_registers->AnalogAccumulate + 1)) - 1));
	}
}
#endif
