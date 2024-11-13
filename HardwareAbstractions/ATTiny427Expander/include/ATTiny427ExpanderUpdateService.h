#include "IDigitalService.h"
#include <forward_list>

#ifndef ATTINY427EXPANDERUPDATESERVICE_H
#define ATTINY427EXPANDERUPDATESERVICE_H
namespace EmbeddedIOServices
{
	enum ATTiny427Expander_Comm
	{
		UART0,
		UART0Alternate,
		UART1,
		UART1Alternate,
		SPI,
		SPIAlternate
	};

	struct ATTiny427Expander_Registers 
	{
		ATTiny427Expander_Comm Comm;

		//registers
		uint8_t PORTA_DIR = 0b00000100;
		uint8_t PORTA_OUT = 0b00000000;
		uint8_t PORTA_IN = 0b00000000;
		uint8_t PORTB_DIR = 0b00000000;
		uint8_t PORTB_OUT = 0b00000000;
		uint8_t PORTB_IN = 0b00000000;
		uint8_t PORTC_DIR = 0b00000000;
		uint8_t PORTC_OUT = 0b00000000;
		uint8_t PORTC_IN = 0b00000000;
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
		uint8_t EVSYS_CHANNEL0 = 0;
		uint8_t EVSYS_CHANNEL1 = 0;
		uint8_t EVSYS_CHANNEL2 = 0;
		uint8_t EVSYS_CHANNEL3 = 0;
		uint8_t EVSYS_CHANNEL4 = 0;
		uint8_t EVSYS_CHANNEL5 = 0;
		uint8_t EVSYS_CCL_LUT0A = 0;
		uint8_t EVSYS_CCL_LUT1A = 0;
		uint8_t EVSYS_CCL_LUT2A = 0;
		uint8_t EVSYS_CCL_LUT3A = 0;
		uint8_t EVSYS_EVOUTA = 0;
		uint8_t EVSYS_EVOUTB = 0;
		uint8_t EVSYS_EVOUTC = 0;
		uint8_t EVSYS_TCB0_CAPT = 0;
		uint8_t EVSYS_TCB1_CAPT = 0;

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
				case UART0:
					PORTB_DIR = 0b00000100;
					break;
				case UART0Alternate:
				case UART1:
					PORTA_DIR = 0b00000010;
					break;
				case UART1Alternate:
					PORTC_DIR = 0b00000100;
					break;
				case SPI:
					PORTA_DIR = 0b00000100;
					break;
				case SPIAlternate:
					PORTC_DIR = 0b00000010;
					break;
			}
		}

		ATTiny427Expander_Registers() : ATTiny427Expander_Registers(UART0)
		{
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
				uint8_t _receiveAnalogValueHighByte;
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
		void Receive(uint8_t *data, size_t size);
		size_t Transmit(uint8_t data[1024]);
	};
}
#endif
