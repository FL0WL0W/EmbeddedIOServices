#include "PwmService_ATTiny427Expander.h"

namespace EmbeddedIOServices
{
	PwmService_ATTiny427Expander::PwmService_ATTiny427Expander(ATTiny427Expander_Registers *registers) : _registers(registers)
    {
    }

	void PwmService_ATTiny427Expander::InitPin(pwmpin_t pin, PinDirection direction, uint16_t minFrequency)
	{
		if (pin == 0xFFFF)
			return;

		if(direction == Out)
		{
			const uint16_t divExact = (20000000 / (minFrequency * ((1 << 16) - 1)));
			uint8_t clksel = 0;
			uint16_t div = 1;
			if(divExact > 255)
			{
				clksel = 0x7;
				div = 1024;
			}
			else if(divExact > 63)
			{
				clksel = 0x6;
				div = 256;
			}
			else if(divExact > 15)
			{
				clksel = 0x5;
				div = 64;
			}
			else if(divExact > 7)
			{
				clksel = 0x4;
				div = 16;
			}
			else if(divExact > 3)
			{
				clksel = 0x3;
				div = 8;
			}
			else if(divExact > 1)
			{
				clksel = 0x2;
				div = 4;
			}
			else if(divExact > 0)
			{
				clksel = 0x1;
				div = 2;
			}
			uint16_t divCurrent = 1;
			switch(_registers->TCA_CTRLA & 0xE)
			{
				case (1 << 1):
					divCurrent = 2;
					break;
				case (2 << 1):
					divCurrent = 4;
					break;
				case (3 << 1):
					divCurrent = 8;
					break;
				case (4 << 1):
					divCurrent = 16;
					break;
				case (5 << 1):
					divCurrent = 64;
					break;
				case (6 << 1):
					divCurrent = 256;
					break;
				case (7 << 1):
					divCurrent = 1024;
					break;
			}
			_registers->TCA_CTRLA = (_registers->TCA_CTRLA & 0xF1) | (clksel << 1);
			_registers->TCA_PERBUF = static_cast<uint32_t>(_registers->TCA_PERBUF) * divCurrent / div;
			_registers->TCA_CMP0BUF = static_cast<uint32_t>(_registers->TCA_CMP0BUF) * divCurrent / div;
			_registers->TCA_CMP1BUF = static_cast<uint32_t>(_registers->TCA_CMP1BUF) * divCurrent / div;
			_registers->TCA_CMP2BUF = static_cast<uint32_t>(_registers->TCA_CMP2BUF) * divCurrent / div;

			switch(pin)
			{
				// TODO: All the ones that are commented out. the below is all i need right now for expander
				// case 3: //TCAWO3 or TCB1WO
				// 	break;
				// case 4: //TCAWO4
				// 	break;
				// case 5: //TCAWO5 or TCB0WO
				// 	break;
				// case 8: //TCAWO0
				// 	break;
				case 9: //TCAWO1
					_registers->TCA_CTRLA |= 0x1; 
					_registers->TCA_CTRLB = (_registers->TCA_CTRLB & 0xF0) | 0x23; 
					_registers->PORTB_DIR |= 0x02;
					break;
				case 10: //TCAWO2
					_registers->TCA_CTRLA |= 0x1;
					_registers->TCA_CTRLB = (_registers->TCA_CTRLB & 0xF0) | 0x43; 
					_registers->PORTB_DIR |= 0x04;
					break;
				// case 11: //TCAWO0'
				// 	break;
				// case 12: //TCAWO1'
				// 	break;
				// case 13: //TCAWO2'
				// 	break;
				// case 16: //TCB0WO'
				// 	break;
				// case 19: //TCAWO3`
				// 	break;
				// case 20: //TCAWO4' or TCB1WO'
				// 	break;
				// case 21: //TCAWO5'
				// 	break;
			}
		}
		else
		{
			//TODO: PWM Input, This will be needed in the future, but not for current expander application
		}
	}
		
	PwmValue PwmService_ATTiny427Expander::ReadPin(pwmpin_t pin)
	{
		PwmValue value = PwmValue();
		if (pin == 0xFFFF)
			return value;

		//TODO: PWM Input, This will be needed in the future, but not for current expander application
		return value;
	}
	
	void PwmService_ATTiny427Expander::WritePin(pwmpin_t pin, PwmValue value)
	{
		if (pin == 0xFFFF)
			return;

		uint16_t divCurrent = 1;
		switch(_registers->TCA_CTRLA & 0xE)
		{
			case (1 << 1):
				divCurrent = 2;
				break;
			case (2 << 1):
				divCurrent = 4;
				break;
			case (3 << 1):
				divCurrent = 8;
				break;
			case (4 << 1):
				divCurrent = 16;
				break;
			case (5 << 1):
				divCurrent = 64;
				break;
			case (6 << 1):
				divCurrent = 256;
				break;
			case (7 << 1):
				divCurrent = 1024;
				break;
		}

		switch(pin)
		{
			// TODO: All the ones that are commented out. the below is all i need right now for expander
			// case 3: //TCAWO3 or TCB1WO
			// 	break;
			// case 4: //TCAWO4
			// 	break;
			// case 5: //TCAWO5 or TCB0WO
			// 	break;
			// case 8: //TCAWO0
			// 	break;
			case 9: //TCAWO1
				_registers->TCA_PERBUF = value.Period * 20000000 / divCurrent;
				_registers->TCA_CMP1BUF = value.PulseWidth * 20000000 / divCurrent;
				break;
			case 10: //TCAWO2
				_registers->TCA_PERBUF = value.Period * 20000000 / divCurrent;
				_registers->TCA_CMP2BUF = value.PulseWidth * 20000000 / divCurrent;
				break;
			// case 11: //TCAWO0'
			// 	break;
			// case 12: //TCAWO1'
			// 	break;
			// case 13: //TCAWO2'
			// 	break;
			// case 16: //TCB0WO'
			// 	break;
			// case 19: //TCAWO3`
			// 	break;
			// case 20: //TCAWO4' or TCB1WO'
			// 	break;
			// case 21: //TCAWO5'
			// 	break;
		}
	}
}