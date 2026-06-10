#include "PwmService_ATTiny427Expander.h"
#include "DigitalService_ATTiny427Expander.h"

namespace EmbeddedIOServices
{
	PwmService_ATTiny427Expander::PwmService_ATTiny427Expander(ATTiny427_ExpanderService *aTTiny427ExpanderService) : 
		_comm(aTTiny427ExpanderService->Comm),
		_TCA_CTRLA(aTTiny427ExpanderService->GetRegister(ADDRESS_TCA_CTRLA)),
		_TCA_CTRLB(aTTiny427ExpanderService->GetRegister(ADDRESS_TCA_CTRLB)),
		_TCA_CTRLD(aTTiny427ExpanderService->GetRegister(ADDRESS_TCA_CTRLD)),
		_TCA_PERBUFL(aTTiny427ExpanderService->GetRegister(ADDRESS_TCA_PERBUFL)),
		_TCA_PERBUFH(aTTiny427ExpanderService->GetRegister(ADDRESS_TCA_PERBUFH)),
		_TCA_CMP0BUFL(aTTiny427ExpanderService->GetRegister(ADDRESS_TCA_CMP0BUFL)),
		_TCA_CMP0BUFH(aTTiny427ExpanderService->GetRegister(ADDRESS_TCA_CMP0BUFH)),
		_TCA_CMP1BUFL(aTTiny427ExpanderService->GetRegister(ADDRESS_TCA_CMP1BUFL)),
		_TCA_CMP1BUFH(aTTiny427ExpanderService->GetRegister(ADDRESS_TCA_CMP1BUFH)),
		_TCA_CMP2BUFL(aTTiny427ExpanderService->GetRegister(ADDRESS_TCA_CMP2BUFL)),
		_TCA_CMP2BUFH(aTTiny427ExpanderService->GetRegister(ADDRESS_TCA_CMP2BUFH)),
		_PORTMUX_TCAROUTEA(aTTiny427ExpanderService->GetRegister(ADDRESS_PORTMUX_TCAROUTEA)),
		_VPORTA_DIR(aTTiny427ExpanderService->GetRegister(ADDRESS_VPORTA_DIR)),
		_VPORTB_DIR(aTTiny427ExpanderService->GetRegister(ADDRESS_VPORTB_DIR)),
		_VPORTC_DIR(aTTiny427ExpanderService->GetRegister(ADDRESS_VPORTC_DIR))
    {
    }

