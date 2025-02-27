#include "DigitalService_ATTiny427Expander.h"

#ifdef DIGITALSERVICE_ATTINY427EXPANDER_H
namespace EmbeddedIOServices
{
    DigitalService_ATTiny427Expander::DigitalService_ATTiny427Expander(ATTiny427Expander_Registers *registers) : _registers(registers)
    {
    }
	void DigitalService_ATTiny427Expander::InitPin(digitalpin_t pin, PinDirection direction)
	{
		const DigitalPort_ATTiny427Expander DigitalPort = PinToDigitalPort(pin);
		const DigitalPin_ATTiny427Expander DigitalPin = PinToDigitalPin(pin);
		switch(DigitalPort)
		{
			case PORTA:
				if(direction == In)
					_registers->PORTA_DIR &= ~DigitalPin;
				else
					_registers->PORTA_DIR |= DigitalPin;
				return;
			case PORTB:
				if(direction == In)
					_registers->PORTB_DIR &= ~DigitalPin;
				else
					_registers->PORTB_DIR |= DigitalPin;
				return;
			case PORTC:
				if(direction == In)
					_registers->PORTC_DIR &= ~DigitalPin;
				else
					_registers->PORTC_DIR |= DigitalPin;
				return;
		}
	}
	bool DigitalService_ATTiny427Expander::ReadPin(digitalpin_t pin)
	{
		const DigitalPort_ATTiny427Expander DigitalPort = PinToDigitalPort(pin);
		const DigitalPin_ATTiny427Expander DigitalPin = PinToDigitalPin(pin);
		switch(DigitalPort)
		{
			case PORTA:
				return _registers->PORTA_IN & DigitalPin;
			case PORTB:
				return _registers->PORTB_IN & DigitalPin;
			case PORTC:
				return _registers->PORTC_IN & DigitalPin;
			default:
				return false;
		}
	}
	void DigitalService_ATTiny427Expander::WritePin(digitalpin_t pin, bool value)
	{
		const DigitalPort_ATTiny427Expander DigitalPort = PinToDigitalPort(pin);
		const DigitalPin_ATTiny427Expander DigitalPin = PinToDigitalPin(pin);
		switch(pin>>3)
		{
			case PORTA:
				if(value == false)
					_registers->PORTA_OUT &= ~DigitalPin;
				else
					_registers->PORTA_OUT |= DigitalPin;
				return;
			case PORTB:
				if(value == false)
					_registers->PORTB_OUT &= ~DigitalPin;
				else
					_registers->PORTB_OUT |= DigitalPin;
				return;
			case PORTC:
				if(value == false)
					_registers->PORTC_OUT &= ~DigitalPin;
				else
					_registers->PORTC_OUT |= DigitalPin;
				return;
		}
	}
	void DigitalService_ATTiny427Expander::AttachInterrupt(digitalpin_t pin, callback_t callBack)
	{
		const DigitalPort_ATTiny427Expander DigitalPort = PinToDigitalPort(pin);
		const DigitalPin_ATTiny427Expander DigitalPin = PinToDigitalPin(pin);
		_interruptList.push_front(DigitalInterrupt_ATTiny427Expander(DigitalPort, DigitalPin, callBack));

	}
	void DigitalService_ATTiny427Expander::DetachInterrupt(digitalpin_t pin)
	{
		const DigitalPort_ATTiny427Expander DigitalPort = PinToDigitalPort(pin);
		const DigitalPin_ATTiny427Expander DigitalPin = PinToDigitalPin(pin);
		_interruptList.remove_if([DigitalPort, DigitalPin](const DigitalInterrupt_ATTiny427Expander& interrupt) { return interrupt.DigitalPort == DigitalPort && interrupt.DigitalPin == DigitalPin; });
	}
	bool DigitalService_ATTiny427Expander::InitPassthrough(digitalpin_t pinIn, digitalpin_t pinOut, bool inverted, bool useAC)
	{
		if(!(pinIn == 7 || pinIn == 13 || pinIn == 9 || pinIn == 14))
			useAC = false;

		if(useAC)
		{
			_registers->AC_DACREF = 0x85;
			_registers->AC_MUXCTRLA = 0x3;
			switch(pinIn)
			{
				case 13:
					_registers->AC_MUXCTRLA |= 0x1 << 3;;
					break;
				case 9:
					_registers->AC_MUXCTRLA |= 0x2 << 3;
					break;
				case 14:
					_registers->AC_MUXCTRLA |= 0x3 << 3;
					break;
			}
			_registers->AC_CTRLA = (_registers->AC_CTRLA & 0b11000111) | 0b10000111; //enable with large hysteresis
		}

		DeinitPassthrough(pinIn);
		switch(pinOut)
		{
			case 2:
			{
				//EVOUTA
				if(_registers->Comm == ATTiny427Expander_Comm_SPI || _registers->Comm == ATTiny427Expander_Comm_UART0Alternate || _registers->Comm == ATTiny427Expander_Comm_UART1)
					return false;
				if(_registers->PORTMUX_EVSYSROUTEA & 0x1 && _registers->EVSYS_EVOUTA != 0)
					DeinitPassthrough(7);
				const ATTiny427Expander_EVSysChannel channel = useAC? _registers->FirstOpenChannel(0x20) : _registers->GetEVSYSChannel(pinIn);
				if(channel.channel == -1)
					return false;
				_registers->EVSYS_CHANNEL[channel.channel] = channel.generator;
				_registers->PORTMUX_EVSYSROUTEA &= ~0x1;
				if(inverted)
					_registers->PORTA_PINCTRL[2] |= 0x80;
				else 
					_registers->PORTA_PINCTRL[2] &= ~0x80;
				_registers->EVSYS_EVOUTA = channel.channel + 1;
				return true;
			}
			case 4:
				//LUT0
				if(_registers->Comm == ATTiny427Expander_Comm_SPI)
					return false;
				DeinitPassthrough(12);
				_registers->PORTMUX_CCLROUTEA &= ~0x1;
				_registers->CCL_LUT0TRUTH = inverted? 0x1 : 0xFE;
				if(useAC)
				{
					_registers->CCL_LUT0CTRLB = 0x60;
					_registers->CCL_LUT0CTRLC = 0x00;
				}
				else
				{
					switch(pinIn)
					{
						case 0: 
							_registers->CCL_LUT0CTRLB = 0x05;
							_registers->CCL_LUT0CTRLC = 0x00;
							break;
						case 1:
							_registers->CCL_LUT0CTRLB = 0x50;
							_registers->CCL_LUT0CTRLC = 0x00;
							break;
						case 2:
							_registers->CCL_LUT0CTRLB = 0x00;
							_registers->CCL_LUT0CTRLC = 0x05;
							break;
						default:
						{
							_registers->CCL_LUT0CTRLB = 0x30;
							_registers->CCL_LUT0CTRLC = 0x00;
							const ATTiny427Expander_EVSysChannel channel = _registers->GetEVSYSChannel(pinIn);
							if(channel.channel == -1)
								return false;
							_registers->EVSYS_CHANNEL[channel.channel] = channel.generator;
							_registers->EVSYS_CCL_LUT0A = channel.channel + 1;
							break;
						}
					}
				}
				_registers->CCL_LUT0CTRLA = 0x41;
				_registers->CCL_CTRLA = 0x41;
				return true;
			case 5:
			{
				//primarily use ACOUT since it does not have any alternates and doesn't use any EventChannels
				if(useAC)
				{
					_registers->AC_CTRLA |= 0b01000000; //enable output
					_registers->AC_MUXCTRLA |= (inverted? 0b10000000 : 0);
					return true;
				}
				//otherwise use LUT3', if LUT3 was already being used, it will be overwritten

				DeinitPassthrough(20);
				_registers->PORTMUX_CCLROUTEA |= 0x8;
				_registers->CCL_LUT3TRUTH = inverted? 0x1 : 0xFE;
				switch(pinIn)
				{
					case 16: 
						_registers->CCL_LUT3CTRLB = 0x05;
						_registers->CCL_LUT3CTRLC = 0x00;
						break;
					case 17:
						_registers->CCL_LUT3CTRLB = 0x50;
						_registers->CCL_LUT3CTRLC = 0x00;
						break;
					case 18:
						_registers->CCL_LUT3CTRLB = 0x00;
						_registers->CCL_LUT3CTRLC = 0x05;
						break;
					default:
					{
						_registers->CCL_LUT3CTRLB = 0x03;
						_registers->CCL_LUT3CTRLC = 0x00;
						const ATTiny427Expander_EVSysChannel channel = _registers->GetEVSYSChannel(pinIn);
						if(channel.channel == -1)
							return false;
						_registers->EVSYS_CHANNEL[channel.channel] = channel.generator;
						_registers->EVSYS_CCL_LUT3A = channel.channel + 1;
						break;
					}
				}
				_registers->CCL_LUT3CTRLA = 0x41;
				_registers->CCL_CTRLA = 0x41;
				return true;
			}
			case 7:
			{
				if(_registers->Comm == ATTiny427Expander_Comm_SPIAlternate || _registers->Comm == ATTiny427Expander_Comm_UART1Alternate)
				{
					//LUT1
					if(_registers->PORTMUX_CCLROUTEA & 0x2 && _registers->CCL_LUT1CTRLA != 0)
						DeinitPassthrough(17);
					_registers->PORTMUX_CCLROUTEA |= 0x2;
					_registers->CCL_LUT1TRUTH = inverted? 0x1 : 0xFE;
					if(useAC)
					{
						_registers->CCL_LUT1CTRLB = 0x66;
						_registers->CCL_LUT1CTRLC = 0x06;
					}
					else
					{
						switch(pinIn)
						{
							case 19: 
								_registers->CCL_LUT1CTRLB = 0x05;
								_registers->CCL_LUT1CTRLC = 0x00;
								break;
							case 20:
								_registers->CCL_LUT1CTRLB = 0x50;
								_registers->CCL_LUT1CTRLC = 0x00;
								break;
							case 21:
								_registers->CCL_LUT1CTRLB = 0x00;
								_registers->CCL_LUT1CTRLC = 0x05;
								break;
							default:
							{
								_registers->CCL_LUT1CTRLB = 0x03;
								_registers->CCL_LUT1CTRLC = 0x00;
								const ATTiny427Expander_EVSysChannel channel = _registers->GetEVSYSChannel(pinIn);
								if(channel.channel == -1)
									return false;
								_registers->EVSYS_CHANNEL[channel.channel] = channel.generator;
								_registers->EVSYS_CCL_LUT1A = channel.channel + 1;
								break;
							}
						}
					}
					_registers->CCL_LUT1CTRLA = 0x41;
					_registers->CCL_CTRLA = 0x41;
					return true;
				}
				//EVOUTA'
				DeinitPassthrough(2);
				const ATTiny427Expander_EVSysChannel channel = useAC? _registers->FirstOpenChannel(0x20) : _registers->GetEVSYSChannel(pinIn);
				if(channel.channel == -1)
					return false;
				_registers->EVSYS_CHANNEL[channel.channel] = channel.generator;
				_registers->PORTMUX_EVSYSROUTEA |= 0x1;
				if(inverted)
					_registers->PORTA_PINCTRL[7] |= 0x80;
				else 
					_registers->PORTA_PINCTRL[7] &= ~0x80;
				_registers->EVSYS_EVOUTA = channel.channel + 1;
				return true;
			}
			case 10:
			{
				//EVOUTB
				DeinitPassthrough(15);
				const ATTiny427Expander_EVSysChannel channel = useAC? _registers->FirstOpenChannel(0x20) : _registers->GetEVSYSChannel(pinIn);
				if(channel.channel == -1)
					return false;
				_registers->EVSYS_CHANNEL[channel.channel] = channel.generator;
				_registers->PORTMUX_EVSYSROUTEA &= ~0x2;
				if(inverted)
					_registers->PORTB_PINCTRL[2] |= 0x80;
				else 
					_registers->PORTB_PINCTRL[2] &= ~0x80;
				_registers->EVSYS_EVOUTB = channel.channel + 1;
				return true;
			}
			case 11:
				//LUT2
				DeinitPassthrough(14);
				_registers->PORTMUX_CCLROUTEA &= ~0x4;
				_registers->CCL_LUT2TRUTH = inverted? 0x1 : 0xFE;
				if(useAC)
				{
					_registers->CCL_LUT2CTRLB = 0x66;
					_registers->CCL_LUT2CTRLC = 0x06;
				}
				else
				{
					switch(pinIn)
					{
						case 8: 
							_registers->CCL_LUT2CTRLB = 0x05;
							_registers->CCL_LUT2CTRLC = 0x00;
							break;
						case 9:
							_registers->CCL_LUT2CTRLB = 0x50;
							_registers->CCL_LUT2CTRLC = 0x00;
							break;
						case 10:
							_registers->CCL_LUT2CTRLB = 0x00;
							_registers->CCL_LUT2CTRLC = 0x05;
							break;
						default:
						{
							_registers->CCL_LUT2CTRLB = 0x03;
							_registers->CCL_LUT2CTRLC = 0x00;
							const ATTiny427Expander_EVSysChannel channel = _registers->GetEVSYSChannel(pinIn);
							if(channel.channel == -1)
								return false;
							_registers->EVSYS_CHANNEL[channel.channel] = channel.generator;
							_registers->EVSYS_CCL_LUT2A = channel.channel + 1;
							break;
						}
					}
				}
				_registers->CCL_LUT2CTRLA = 0x41;
				_registers->CCL_CTRLA = 0x41;
				return true;
			case 12:
				//LUT0'
				DeinitPassthrough(4);
				_registers->PORTMUX_CCLROUTEA |= 0x1;
				_registers->CCL_LUT0TRUTH = inverted? 0x1 : 0xFE;
				if(useAC)
				{
					_registers->CCL_LUT0CTRLB = 0x66;
					_registers->CCL_LUT0CTRLC = 0x06;
				}
				else
				{
					switch(pinIn)
					{
						case 0: 
							_registers->CCL_LUT0CTRLB = 0x05;
							_registers->CCL_LUT0CTRLC = 0x00;
							break;
						case 1:
							_registers->CCL_LUT0CTRLB = 0x50;
							_registers->CCL_LUT0CTRLC = 0x00;
							break;
						case 2:
							_registers->CCL_LUT0CTRLB = 0x00;
							_registers->CCL_LUT0CTRLC = 0x05;
							break;
						default:
						{
							_registers->CCL_LUT0CTRLB = 0x03;
							_registers->CCL_LUT0CTRLC = 0x00;
							const ATTiny427Expander_EVSysChannel channel = _registers->GetEVSYSChannel(pinIn);
							if(channel.channel == -1)
								return false;
							_registers->EVSYS_CHANNEL[channel.channel] = channel.generator;
							_registers->EVSYS_CCL_LUT0A = channel.channel + 1;
							break;
						}
					}
				}
				_registers->CCL_LUT0CTRLA = 0x41;
				_registers->CCL_CTRLA = 0x41;
				return true;
			case 14:
				//LUT2'
				DeinitPassthrough(11);
				_registers->PORTMUX_CCLROUTEA |= 0x4;
				_registers->CCL_LUT2TRUTH = inverted? 0x1 : 0xFE;
				if(useAC)
				{
					_registers->CCL_LUT2CTRLB = 0x66;
					_registers->CCL_LUT2CTRLC = 0x06;
				}
				else
				{
					switch(pinIn)
					{
						case 8: 
							_registers->CCL_LUT2CTRLB = 0x05;
							_registers->CCL_LUT2CTRLC = 0x00;
							break;
						case 9:
							_registers->CCL_LUT2CTRLB = 0x50;
							_registers->CCL_LUT2CTRLC = 0x00;
							break;
						case 10:
							_registers->CCL_LUT2CTRLB = 0x00;
							_registers->CCL_LUT2CTRLC = 0x05;
							break;
						default:
						{
							_registers->CCL_LUT2CTRLB = 0x03;
							_registers->CCL_LUT2CTRLC = 0x00;
							const ATTiny427Expander_EVSysChannel channel = _registers->GetEVSYSChannel(pinIn);
							if(channel.channel == -1)
								return false;
							_registers->EVSYS_CHANNEL[channel.channel] = channel.generator;
							_registers->EVSYS_CCL_LUT2A = channel.channel + 1;
							break;
						}
					}
				}
				_registers->CCL_LUT2CTRLA = 0x41;
				_registers->CCL_CTRLA = 0x41;
				return true;
			case 15:
			{
				//EVOUTB'
				DeinitPassthrough(10);
				const ATTiny427Expander_EVSysChannel channel = useAC? _registers->FirstOpenChannel(0x20) : _registers->GetEVSYSChannel(pinIn);
				if(channel.channel == -1)
					return false;
				_registers->EVSYS_CHANNEL[channel.channel] = channel.generator;
				_registers->PORTMUX_EVSYSROUTEA |= 0x2;
				if(inverted)
					_registers->PORTB_PINCTRL[7] |= 0x80;
				else 
					_registers->PORTB_PINCTRL[7] &= ~0x80;
				_registers->EVSYS_EVOUTB = channel.channel + 1;
				return true;
			}
			case 17:
				//LUT1'
				if(_registers->Comm == ATTiny427Expander_Comm_SPIAlternate || _registers->Comm == ATTiny427Expander_Comm_UART1Alternate)
					return false;
				_registers->PORTMUX_CCLROUTEA |= 0x2;
				_registers->CCL_LUT1TRUTH = inverted? 0x1 : 0xFE;
				if(useAC)
				{
					_registers->CCL_LUT1CTRLB = 0x66;
					_registers->CCL_LUT1CTRLC = 0x06;
				}
				else
				{
					switch(pinIn)
					{
						case 19: 
							_registers->CCL_LUT1CTRLB = 0x05;
							_registers->CCL_LUT1CTRLC = 0x00;
							break;
						case 20:
							_registers->CCL_LUT1CTRLB = 0x50;
							_registers->CCL_LUT1CTRLC = 0x00;
							break;
						case 21:
							_registers->CCL_LUT1CTRLB = 0x00;
							_registers->CCL_LUT1CTRLC = 0x05;
							break;
						default:
						{
							_registers->CCL_LUT1CTRLB = 0x03;
							_registers->CCL_LUT1CTRLC = 0x00;
							const ATTiny427Expander_EVSysChannel channel = _registers->GetEVSYSChannel(pinIn);
							if(channel.channel == -1)
								return false;
							_registers->EVSYS_CHANNEL[channel.channel] = channel.generator;
							_registers->EVSYS_CCL_LUT1A = channel.channel + 1;
							break;
						}
					}
				}
				_registers->CCL_LUT1CTRLA = 0x41;
				_registers->CCL_CTRLA = 0x41;
				return true;
			case 18:
			{
				if(_registers->Comm == ATTiny427Expander_Comm_SPIAlternate || _registers->Comm == ATTiny427Expander_Comm_UART1Alternate)
					return false;
				//EVOUTC
				const ATTiny427Expander_EVSysChannel channel = useAC? _registers->FirstOpenChannel(0x20) : _registers->GetEVSYSChannel(pinIn);
				if(channel.channel == -1)
					return false;
				_registers->EVSYS_CHANNEL[channel.channel] = channel.generator;
				if(inverted)
					_registers->PORTC_PINCTRL[2] |= 0x80;
				else 
					_registers->PORTC_PINCTRL[2] &= ~0x80;
				_registers->EVSYS_EVOUTC = channel.channel + 1;
				return true;
			}
			case 20:
				//LUT3
				if(_registers->PORTMUX_CCLROUTEA & 0x8 && _registers->CCL_LUT3CTRLA != 0)
					DeinitPassthrough(5);
				_registers->PORTMUX_CCLROUTEA &= ~0x8;
				_registers->CCL_LUT3TRUTH = inverted? 0x1 : 0xFE;
				if(useAC)
				{
					_registers->CCL_LUT3CTRLB = 0x66;
					_registers->CCL_LUT3CTRLC = 0x06;
				}
				else
				{
					switch(pinIn)
					{
						case 16: 
							_registers->CCL_LUT3CTRLB = 0x05;
							_registers->CCL_LUT3CTRLC = 0x00;
							break;
						case 17:
							_registers->CCL_LUT3CTRLB = 0x50;
							_registers->CCL_LUT3CTRLC = 0x00;
							break;
						case 18:
							_registers->CCL_LUT3CTRLB = 0x00;
							_registers->CCL_LUT3CTRLC = 0x05;
							break;
						default:
						{
							_registers->CCL_LUT3CTRLB = 0x03;
							_registers->CCL_LUT3CTRLC = 0x00;
							const ATTiny427Expander_EVSysChannel channel = _registers->GetEVSYSChannel(pinIn);
							if(channel.channel == -1)
								return false;
							_registers->EVSYS_CHANNEL[channel.channel] = channel.generator;
							_registers->EVSYS_CCL_LUT3A = channel.channel + 1;
							break;
						}
					}
				}
				_registers->CCL_LUT3CTRLA = 0x41;
				_registers->CCL_CTRLA = 0x41;
				return true;
		}
		return false;
	}
	void DigitalService_ATTiny427Expander::DeinitPassthrough(digitalpin_t pinOut)
	{
		switch(pinOut)
		{
			case 2:
				if(_registers->Comm == ATTiny427Expander_Comm_SPI || _registers->Comm == ATTiny427Expander_Comm_UART0Alternate || _registers->Comm == ATTiny427Expander_Comm_UART1)
					return;
				//EVOUTA
				if(!(_registers->PORTMUX_EVSYSROUTEA & 0x1))
					return;
				if(_registers->EVSYS_EVOUTA > 0)
				{
					const uint8_t channel = _registers->EVSYS_EVOUTA;
					_registers->EVSYS_EVOUTA = 0;
					_registers->PORTA_PINCTRL[2] &= ~0x80;
					for(uint8_t i = 0; i < sizeof(_registers->EVSYS_CHANNELUSER); i++)
					{
						if(_registers->EVSYS_CHANNELUSER[i] == channel)
							return;
					}
					_registers->EVSYS_CHANNEL[channel - 1] = 0;
				}
				return;
			case 4:
				if(_registers->Comm == ATTiny427Expander_Comm_SPI)
					return;
				//LUT0
				if(_registers->PORTMUX_CCLROUTEA & 0x1)
					return;
				_registers->CCL_LUT0CTRLA = 0;
				if(_registers->EVSYS_CCL_LUT0A > 0)
				{
					const uint8_t channel = _registers->EVSYS_CCL_LUT0A;
					_registers->EVSYS_CCL_LUT0A = 0;
					for(uint8_t i = 0; i < sizeof(_registers->EVSYS_CHANNELUSER); i++)
					{
						if(_registers->EVSYS_CHANNELUSER[i] == channel)
							return;
					}
					_registers->EVSYS_CHANNEL[channel - 1] = 0;
				}
				return;
			case 5:
				//disable AC OUT
				_registers->AC_CTRLA &= ~0b01000000;
				if(!(_registers->PORTMUX_CCLROUTEA & 0x8))
					return;
				_registers->CCL_LUT3CTRLA = 0;
				if(_registers->EVSYS_CCL_LUT3A > 0)
				{
					const uint8_t channel = _registers->EVSYS_CCL_LUT3A;
					_registers->EVSYS_CCL_LUT3A = 0;
					for(uint8_t i = 0; i < sizeof(_registers->EVSYS_CHANNELUSER); i++)
					{
						if(_registers->EVSYS_CHANNELUSER[i] == channel)
							return;
					}
					_registers->EVSYS_CHANNEL[channel - 1] = 0;
				}
				return;
			case 7:
				if(_registers->Comm == ATTiny427Expander_Comm_SPIAlternate || _registers->Comm == ATTiny427Expander_Comm_UART1Alternate)
				{
					//LUT1
					_registers->CCL_LUT1CTRLA = 0;
					if(_registers->EVSYS_CCL_LUT1A > 0)
					{
						const uint8_t channel = _registers->EVSYS_CCL_LUT1A;
						_registers->EVSYS_CCL_LUT1A = 0;
						for(uint8_t i = 0; i < sizeof(_registers->EVSYS_CHANNELUSER); i++)
						{
							if(_registers->EVSYS_CHANNELUSER[i] == channel)
								return;
						}
						_registers->EVSYS_CHANNEL[channel - 1] = 0;
					}
					return;
				}
				//EVOUTA'
				if(_registers->PORTMUX_EVSYSROUTEA & 0x1)
					return;
				if(_registers->EVSYS_EVOUTA > 0)
				{
					const uint8_t channel = _registers->EVSYS_EVOUTA;
					_registers->EVSYS_EVOUTA = 0;
					_registers->PORTA_PINCTRL[7] &= ~0x80;
					for(uint8_t i = 0; i < sizeof(_registers->EVSYS_CHANNELUSER); i++)
					{
						if(_registers->EVSYS_CHANNELUSER[i] == channel)
							return;
					}
					_registers->EVSYS_CHANNEL[channel - 1] = 0;
				}
				return;
			case 10:
				//EVOUTB
				if(_registers->PORTMUX_EVSYSROUTEA & 0x2)
					return;
				if(_registers->EVSYS_EVOUTB > 0)
				{
					const uint8_t channel = _registers->EVSYS_EVOUTB;
					_registers->EVSYS_EVOUTB = 0;
					_registers->PORTB_PINCTRL[2] &= ~0x80;
					for(uint8_t i = 0; i < sizeof(_registers->EVSYS_CHANNELUSER); i++)
					{
						if(_registers->EVSYS_CHANNELUSER[i] == channel)
							return;
					}
					_registers->EVSYS_CHANNEL[channel - 1] = 0;
				}
				return;
			case 11:
				//LUT2
				if(_registers->PORTMUX_CCLROUTEA & 0x4)
					return;
				_registers->CCL_LUT2CTRLA = 0;
				if(_registers->EVSYS_CCL_LUT2A > 0)
				{
					const uint8_t channel = _registers->EVSYS_CCL_LUT2A;
					_registers->EVSYS_CCL_LUT2A = 0;
					for(uint8_t i = 0; i < sizeof(_registers->EVSYS_CHANNELUSER); i++)
					{
						if(_registers->EVSYS_CHANNELUSER[i] == channel)
							return;
					}
					_registers->EVSYS_CHANNEL[channel - 1] = 0;
				}
				return;
			case 12:
				//LUT0'
				if(!(_registers->PORTMUX_CCLROUTEA & 0x1))
					return;
				_registers->CCL_LUT0CTRLA = 0;
				if(_registers->EVSYS_CCL_LUT0A > 0)
				{
					const uint8_t channel = _registers->EVSYS_CCL_LUT0A;
					_registers->EVSYS_CCL_LUT0A = 0;
					for(uint8_t i = 0; i < sizeof(_registers->EVSYS_CHANNELUSER); i++)
					{
						if(_registers->EVSYS_CHANNELUSER[i] == channel)
							return;
					}
					_registers->EVSYS_CHANNEL[channel - 1] = 0;
				}
				return;
			case 14:
				//LUT2'
				if(!(_registers->PORTMUX_CCLROUTEA & 0x4))
					return;
				_registers->CCL_LUT2CTRLA = 0;
				if(_registers->EVSYS_CCL_LUT2A > 0)
				{
					const uint8_t channel = _registers->EVSYS_CCL_LUT2A;
					_registers->EVSYS_CCL_LUT2A = 0;
					for(uint8_t i = 0; i < sizeof(_registers->EVSYS_CHANNELUSER); i++)
					{
						if(_registers->EVSYS_CHANNELUSER[i] == channel)
							return;
					}
					_registers->EVSYS_CHANNEL[channel - 1] = 0;
				}
				return;
			case 15:
				//EVOUTB'
				if(!(_registers->PORTMUX_EVSYSROUTEA & 0x2))
					return;
				if(_registers->EVSYS_EVOUTB > 0)
				{
					const uint8_t channel = _registers->EVSYS_EVOUTB;
					_registers->EVSYS_EVOUTB = 0;
					_registers->PORTB_PINCTRL[7] &= ~0x80;
					for(uint8_t i = 0; i < sizeof(_registers->EVSYS_CHANNELUSER); i++)
					{
						if(_registers->EVSYS_CHANNELUSER[i] == channel)
							return;
					}
					_registers->EVSYS_CHANNEL[channel - 1] = 0;
				}
				return;
			case 17:
				if(_registers->Comm == ATTiny427Expander_Comm_SPIAlternate || _registers->Comm == ATTiny427Expander_Comm_UART1Alternate)
					return;
				//LUT1'
				_registers->CCL_LUT1CTRLA = 0;
				if(_registers->EVSYS_CCL_LUT1A > 0)
				{
					const uint8_t channel = _registers->EVSYS_CCL_LUT1A;
					_registers->EVSYS_CCL_LUT1A = 0;
					for(uint8_t i = 0; i < sizeof(_registers->EVSYS_CHANNELUSER); i++)
					{
						if(_registers->EVSYS_CHANNELUSER[i] == channel)
							return;
					}
					_registers->EVSYS_CHANNEL[channel - 1] = 0;
				}
				return;
			case 18:
				if(_registers->Comm == ATTiny427Expander_Comm_SPIAlternate || _registers->Comm == ATTiny427Expander_Comm_UART1Alternate)
					return;
				//EVOUTC
				if(_registers->EVSYS_EVOUTC > 0)
				{
					const uint8_t channel = _registers->EVSYS_EVOUTC;
					_registers->EVSYS_EVOUTC = 0;
					_registers->PORTC_PINCTRL[2] &= ~0x80;
					for(uint8_t i = 0; i < sizeof(_registers->EVSYS_CHANNELUSER); i++)
					{
						if(_registers->EVSYS_CHANNELUSER[i] == channel)
							return;
					}
					_registers->EVSYS_CHANNEL[channel - 1] = 0;
				}
				return;
			case 20:
				//LUT3
				if(_registers->PORTMUX_CCLROUTEA & 0x8)
					return;
				if(_registers->EVSYS_CCL_LUT3A > 0)
				{
					const uint8_t channel = _registers->EVSYS_CCL_LUT3A;
					_registers->EVSYS_CCL_LUT3A = 0;
					for(uint8_t i = 0; i < sizeof(_registers->EVSYS_CHANNELUSER); i++)
					{
						if(_registers->EVSYS_CHANNELUSER[i] == channel)
							return;
					}
					_registers->EVSYS_CHANNEL[channel - 1] = 0;
				}
				return;
		}
	}
	void DigitalService_ATTiny427Expander::Update()
	{
		_previousPORTA_IN ^= _registers->PORTA_IN;
		_previousPORTB_IN ^= _registers->PORTB_IN;
		_previousPORTC_IN ^= _registers->PORTC_IN;

		for (DigitalInterruptList_ATTiny427Expander::iterator interrupt = _interruptList.begin(); interrupt != _interruptList.end(); ++interrupt)
		{
			const uint8_t DigitalPin = PinToDigitalPin(interrupt->DigitalPin);
			switch(interrupt->DigitalPort)
			{
				case PORTA: 
					if(_previousPORTA_IN & DigitalPin)
						interrupt->CallBack();
					break;
				case PORTB: 
					if(_previousPORTB_IN& DigitalPin)
						interrupt->CallBack();
					break;
				case PORTC: 
					if(_previousPORTC_IN & DigitalPin)
						interrupt->CallBack();
					break;
			}
		}

		_previousPORTA_IN = _registers->PORTA_IN;
		_previousPORTB_IN = _registers->PORTB_IN;
		_previousPORTC_IN = _registers->PORTC_IN;
	}
}
#endif
