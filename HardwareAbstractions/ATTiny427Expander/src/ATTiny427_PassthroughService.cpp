#include "ATTiny427_PassthroughService.h"
#include <cstring>
#include <algorithm>

#ifdef ATTINY427_PASSTHROUGHSERVICE_H
namespace EmbeddedIOServices
{
	ATTiny427_PassthroughService::ATTiny427_PassthroughService(ATTiny427_ExpanderService *aTTiny427ExpanderService, ATTiny427_EVSYSService *aTTiny427EVSYSService) :
		_aTTiny427ExpanderService(aTTiny427ExpanderService),
		_aTTiny427EVSYSService(aTTiny427EVSYSService),
		_PORTMUX_EVSYSROUTEA(_aTTiny427ExpanderService->GetRegister(ADDRESS_PORTMUX_EVSYSROUTEA)),
		_PORTMUX_CCLROUTEA(_aTTiny427ExpanderService->GetRegister(ADDRESS_PORTMUX_CCLROUTEA)),
		_PORTA_PINCTRL_2(_aTTiny427ExpanderService->GetRegister(ADDRESS_PORTA_PINCTRL(2))),
		_PORTA_PINCTRL_7(_aTTiny427ExpanderService->GetRegister(ADDRESS_PORTA_PINCTRL(7))),
		_PORTB_PINCTRL_2(_aTTiny427ExpanderService->GetRegister(ADDRESS_PORTB_PINCTRL(2))),
		_PORTB_PINCTRL_7(_aTTiny427ExpanderService->GetRegister(ADDRESS_PORTB_PINCTRL(7))),
		_PORTC_PINCTRL_2(_aTTiny427ExpanderService->GetRegister(ADDRESS_PORTC_PINCTRL(2))),
		_AC0_CTRLA(_aTTiny427ExpanderService->GetRegister(ADDRESS_AC0_CTRLA)),
		_CCL_CTRLA(_aTTiny427ExpanderService->GetRegister(ADDRESS_CCL_CTRLA)),
		_CCL_LUT0CTRLA(_aTTiny427ExpanderService->GetRegister(ADDRESS_CCL_LUT0CTRLA)),
		_CCL_LUT1CTRLA(_aTTiny427ExpanderService->GetRegister(ADDRESS_CCL_LUT1CTRLA)),
		_CCL_LUT2CTRLA(_aTTiny427ExpanderService->GetRegister(ADDRESS_CCL_LUT2CTRLA)),
		_CCL_LUT3CTRLA(_aTTiny427ExpanderService->GetRegister(ADDRESS_CCL_LUT3CTRLA))
	{
	}

	void ATTiny427_PassthroughService::_setupACPassthrough(digitalpin_t pinIn, digitalpin_t pinOut, bool inverted)
	{
		_aTTiny427ExpanderService->Write(ADDRESS_AC0_DACREF, 0x85);
		uint8_t AC_MUXCTRLA ;
		uint8_t AC_CTRLA = 0b10000111;
		switch(pinIn)
		{
			case 13:
				AC_MUXCTRLA = 0x03 | (0x1 << 3);
				break;
			case 9:
				AC_MUXCTRLA = 0x03 | (0x2 << 3);
				break;
			case 14:
				AC_MUXCTRLA = 0x03 | (0x3 << 3);
				break;
			default:
				return;
		}
		if(pinOut == 5)
		{
			AC_MUXCTRLA |= inverted? 0b10000000 : 0;
			AC_CTRLA |= 0b01000000; //enable output
		}
		_aTTiny427ExpanderService->Write(ADDRESS_AC0_MUXCTRLA, AC_MUXCTRLA);
		_AC0_CTRLA = AC_CTRLA;
	}