	void PwmService_ATTiny427Expander::SetupTCADiv16Bit(uint16_t minFrequency)
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
		switch(_TCA_CTRLA & 0xE)
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
		const uint16_t per = (static_cast<uint16_t>(_TCA_PERBUFL) | (static_cast<uint16_t>(_TCA_PERBUFH) << 8)) * divCurrent / div;
		const uint16_t cmp0 = (static_cast<uint16_t>(_TCA_CMP0BUFL) | (static_cast<uint16_t>(_TCA_CMP0BUFH) << 8)) * divCurrent / div;
		const uint16_t cmp1 = (static_cast<uint16_t>(_TCA_CMP1BUFL) | (static_cast<uint16_t>(_TCA_CMP1BUFH) << 8)) * divCurrent / div;
		const uint16_t cmp2 = (static_cast<uint16_t>(_TCA_CMP2BUFL) | (static_cast<uint16_t>(_TCA_CMP2BUFH) << 8)) * divCurrent / div;
		_TCA_CTRLA = (_TCA_CTRLA & 0xF0) | (clksel << 1);
		if(_TCA_CTRLD == 0x1)
		{
			_TCA_CTRLD = 0x0;
			_TCA_CTRLB = ((_TCA_CTRLB & 0x0F) << 4) | 0x3;
		}
		_TCA_PERBUFH = (per >> 8) & 0xFF;
		_TCA_PERBUFL = per & 0xFF;
		_TCA_CMP0BUFH = (cmp0 >> 8) & 0xFF;
		_TCA_CMP0BUFL = cmp0 & 0xFF;
		_TCA_CMP1BUFH = (cmp1 >> 8) & 0xFF;
		_TCA_CMP1BUFL = cmp1 & 0xFF;
		_TCA_CMP2BUFH = (cmp2 >> 8) & 0xFF;
		_TCA_CMP2BUFL = cmp2 & 0xFF;
		_TCA_CTRLA |= 0x1;
	}

	void PwmService_ATTiny427Expander::SetupTCADiv8Bit(uint16_t minFrequency)
	{
		const uint16_t divExact = (20000000 / (minFrequency * ((1 << 8) - 1)));
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
		switch(_TCA_CTRLA & 0xE)
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
		_TCA_CTRLA = (_TCA_CTRLA & 0xF0) | (clksel << 1);
		if(_TCA_CTRLD == 0x1)
		{
			_TCA_PERBUFH = (static_cast<uint8_t>(_TCA_PERBUFH) * divCurrent / div);
			_TCA_PERBUFL = (static_cast<uint8_t>(_TCA_PERBUFL) * divCurrent / div);
			_TCA_CMP0BUFH = (static_cast<uint8_t>(_TCA_CMP0BUFH) * divCurrent / div);
			_TCA_CMP0BUFL = (static_cast<uint8_t>(_TCA_CMP0BUFL) * divCurrent / div);
			_TCA_CMP1BUFH = (static_cast<uint8_t>(_TCA_CMP1BUFH) * divCurrent / div);
			_TCA_CMP1BUFL = (static_cast<uint8_t>(_TCA_CMP1BUFL) * divCurrent / div);
			_TCA_CMP2BUFH = (static_cast<uint8_t>(_TCA_CMP2BUFH) * divCurrent / div);
			_TCA_CMP2BUFL = (static_cast<uint8_t>(_TCA_CMP2BUFL) * divCurrent / div);
		}
		else
		{
			const uint16_t per = (static_cast<uint16_t>(_TCA_PERBUFL) | (static_cast<uint16_t>(_TCA_PERBUFH) << 8)) * divCurrent / div;
			const uint16_t cmp0 = (static_cast<uint16_t>(_TCA_CMP0BUFL) | (static_cast<uint16_t>(_TCA_CMP0BUFH) << 8)) * divCurrent / div;
			const uint16_t cmp1 = (static_cast<uint16_t>(_TCA_CMP1BUFL) | (static_cast<uint16_t>(_TCA_CMP1BUFH) << 8)) * divCurrent / div;
			const uint16_t cmp2 = (static_cast<uint16_t>(_TCA_CMP2BUFL) | (static_cast<uint16_t>(_TCA_CMP2BUFH) << 8)) * divCurrent / div;
			_TCA_CTRLB = (_TCA_CTRLB & 0xF0) >> 4;
			_TCA_CTRLD = 0x1;
			_TCA_PERBUFH = 0xFF;
			_TCA_PERBUFL = per;
			_TCA_CMP0BUFH = 0xFF;
			_TCA_CMP0BUFL = cmp0 & 0xFF;
			_TCA_CMP1BUFH = 0xFF;
			_TCA_CMP1BUFL = cmp1 & 0xFF;
			_TCA_CMP2BUFH = 0xFF;
			_TCA_CMP2BUFL = cmp2 & 0xFF;
		}
		_TCA_CTRLA |= 0x1;
	}

	void PwmService_ATTiny427Expander::InitPin(pwmpin_t pin, PinDirection direction, uint16_t minFrequency)
	{
		if (pin == 0xFFFF)
			return;

		switch(_comm)
		{
			case ATTiny427_ExpanderComm_SPI:
				if(pin < 5 && pin > 0)
					return;
				break;
			case ATTiny427_ExpanderComm_SPIAlternate:
				if(pin > 15 && pin < 20)
					return;
				break;
			case ATTiny427_ExpanderComm_UART0:
				if(pin > 16 && pin < 19)
					return;
				break;
			default:
				break;
		}

		if(direction == Out)
		{
			switch(pin)
			{
				case 3: //TCAWO3 or TODO TCB1WO
					SetupTCADiv8Bit(minFrequency);
					_TCA_CTRLB = _TCA_CTRLB | 0x10; 
					_PORTMUX_TCAROUTEA &= ~0x8; //set TCA WO4 to PC3
					_VPORTC_DIR |= 0x08;
					break;
				case 4: //TCAWO4
					SetupTCADiv8Bit(minFrequency);
					_TCA_CTRLB = _TCA_CTRLB | 0x20; 
					_PORTMUX_TCAROUTEA &= ~0x10; //set TCA WO4 to PA4
					_VPORTA_DIR |= 0x10;
					break;
				case 5: //TCAWO5 or TODO TCB0WO
					SetupTCADiv8Bit(minFrequency);
					_TCA_CTRLB = _TCA_CTRLB | 0x40; 
					_PORTMUX_TCAROUTEA &= ~0x20; //set TCA WO5 to PC5
					_VPORTC_DIR |= 0x20;
					break;
				case 8: //TCAWO0
					if(_TCA_CTRLD == 0x1 && (_TCA_CTRLB & 0x7) != 0)
						SetupTCADiv8Bit(minFrequency);
					else
						SetupTCADiv16Bit(minFrequency);
					_TCA_CTRLB = _TCA_CTRLB | (_TCA_CTRLD == 0x1? 0x01 : 0x10); 
					_PORTMUX_TCAROUTEA &= ~0x1; //set TCA WO0 to PB0
					_VPORTB_DIR |= 0x01;
					break;
				case 9: //TCAWO1
					if(_TCA_CTRLD == 0x1 && (_TCA_CTRLB & 0x7) != 0)
						SetupTCADiv8Bit(minFrequency);
					else
						SetupTCADiv16Bit(minFrequency);
					_TCA_CTRLB = _TCA_CTRLB | (_TCA_CTRLD == 0x1? 0x02 : 0x20); 
					_PORTMUX_TCAROUTEA &= ~0x2; //set TCA WO1 to PB1
					_VPORTB_DIR |= 0x02;
					break;
				case 10: //TCAWO2
					if(_TCA_CTRLD == 0x1 && (_TCA_CTRLB & 0x7) != 0)
						SetupTCADiv8Bit(minFrequency);
					else
						SetupTCADiv16Bit(minFrequency);
					_TCA_CTRLB = _TCA_CTRLB | (_TCA_CTRLD == 0x1? 0x04 : 0x40); 
					_PORTMUX_TCAROUTEA &= ~0x4; //set TCA WO2 to PB2
					_VPORTB_DIR |= 0x04;
					break;
				case 11: //TCAWO0'
					if(_TCA_CTRLD == 0x1 && (_TCA_CTRLB & 0x7) != 0)
						SetupTCADiv8Bit(minFrequency);
					else
						SetupTCADiv16Bit(minFrequency);
					_TCA_CTRLB = _TCA_CTRLB | (_TCA_CTRLD == 0x1? 0x01 : 0x10);; 
					_PORTMUX_TCAROUTEA |= 0x1; //set TCA WO0 to PB3
					_VPORTB_DIR |= 0x08;
					break;
				case 12: //TCAWO1'
					if(_TCA_CTRLD == 0x1 && (_TCA_CTRLB & 0x7) != 0)
						SetupTCADiv8Bit(minFrequency);
					else
						SetupTCADiv16Bit(minFrequency);
					_TCA_CTRLB = _TCA_CTRLB | (_TCA_CTRLD == 0x1? 0x02 : 0x20); 
					_PORTMUX_TCAROUTEA |= 0x2; //set TCA WO1 to PB4
					_VPORTB_DIR |= 0x10;
					break;
				case 13: //TCAWO2'
					if(_TCA_CTRLD == 0x1 && (_TCA_CTRLB & 0x7) != 0)
						SetupTCADiv8Bit(minFrequency);
					else
						SetupTCADiv16Bit(minFrequency);
					_TCA_CTRLB = _TCA_CTRLB | (_TCA_CTRLD == 0x1? 0x04 : 0x40); 
					_PORTMUX_TCAROUTEA |= 0x4; //set TCA WO2 to PB5
					_VPORTB_DIR |= 0x20;
					break;
				// case 16: //TCB0WO'
				// 	break;
				case 19: //TCAWO3`
					SetupTCADiv8Bit(minFrequency);
					_TCA_CTRLB = _TCA_CTRLB | 0x10; 
					_PORTMUX_TCAROUTEA |= 0x8; //set TCA WO4 to PC3
					_VPORTC_DIR |= 0x08;
					break;
				case 20: //TCAWO4' or TODO TCB1WO'
					SetupTCADiv8Bit(minFrequency);
					_TCA_CTRLB = _TCA_CTRLB | 0x20; 
					_PORTMUX_TCAROUTEA |= 0x10; //set TCA WO4 to PC4
					_VPORTC_DIR |= 0x10;
					break;
				case 21: //TCAWO5'
					SetupTCADiv8Bit(minFrequency);
					_TCA_CTRLB = _TCA_CTRLB | 0x40; 
					_PORTMUX_TCAROUTEA |= 0x20; //set TCA WO5 to PC5
					_VPORTC_DIR |= 0x20;
					break;
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
		switch(_TCA_CTRLA & 0xE)
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
			case 3: //TCAWO3 or TODO TCB1WO
				_TCA_PERBUFH = value.Period * 20000000 / divCurrent + 0.5f;
				_TCA_CMP0BUFH = value.PulseWidth * 20000000 / divCurrent + 0.5f;
				break;
			case 4: //TCAWO4
				_TCA_PERBUFH = value.Period * 20000000 / divCurrent + 0.5f;
				_TCA_CMP1BUFH = value.PulseWidth * 20000000 / divCurrent + 0.5f;
				break;
			case 5: //TCAWO5 or TODO TCB0WO
				_TCA_PERBUFH = value.Period * 20000000 / divCurrent + 0.5f;
				_TCA_CMP2BUFH = value.PulseWidth * 20000000 / divCurrent + 0.5f;
				break;
			case 8: //TCAWO0
			case 11: //TCAWO0'
			{
				const uint16_t per = value.Period * 20000000 / divCurrent + 0.5f;
				const uint16_t cmp0 = value.PulseWidth * 20000000 / divCurrent + 0.5f;
				if(_TCA_CTRLD == 0x1)
				{
					_TCA_PERBUFH = (per >> 8) & 0xFF;
					_TCA_PERBUFL = per & 0xFF;
					_TCA_CMP0BUFH = (cmp0 >> 8) & 0xFF;
					_TCA_CMP0BUFL = cmp0 & 0xFF;
				}
				else
				{
					_TCA_PERBUFL = per;
					_TCA_CMP0BUFL = cmp0;
				}
				break;
			}
			case 9: //TCAWO1
			case 12: //TCAWO1'
			{
				const uint16_t per = value.Period * 20000000 / divCurrent + 0.5f;
				const uint16_t cmp1 = value.PulseWidth * 20000000 / divCurrent + 0.5f;
				if(_TCA_CTRLD == 0x1)
				{
					_TCA_PERBUFH = (per >> 8) & 0xFF;
					_TCA_PERBUFL = per & 0xFF;
					_TCA_CMP1BUFH = (cmp1 >> 8) & 0xFF;
					_TCA_CMP1BUFL = cmp1 & 0xFF;
				}
				else
				{
					_TCA_PERBUFL = per;
					_TCA_CMP1BUFL = cmp1;
				}
				break;
			}
			case 10: //TCAWO2
			case 13: //TCAWO2'
			{
				const uint16_t per = value.Period * 20000000 / divCurrent + 0.5f;
				const uint16_t cmp2 = value.PulseWidth * 20000000 / divCurrent + 0.5f;
				if(_TCA_CTRLD == 0x1)
				{
					_TCA_PERBUFH = (per >> 8) & 0xFF;
					_TCA_PERBUFL = per & 0xFF;
					_TCA_CMP2BUFH = (cmp2 >> 8) & 0xFF;
					_TCA_CMP2BUFL = cmp2 & 0xFF;
				}
				else
				{
					_TCA_PERBUFL = per;
					_TCA_CMP2BUFL = cmp2;
				}
				break;
			}
			// case 16: //TCB0WO'
			// 	break;
			case 19: //TCAWO3`
				_TCA_PERBUFH = value.Period * 20000000 / divCurrent + 0.5f;
				_TCA_CMP0BUFH = value.PulseWidth * 20000000 / divCurrent + 0.5f;
				break;
			case 20: //TCAWO4' or TODO TCB1WO'
				_TCA_PERBUFH = value.Period * 20000000 / divCurrent + 0.5f;
				_TCA_CMP1BUFH = value.PulseWidth * 20000000 / divCurrent + 0.5f;
				break;
			case 21: //TCAWO5'
				_TCA_PERBUFH = value.Period * 20000000 / divCurrent + 0.5f;
				_TCA_CMP2BUFH = value.PulseWidth * 20000000 / divCurrent + 0.5f;
				break;
		}
	}
}