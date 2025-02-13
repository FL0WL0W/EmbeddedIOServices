#include "stddef.h"
#include "stdint.h"
#include "IDigitalService.h"
#include <forward_list>

#ifndef ATTINY427EXPANDERUPDATESERVICE_H
#define ATTINY427EXPANDERUPDATESERVICE_H
namespace EmbeddedIOServices
{
	enum ATTiny427Expander_Comm
	{
		ATTiny427Expander_Comm_UART0,
		ATTiny427Expander_Comm_UART0Alternate,
		ATTiny427Expander_Comm_UART1,
		ATTiny427Expander_Comm_UART1Alternate,
		ATTiny427Expander_Comm_SPI,
		ATTiny427Expander_Comm_SPIAlternate
	};

	struct ATTiny427Expander_Registers 
	{
		ATTiny427Expander_Comm Comm;

		//registers
		uint8_t PORTA_DIR = 0;
		uint8_t PORTA_OUT = 0;
		uint8_t PORTA_IN = 0;
		uint8_t PORTB_DIR = 0;
		uint8_t PORTB_OUT = 0;
		uint8_t PORTB_IN = 0;
		uint8_t PORTC_DIR = 0;
		uint8_t PORTC_OUT = 0;
		uint8_t PORTC_IN = 0;

		uint8_t PORTA_PINCTRL[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
		uint8_t PORTB_PINCTRL[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
		uint8_t PORTC_PINCTRL[6] = { 0, 0, 0, 0, 0, 0 };
		union
		{
			uint16_t AnalogEnable = 0;
			struct
			{
				uint8_t GPIOR0;
				uint8_t GPIOR1;
			};
		};
		union
		{
			uint8_t GPIOR2 = 0;
			struct
			{
				uint8_t AnalogAccumulate : 4;
				bool SENTenableTCB0 : 1;
				bool SENTenableTCB1 : 1;
				bool GPIOR2_RESERVED : 1;
				bool AnalogAccumulateReset : 1;
			};
			
		};
		uint8_t AC_CTRLA = 0;
		uint8_t AC_MUXCTRLA = 0;
		uint8_t AC_DACREF = 0xFF;
		uint8_t PORTMUX_EVSYSROUTEA = 0;
		uint8_t PORTMUX_CCLROUTEA = 0;
		uint8_t EVSYS_CHANNEL[6] = { 0, 0, 0, 0, 0, 0 };
		union
		{
			uint8_t EVSYS_CHANNELUSER[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			struct
			{
				uint8_t EVSYS_CCL_LUT0A;
				uint8_t EVSYS_CCL_LUT1A;
				uint8_t EVSYS_CCL_LUT2A;
				uint8_t EVSYS_CCL_LUT3A;
				uint8_t EVSYS_EVOUTA;
				uint8_t EVSYS_EVOUTB;
				uint8_t EVSYS_EVOUTC;
				uint8_t EVSYS_TCB0_CAPT;
				uint8_t EVSYS_TCB1_CAPT;
			};
		};

		uint8_t CCL_CTRLA = 0;
		uint8_t CCL_LUT0CTRLA = 0;
		uint8_t CCL_LUT0CTRLB = 0;
		uint8_t CCL_LUT0CTRLC = 0;
		uint8_t CCL_LUT0TRUTH = 0;
		uint8_t CCL_LUT1CTRLA = 0;
		uint8_t CCL_LUT1CTRLB = 0;
		uint8_t CCL_LUT1CTRLC = 0;
		uint8_t CCL_LUT1TRUTH = 0;
		uint8_t CCL_LUT2CTRLA = 0;
		uint8_t CCL_LUT2CTRLB = 0;
		uint8_t CCL_LUT2CTRLC = 0;
		uint8_t CCL_LUT2TRUTH = 0;
		uint8_t CCL_LUT3CTRLA = 0;
		uint8_t CCL_LUT3CTRLB = 0;
		uint8_t CCL_LUT3CTRLC = 0;
		uint8_t CCL_LUT3TRUTH = 0;

		uint8_t TCA_CTRLA = 0;
		uint8_t TCA_CTRLB = 0;
		uint8_t TCA_CTRLC = 0;
		uint8_t TCA_CTRLD = 0;
		uint16_t TCA_PERBUF = 0xFFFF;
		uint16_t TCA_CMP0BUF = 0;
		uint16_t TCA_CMP1BUF = 0;
		uint16_t TCA_CMP2BUF = 0;

		//SRAM
		uint8_t AnalogCounter = 0;
		uint16_t AnalogValues[15];
		uint8_t TCB0Counter = 0;
		union
		{
			struct
			{
				uint16_t TCB0CNT;
				uint16_t TCB0CCMP;
			};
			struct
			{
				uint8_t TCB0SENTSTATUS : 4;
				uint16_t TCB0SENTSIGNAL1 : 12;
				uint16_t TCB0SENTSIGNAL2 : 12;
				uint8_t TCB0SENTCRC : 4;
			};
			uint8_t TCB0VALUE[4];
		};
		uint8_t TCB1Counter = 0;
		union
		{
			struct
			{
				uint16_t TCB1CNT;
				uint16_t TCB1CCMP;
			};
			struct
			{
				uint8_t TCB1SENTSTATUS : 4;
				uint16_t TCB1SENTSIGNAL1 : 12;
				uint16_t TCB1SENTSIGNAL2 : 12;
				uint8_t TCB1SENTCRC : 4;
			};
			uint8_t TCB1VALUE[4];
		};
		
		ATTiny427Expander_Registers(ATTiny427Expander_Comm comm) : Comm(comm)
		{
			switch (Comm)
			{
				case ATTiny427Expander_Comm_UART0:
					PORTB_DIR = 0b00000100;
					break;
				case ATTiny427Expander_Comm_UART0Alternate:
				case ATTiny427Expander_Comm_UART1:
					PORTA_DIR = 0b00000010;
					break;
				case ATTiny427Expander_Comm_UART1Alternate:
					PORTC_DIR = 0b00000100;
					break;
				case ATTiny427Expander_Comm_SPI:
					PORTA_DIR = 0b00000100;
					break;
				case ATTiny427Expander_Comm_SPIAlternate:
					PORTC_DIR = 0b00000010;
					break;
			}
		}

		ATTiny427Expander_Registers() : ATTiny427Expander_Registers(ATTiny427Expander_Comm_UART0)
		{
		}

		void ReassignChannel(uint8_t from, uint8_t to)
		{
			//TODO reassign channels
			EVSYS_CHANNEL[to & 0x7] = 0x4 | (to >> 3);
			for(uint8_t i = 0; i < sizeof(EVSYS_CHANNELUSER); i++)
			{
				if(EVSYS_CHANNELUSER[i] == from)
					EVSYS_CHANNELUSER[i] = (to & 0x7) + 1;
			}
			EVSYS_CHANNEL[from] = 0;
		}
		int8_t GetEVSYSChannel(uint8_t inputPin, int8_t depth = 0)
		{
			if(inputPin < 8)
			{
				if(EVSYS_CHANNEL[0] == 0 || EVSYS_CHANNEL[0] == ((inputPin & 0x7) | 0x40))
					return ((inputPin & 0x7) << 3) | 0;
				if(EVSYS_CHANNEL[1] == 0 || EVSYS_CHANNEL[1] == ((inputPin & 0x7) | 0x40))
					return ((inputPin & 0x7) << 3) | 1;
				if(EVSYS_CHANNEL[2] == 0 || EVSYS_CHANNEL[2] == ((inputPin & 0x7) | 0x48))
					return (((inputPin & 0x7) | 0x8) << 3) | 2;
				if(EVSYS_CHANNEL[3] == 0 || EVSYS_CHANNEL[3] == ((inputPin & 0x7) | 0x48))
					return (((inputPin & 0x7) | 0x8) << 3) | 3;
				if(depth > 1)
					return -1;
				const int8_t channel0ReassignTo = GetEVSYSChannel(EVSYS_CHANNEL[0] & 0xF, depth + 1);
				if(channel0ReassignTo != -1)
				{
					ReassignChannel(0, channel0ReassignTo);
					return GetEVSYSChannel(inputPin, depth + 1);
				}
				const int8_t channel1ReassignTo = GetEVSYSChannel(EVSYS_CHANNEL[1] & 0xF, depth + 1);
				if(channel1ReassignTo != -1)
				{
					ReassignChannel(1, channel1ReassignTo);
					return GetEVSYSChannel(inputPin, depth + 1);
				}
				const int8_t channel2ReassignTo = GetEVSYSChannel(EVSYS_CHANNEL[2] < 0x48? ((EVSYS_CHANNEL[2] & 0x7) + 16) : (EVSYS_CHANNEL[2] & 0x7), depth + 1);
				if(channel2ReassignTo != -1)
				{
					ReassignChannel(2, channel2ReassignTo);
					return GetEVSYSChannel(inputPin, depth + 1);
				}
				const int8_t channel3ReassignTo = GetEVSYSChannel(EVSYS_CHANNEL[3] < 0x48? ((EVSYS_CHANNEL[3] & 0x7) + 16) : (EVSYS_CHANNEL[3] & 0x7), depth + 1);
				if(channel3ReassignTo != -1)
				{
					ReassignChannel(3, channel3ReassignTo);
					return GetEVSYSChannel(inputPin, depth + 1);
				}
				return -1;
			}
			if(inputPin < 16)
			{
				if(EVSYS_CHANNEL[4] == 0 || EVSYS_CHANNEL[4] == ((inputPin & 0x7) | 0x40))
					return ((inputPin & 0x7) << 3) | 4;
				if(EVSYS_CHANNEL[5] == 0 || EVSYS_CHANNEL[5] == ((inputPin & 0x7) | 0x40))
					return ((inputPin & 0x7) << 3) | 5;
				if(EVSYS_CHANNEL[0] == 0 || EVSYS_CHANNEL[0] == ((inputPin & 0x7) | 0x48))
					return (((inputPin & 0x7) + 0x8) << 3) | 0;
				if(EVSYS_CHANNEL[1] == 0 || EVSYS_CHANNEL[1] == ((inputPin & 0x7) | 0x48))
					return (((inputPin & 0x7) + 0x8) << 3) | 1;
				const int8_t channel4ReassignTo = GetEVSYSChannel((EVSYS_CHANNEL[4] & 0xF) + 8, depth + 1);
				if(channel4ReassignTo != -1)
				{
					ReassignChannel(4, channel4ReassignTo);
					return GetEVSYSChannel(inputPin, depth + 1);
				}
				const int8_t channel5ReassignTo = GetEVSYSChannel((EVSYS_CHANNEL[5] & 0xF) + 8, depth + 1);
				if(channel5ReassignTo != -1)
				{
					ReassignChannel(5, channel5ReassignTo);
					return GetEVSYSChannel(inputPin, depth + 1);
				}
				const int8_t channel0ReassignTo = GetEVSYSChannel(EVSYS_CHANNEL[0] & 0xF, depth + 1);
				if(channel0ReassignTo != -1)
				{
					ReassignChannel(0, channel0ReassignTo);
					return GetEVSYSChannel(inputPin, depth + 1);
				}
				const int8_t channel1ReassignTo = GetEVSYSChannel(EVSYS_CHANNEL[1] & 0xF, depth + 1);
				if(channel1ReassignTo != -1)
				{
					ReassignChannel(1, channel1ReassignTo);
					return GetEVSYSChannel(inputPin, depth + 1);
				}
				return -1;
			}
			if(inputPin < 24)
			{
				if(EVSYS_CHANNEL[2] == 0 || EVSYS_CHANNEL[2] == ((inputPin & 0x7) | 0x40))
					return ((inputPin & 0x7) << 3) | 2;
				if(EVSYS_CHANNEL[3] == 0 || EVSYS_CHANNEL[3] == ((inputPin & 0x7) | 0x40))
					return ((inputPin & 0x7) << 3) | 3;
				if(EVSYS_CHANNEL[4] == 0 || EVSYS_CHANNEL[4] == ((inputPin & 0x7) | 0x48))
					return (((inputPin & 0x7) + 0x8) << 3) | 4;
				if(EVSYS_CHANNEL[5] == 0 || EVSYS_CHANNEL[5] == ((inputPin & 0x7) | 0x48))
					return (((inputPin & 0x7) + 0x8) << 3) | 5;
				const int8_t channel2ReassignTo = GetEVSYSChannel(EVSYS_CHANNEL[2] < 0x48? ((EVSYS_CHANNEL[2] & 0x7) + 16) : (EVSYS_CHANNEL[2] & 0x7), depth + 1);
				if(channel2ReassignTo != -1)
				{
					ReassignChannel(2, channel2ReassignTo);
					return GetEVSYSChannel(inputPin, depth + 1);
				}
				const int8_t channel3ReassignTo = GetEVSYSChannel(EVSYS_CHANNEL[3] < 0x48? ((EVSYS_CHANNEL[3] & 0x7) + 16) : (EVSYS_CHANNEL[3] & 0x7), depth + 1);
				if(channel3ReassignTo != -1)
				{
					ReassignChannel(3, channel3ReassignTo);
					return GetEVSYSChannel(inputPin, depth + 1);
				}
				const int8_t channel4ReassignTo = GetEVSYSChannel((EVSYS_CHANNEL[4] & 0xF) + 8, depth + 1);
				if(channel4ReassignTo != -1)
				{
					ReassignChannel(4, channel4ReassignTo);
					return GetEVSYSChannel(inputPin, depth + 1);
				}
				const int8_t channel5ReassignTo = GetEVSYSChannel((EVSYS_CHANNEL[5] & 0xF) + 8, depth + 1);
				if(channel5ReassignTo != -1)
				{
					ReassignChannel(5, channel5ReassignTo);
					return GetEVSYSChannel(inputPin, depth + 1);
				}
				return -1;
			}
			return -1;
		}
	};

	class ATTiny427ExpanderUpdateService
	{
		ATTiny427Expander_Registers *_registers;
		ATTiny427Expander_Registers _previousRegisters;
		bool _first = true;
		bool _analogRunning = false;
		uint8_t _receiveState = 0;
		uint8_t _receiveIndex = 0;
		uint8_t _receiveLength = 0;
		uint16_t _receiveAddress = 0;
		union
		{
			struct
			{
				uint8_t _receiveAnalogCounter;
				uint8_t _receiveAnalogValueLowByte;
				uint16_t _receiveAnalogEnable;
			};
			struct
			{
				uint8_t _receiveTcbCounter;
				uint8_t _receiveTcbBuffer[3];
			};
		};
	public:
		ATTiny427ExpanderUpdateService(ATTiny427Expander_Registers *registers);
		void Receive(const uint8_t *data, size_t size);
		size_t Transmit(uint8_t data[1024]);
	};
}
#endif