	bool ATTiny427_PassthroughService::InitPassthrough(digitalpin_t pinIn, digitalpin_t pinOut, bool inverted, bool useAC)
	{
		if(pinIn >= 22 || pinOut >= 22 || pinIn == pinOut)
			return false;
		if(!(pinIn == 7 || pinIn == 13 || pinIn == 9 || pinIn == 14))
			useAC = false;

		const ATTiny427_ExpanderComm comm = _aTTiny427ExpanderService->Comm;

		DeinitPassthrough(pinIn);
		switch(pinOut)
		{
			case 2:
				//EVOUTA
				if(comm == ATTiny427_ExpanderComm_SPI || comm == ATTiny427_ExpanderComm_UART0Alternate || comm == ATTiny427_ExpanderComm_UART1)
					return false;
				if(!(comm == ATTiny427_ExpanderComm_SPIAlternate || comm == ATTiny427_ExpanderComm_UART1Alternate))
					DeinitPassthrough(7);
				_PORTMUX_EVSYSROUTEA &= ~0x1;
				if(!_aTTiny427EVSYSService->InitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_EVOUTA, pinIn, useAC))
					return false;
				if(inverted)
					_PORTA_PINCTRL_2 |= 0x80;
				else 
					_PORTA_PINCTRL_2 &= ~0x80;
				if(useAC) _setupACPassthrough(pinIn, pinOut, inverted);
				return true;
			case 4:
				//LUT0
				if(comm == ATTiny427_ExpanderComm_SPI)
					return false;
				DeinitPassthrough(12);
				if(useAC)
				{
					_setupACPassthrough(pinIn, pinOut, inverted);
					_CCL_CTRLA &= ~0x01;
					_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT0CTRLB, 0x66);
					_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT0CTRLC, 0x00);
				}
				else
				{
					switch(pinIn)
					{
						case 0: 
							_CCL_CTRLA &= ~0x01;
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT0CTRLB, 0x05);
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT0CTRLC, 0x00);
							break;
						case 1:
							_CCL_CTRLA &= ~0x01;
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT0CTRLB, 0x50);
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT0CTRLC, 0x00);
							break;
						case 2:
							_CCL_CTRLA &= ~0x01;
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT0CTRLB, 0x00);
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT0CTRLC, 0x05);
							break;
						default:
							if(!_aTTiny427EVSYSService->InitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_CCL_LUT0A, pinIn))
								return false;
							_CCL_CTRLA &= ~0x01;
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT0CTRLB, 0x03);
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT0CTRLC, 0x00);
							break;
					}
				}
				_aTTiny427ExpanderService->Write(ADDRESS_CCL_TRUTH0, inverted? 0x1 : 0xFE);
				_PORTMUX_CCLROUTEA &= ~0x1;
				_CCL_LUT0CTRLA = 0x41;
				_CCL_CTRLA = 0x41;
				return true;
			case 5:
				//otherwise use LUT3', if LUT3 was already being used, it will be overwritten
				DeinitPassthrough(20);
				if(useAC) 
				{
					_setupACPassthrough(pinIn, pinOut, inverted);
					return true;
				}
				switch(pinIn)
				{
					case 16: 
						_CCL_CTRLA &= ~0x01;
						_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT3CTRLB, 0x05);
						_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT3CTRLC, 0x00);
						break;
					case 17:
						_CCL_CTRLA &= ~0x01;
						_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT3CTRLB, 0x50);
						_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT3CTRLC, 0x00);
						break;
					case 18:
						_CCL_CTRLA &= ~0x01;
						_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT3CTRLB, 0x00);
						_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT3CTRLC, 0x05);
						break;
					default:
						if(!_aTTiny427EVSYSService->InitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_CCL_LUT3A, pinIn))
							return false;
						_CCL_CTRLA &= ~0x01;
						_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT3CTRLB, 0x03);
						_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT3CTRLC, 0x00);
						break;
				}
				_aTTiny427ExpanderService->Write(ADDRESS_CCL_TRUTH3, inverted? 0x1 : 0xFE);
				_PORTMUX_CCLROUTEA |= 0x8;
				_CCL_LUT3CTRLA = 0x41;
				_CCL_CTRLA = 0x41;
				return true;
			case 7:
				if(comm == ATTiny427_ExpanderComm_SPIAlternate || comm == ATTiny427_ExpanderComm_UART1Alternate)
				{
					//LUT1
					DeinitPassthrough(17);
					if(useAC)
					{
						_setupACPassthrough(pinIn, pinOut, inverted);
						_CCL_CTRLA &= ~0x01;
						_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT1CTRLB, 0x66);
						_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT1CTRLC, 0x00);
					}
					else
					{
						switch(pinIn)
						{
							case 19: 
								_CCL_CTRLA &= ~0x01;
								_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT1CTRLB, 0x05);
								_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT1CTRLC, 0x00);
								break;
							case 20:
								_CCL_CTRLA &= ~0x01;
								_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT1CTRLB, 0x50);
								_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT1CTRLC, 0x00);
								break;
							case 21:
								_CCL_CTRLA &= ~0x01;
								_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT1CTRLB, 0x00);
								_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT1CTRLC, 0x05);
								break;
							default:
								if(!_aTTiny427EVSYSService->InitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_CCL_LUT1A, pinIn))
									return false;
								_CCL_CTRLA &= ~0x01;
								_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT1CTRLB, 0x03);
								_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT1CTRLC, 0x00);
								break;
						}
					}
					_aTTiny427ExpanderService->Write(ADDRESS_CCL_TRUTH1, inverted? 0x1 : 0xFE);
					_PORTMUX_CCLROUTEA &= ~0x2;
					_CCL_LUT1CTRLA = 0x41;
					_CCL_CTRLA = 0x41;
					return true;
				}
				//EVOUTA'
				DeinitPassthrough(2);
				_PORTMUX_EVSYSROUTEA |= 0x1;
				if(!_aTTiny427EVSYSService->InitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_EVOUTA, pinIn, useAC))
					return false;
				if(inverted)
					_PORTA_PINCTRL_7 |= 0x80;
				else 
					_PORTA_PINCTRL_7 &= ~0x80;
				if(useAC) _setupACPassthrough(pinIn, pinOut, inverted);
				return true;
			case 10:
				//EVOUTB
				DeinitPassthrough(15);
				_PORTMUX_EVSYSROUTEA &= ~0x2;
				if(!_aTTiny427EVSYSService->InitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_EVOUTB, pinIn, useAC))
					return false;
				if(inverted)
					_PORTB_PINCTRL_2 |= 0x80;
				else 
					_PORTB_PINCTRL_2 &= ~0x80;
				if(useAC) _setupACPassthrough(pinIn, pinOut, inverted);
				return true;
			case 11:
				//LUT2
				DeinitPassthrough(14);
				if(useAC)
				{
					_setupACPassthrough(pinIn, pinOut, inverted);
					_CCL_CTRLA &= ~0x01;
					_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT2CTRLB, 0x66);
					_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT2CTRLC, 0x00);
				}
				else
				{
					switch(pinIn)
					{
						case 8: 
							_CCL_CTRLA &= ~0x01;
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT2CTRLB, 0x05);
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT2CTRLC, 0x00);
							break;
						case 9:
							_CCL_CTRLA &= ~0x01;
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT2CTRLB, 0x50);
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT2CTRLC, 0x00);
							break;
						case 10:
							_CCL_CTRLA &= ~0x01;
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT2CTRLB, 0x00);
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT2CTRLC, 0x05);
							break;
						default:
							if(!_aTTiny427EVSYSService->InitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_CCL_LUT2A, pinIn))
								return false;
							_CCL_CTRLA &= ~0x01;
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT2CTRLB, 0x03);
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT2CTRLC, 0x00);
							break;
					}
				}
				_aTTiny427ExpanderService->Write(ADDRESS_CCL_TRUTH2, inverted? 0x1 : 0xFE);
				_PORTMUX_CCLROUTEA &= ~0x4;
				_CCL_LUT2CTRLA = 0x41;
				_CCL_CTRLA = 0x41;
				return true;
			case 12:
				//LUT0'
				DeinitPassthrough(4);
				if(useAC)
				{
					_setupACPassthrough(pinIn, pinOut, inverted);
					_CCL_CTRLA &= ~0x01;
					_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT0CTRLB, 0x66);
					_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT0CTRLC, 0x00);
				}
				else
				{
					switch(pinIn)
					{
						case 0: 
							_CCL_CTRLA &= ~0x01;
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT0CTRLB, 0x05);
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT0CTRLC, 0x00);
							break;
						case 1:
							_CCL_CTRLA &= ~0x01;
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT0CTRLB, 0x50);
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT0CTRLC, 0x00);
							break;
						case 2:
							_CCL_CTRLA &= ~0x01;
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT0CTRLB, 0x00);
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT0CTRLC, 0x05);
							break;
						default:
							if(!_aTTiny427EVSYSService->InitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_CCL_LUT0A, pinIn))
								return false;
							_CCL_CTRLA &= ~0x01;
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT0CTRLB, 0x03);
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT0CTRLC, 0x00);
							break;
					}
				}
				_aTTiny427ExpanderService->Write(ADDRESS_CCL_TRUTH0, inverted? 0x1 : 0xFE);
				_PORTMUX_CCLROUTEA |= 0x1;
				_CCL_LUT0CTRLA = 0x41;
				_CCL_CTRLA = 0x41;
				return true;
			case 14:
				//LUT2'
				DeinitPassthrough(11);
				if(useAC)
				{
					_setupACPassthrough(pinIn, pinOut, inverted);
					_CCL_CTRLA &= ~0x01;
					_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT2CTRLB, 0x66);
					_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT2CTRLC, 0x00);
				}
				else
				{
					switch(pinIn)
					{
						case 8: 
							_CCL_CTRLA &= ~0x01;
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT2CTRLB, 0x05);
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT2CTRLC, 0x00);
							break;
						case 9:
							_CCL_CTRLA &= ~0x01;
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT2CTRLB, 0x50);
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT2CTRLC, 0x00);
							break;
						case 10:
							_CCL_CTRLA &= ~0x01;
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT2CTRLB, 0x00);
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT2CTRLC, 0x05);
							break;
						default:
							if(!_aTTiny427EVSYSService->InitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_CCL_LUT2A, pinIn))
								return false;
							_CCL_CTRLA &= ~0x01;
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT2CTRLB, 0x03);
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT2CTRLC, 0x00);
							break;
					}
				}
				_aTTiny427ExpanderService->Write(ADDRESS_CCL_TRUTH2, inverted? 0x1 : 0xFE);
				_PORTMUX_CCLROUTEA |= 0x4;
				_CCL_LUT2CTRLA = 0x41;
				_CCL_CTRLA = 0x41;
				return true;
			case 15:
				//EVOUTB'
				DeinitPassthrough(10);
				_PORTMUX_EVSYSROUTEA |= 0x2;
				if(!_aTTiny427EVSYSService->InitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_EVOUTB, pinIn, useAC))
					return false;
				if(inverted)
					_PORTB_PINCTRL_7 |= 0x80;
				else 
					_PORTB_PINCTRL_7 &= ~0x80;
				if(useAC) _setupACPassthrough(pinIn, pinOut, inverted);
				return true;
			case 17:
				//LUT1'
				if(comm == ATTiny427_ExpanderComm_SPIAlternate || comm == ATTiny427_ExpanderComm_UART1Alternate)
					return false;
				if(useAC)
				{
					_setupACPassthrough(pinIn, pinOut, inverted);
					_CCL_CTRLA &= ~0x01;
					_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT1CTRLB, 0x66);
					_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT1CTRLC, 0x00);
				}
				else
				{
					switch(pinIn)
					{
						case 19: 
							_CCL_CTRLA &= ~0x01;
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT1CTRLB, 0x05);
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT1CTRLC, 0x00);
							break;
						case 20:
							_CCL_CTRLA &= ~0x01;
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT1CTRLB, 0x50);
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT1CTRLC, 0x00);
							break;
						case 21:
							_CCL_CTRLA &= ~0x01;
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT1CTRLB, 0x00);
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT1CTRLC, 0x05);
							break;
						default:
							if(!_aTTiny427EVSYSService->InitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_CCL_LUT1A, pinIn))
								return false;
							_CCL_CTRLA &= ~0x01;
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT1CTRLB, 0x03);
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT1CTRLC, 0x00);
							break;
					}
				}
				_aTTiny427ExpanderService->Write(ADDRESS_CCL_TRUTH1, inverted? 0x1 : 0xFE);
				_PORTMUX_CCLROUTEA |= 0x2;
				_CCL_LUT1CTRLA = 0x41;
				_CCL_CTRLA = 0x41;
				return true;
			case 18:
				if(comm == ATTiny427_ExpanderComm_SPIAlternate || comm == ATTiny427_ExpanderComm_UART1Alternate)
					return false;
				//EVOUTC
				if(!_aTTiny427EVSYSService->InitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_EVOUTC, pinIn, useAC))
					return false;
				if(inverted)
					_PORTC_PINCTRL_2 |= 0x80;
				else 
					_PORTC_PINCTRL_2 &= ~0x80;
				if(useAC) _setupACPassthrough(pinIn, pinOut, inverted);
				return true;
			case 20:
				//LUT3
				DeinitPassthrough(5);
				if(useAC)
				{
					_setupACPassthrough(pinIn, pinOut, inverted);
					_CCL_CTRLA &= ~0x01;
					_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT3CTRLB, 0x66);
					_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT3CTRLC, 0x00);
				}
				else
				{
					switch(pinIn)
					{
						case 16: 
							_CCL_CTRLA &= ~0x01;
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT3CTRLB, 0x05);
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT3CTRLC, 0x00);
							break;
						case 17:
							_CCL_CTRLA &= ~0x01;
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT3CTRLB, 0x50);
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT3CTRLC, 0x00);
							break;
						case 18:
							_CCL_CTRLA &= ~0x01;
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT3CTRLB, 0x00);
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT3CTRLC, 0x05);
							break;
						default:
							if(!_aTTiny427EVSYSService->InitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_CCL_LUT3A, pinIn))
								return false;
							_CCL_CTRLA &= ~0x01;
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT3CTRLB, 0x03);
							_aTTiny427ExpanderService->Write(ADDRESS_CCL_LUT3CTRLC, 0x00);
							break;
					}
				}
				_aTTiny427ExpanderService->Write(ADDRESS_CCL_TRUTH3, inverted? 0x1 : 0xFE);
				_PORTMUX_CCLROUTEA &= ~0x8;
				_CCL_LUT3CTRLA = 0x41;
				_CCL_CTRLA = 0x41;
				return true;
		}
		return false;
	}

	void ATTiny427_PassthroughService::DeinitPassthrough(digitalpin_t pinOut)
	{
		const ATTiny427_ExpanderComm comm = _aTTiny427ExpanderService->Comm;

		switch(pinOut)
		{
			case 2:
				if(comm == ATTiny427_ExpanderComm_SPI || comm == ATTiny427_ExpanderComm_UART0Alternate || comm == ATTiny427_ExpanderComm_UART1)
					return;
				//EVOUTA
				if(!_aTTiny427EVSYSService->EVSYS_EVOUTA->IsDataValid() || _PORTMUX_EVSYSROUTEA & 0x1 || *_aTTiny427EVSYSService->EVSYS_EVOUTA == 0)
					return;
				_aTTiny427EVSYSService->DeinitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_EVOUTA);
				_PORTA_PINCTRL_2 &= ~0x80;
				return;
			case 4:
				if(comm == ATTiny427_ExpanderComm_SPI)
					return;
				//LUT0
				if(!_CCL_LUT0CTRLA.IsDataValid() || _PORTMUX_CCLROUTEA & 0x1 || _CCL_LUT0CTRLA == 0)
					return;
				_CCL_CTRLA &= ~0x1;
				_CCL_LUT0CTRLA = 0;
				_aTTiny427EVSYSService->DeinitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_CCL_LUT0A);
				_CCL_CTRLA |= (_CCL_LUT1CTRLA.IsDataValid() && _CCL_LUT1CTRLA != 0)? 0x01 :  (_CCL_LUT2CTRLA.IsDataValid() && _CCL_LUT2CTRLA != 0)? 0x01 : (_CCL_LUT3CTRLA.IsDataValid() && _CCL_LUT3CTRLA != 0)? 0x01 : 0x00;
				return;
			case 5:
				//disable AC OUT
				if(_AC0_CTRLA.IsDataValid())
					_AC0_CTRLA &= ~0b01000000;
				//LUT3'
				if(!_CCL_LUT3CTRLA.IsDataValid() || !(_PORTMUX_CCLROUTEA & 0x8) || _CCL_LUT3CTRLA == 0)
					return;
				_CCL_CTRLA &= ~0x1;
				_CCL_LUT3CTRLA = 0;
				_aTTiny427EVSYSService->DeinitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_CCL_LUT3A);
				_CCL_CTRLA |= _CCL_LUT0CTRLA.IsDataValid() && _CCL_LUT0CTRLA != 0 ? 0x01 :  _CCL_LUT1CTRLA.IsDataValid() && _CCL_LUT1CTRLA != 0 ? 0x01 : _CCL_LUT2CTRLA.IsDataValid() && _CCL_LUT2CTRLA != 0 ? 0x01 : 0x00;
				return;
			case 7:
				if(comm == ATTiny427_ExpanderComm_SPIAlternate || comm == ATTiny427_ExpanderComm_UART1Alternate)
				{
					//LUT1
					if(!_CCL_LUT1CTRLA.IsDataValid() || _PORTMUX_CCLROUTEA & 0x2 || _CCL_LUT1CTRLA == 0)
						return;
					_CCL_CTRLA &= ~0x1;
					_CCL_LUT1CTRLA = 0;
					_aTTiny427EVSYSService->DeinitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_CCL_LUT1A);
					_CCL_CTRLA |= _CCL_LUT0CTRLA.IsDataValid() && _CCL_LUT0CTRLA != 0 ? 0x01 :  _CCL_LUT2CTRLA.IsDataValid() && _CCL_LUT2CTRLA != 0 ? 0x01 : _CCL_LUT3CTRLA.IsDataValid() && _CCL_LUT3CTRLA != 0 ? 0x01 : 0x00;
					return;
				}
				//EVOUTA'
				if(!_aTTiny427EVSYSService->EVSYS_EVOUTA->IsDataValid() || !(_PORTMUX_EVSYSROUTEA & 0x1) || *_aTTiny427EVSYSService->EVSYS_EVOUTA == 0)
					return;
				_aTTiny427EVSYSService->DeinitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_EVOUTA);
				_PORTA_PINCTRL_7 &= ~0x80;
				return;
			case 10:
				//EVOUTB
				if(!_aTTiny427EVSYSService->EVSYS_EVOUTB->IsDataValid() || _PORTMUX_EVSYSROUTEA & 0x2 || _aTTiny427EVSYSService->EVSYS_EVOUTB == 0)
					return;
				_aTTiny427EVSYSService->DeinitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_EVOUTB);
				_PORTB_PINCTRL_2 &= ~0x80;
				return;
			case 11:
				//LUT2
				if(!_CCL_LUT2CTRLA.IsDataValid() || _PORTMUX_CCLROUTEA & 0x4 || _CCL_LUT2CTRLA == 0)
					return;
				_CCL_CTRLA &= ~0x1;
				_CCL_LUT2CTRLA = 0;
				_aTTiny427EVSYSService->DeinitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_CCL_LUT2A);
				_CCL_CTRLA |= _CCL_LUT0CTRLA.IsDataValid() && _CCL_LUT0CTRLA != 0 ? 0x01 :  _CCL_LUT1CTRLA.IsDataValid() && _CCL_LUT1CTRLA != 0 ? 0x01 : _CCL_LUT3CTRLA.IsDataValid() && _CCL_LUT3CTRLA != 0 ? 0x01 : 0x00;
				return;
			case 12:
				//LUT0'
				if(!_CCL_LUT0CTRLA.IsDataValid() || !(_PORTMUX_CCLROUTEA & 0x1) || _CCL_LUT0CTRLA == 0)
					return;
				_CCL_CTRLA &= ~0x1;
				_CCL_LUT0CTRLA = 0;
				_aTTiny427EVSYSService->DeinitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_CCL_LUT0A);
				_CCL_CTRLA |= _CCL_LUT1CTRLA.IsDataValid() && _CCL_LUT1CTRLA != 0 ? 0x01 :  _CCL_LUT2CTRLA.IsDataValid() && _CCL_LUT2CTRLA != 0 ? 0x01 : _CCL_LUT3CTRLA.IsDataValid() && _CCL_LUT3CTRLA != 0 ? 0x01 : 0x00;
				return;
			case 14:
				//LUT2'
				if(!_CCL_LUT2CTRLA.IsDataValid() || !(_PORTMUX_CCLROUTEA & 0x4) || _CCL_LUT2CTRLA == 0)
					return;
				_CCL_CTRLA &= ~0x1;
				_CCL_LUT2CTRLA = 0;
				_aTTiny427EVSYSService->DeinitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_CCL_LUT2A);
				_CCL_CTRLA |= _CCL_LUT0CTRLA.IsDataValid() && _CCL_LUT0CTRLA != 0 ? 0x01 :  _CCL_LUT1CTRLA.IsDataValid() && _CCL_LUT1CTRLA != 0 ? 0x01 : _CCL_LUT3CTRLA.IsDataValid() && _CCL_LUT3CTRLA != 0 ? 0x01 : 0x00;
				return;
			case 15:
				//EVOUTB'
				if(!_aTTiny427EVSYSService->EVSYS_EVOUTB->IsDataValid() || !(_PORTMUX_EVSYSROUTEA & 0x2) || *_aTTiny427EVSYSService->EVSYS_EVOUTB == 0)
					return;
				_aTTiny427EVSYSService->DeinitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_EVOUTB);
				_PORTB_PINCTRL_7 &= ~0x80;
				return;
			case 17:
				if(comm == ATTiny427_ExpanderComm_SPIAlternate || comm == ATTiny427_ExpanderComm_UART1Alternate)
					return;
				//LUT1'
				if(!_CCL_LUT1CTRLA.IsDataValid() || !(_PORTMUX_CCLROUTEA & 0x2) || _CCL_LUT1CTRLA == 0)
					return;
				_CCL_CTRLA &= ~0x1;
				_CCL_LUT1CTRLA = 0;
				_aTTiny427EVSYSService->DeinitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_CCL_LUT1A);
				_CCL_CTRLA |= _CCL_LUT0CTRLA.IsDataValid() && _CCL_LUT0CTRLA != 0 ? 0x01 :  _CCL_LUT2CTRLA.IsDataValid() && _CCL_LUT2CTRLA != 0 ? 0x01 : _CCL_LUT3CTRLA.IsDataValid() && _CCL_LUT3CTRLA != 0 ? 0x01 : 0x00;
				return;
			case 18:
				if(comm == ATTiny427_ExpanderComm_SPIAlternate || comm == ATTiny427_ExpanderComm_UART1Alternate)
					return;
				if(!_aTTiny427EVSYSService->EVSYS_EVOUTC->IsDataValid() || *_aTTiny427EVSYSService->EVSYS_EVOUTC == 0)
					return;
				_aTTiny427EVSYSService->DeinitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_EVOUTC);
				_PORTC_PINCTRL_2 &= ~0x80;
				return;
			case 20:
				//LUT3
				if(!_CCL_LUT3CTRLA.IsDataValid() || _PORTMUX_CCLROUTEA & 0x8 || _CCL_LUT3CTRLA == 0)
					return;
				_CCL_CTRLA &= ~0x1;
				_CCL_LUT3CTRLA = 0;
				_aTTiny427EVSYSService->DeinitEVSYSUser(*_aTTiny427EVSYSService->EVSYS_CCL_LUT3A);
				_CCL_CTRLA |= _CCL_LUT0CTRLA.IsDataValid() && _CCL_LUT0CTRLA != 0 ? 0x01 :  _CCL_LUT1CTRLA.IsDataValid() && _CCL_LUT1CTRLA != 0 ? 0x01 : _CCL_LUT2CTRLA.IsDataValid() && _CCL_LUT2CTRLA != 0 ? 0x01 : 0x00;
				return;
		}
	}
}
#endif
