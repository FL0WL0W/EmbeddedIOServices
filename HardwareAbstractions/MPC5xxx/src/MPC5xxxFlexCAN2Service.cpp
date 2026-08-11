#include "MPC5xxxFlexCAN2Service.h"

using namespace EmbeddedIOServices;

namespace MPC5xxx
{
	// TX mailbox index — use MB63 (highest priority in local priority scheme)
	static constexpr uint8_t TX_MB = 63;
	static void InitFlexCAN(volatile struct FLEXCAN2_tag &can, CANBaudRate baudRate, const uint32_t externalCrystalHz)
	{
		// Enter freeze mode
		can.MCR.B.MDIS = 0;
		can.MCR.B.FRZ  = 1;
		can.MCR.B.HALT = 1;
		while (can.MCR.B.FRZACK == 0) {}

		// Configure bit timing
		const uint32_t baudRateHz = static_cast<uint32_t>(baudRate);
		if (baudRate == CANBaudRate::Mbps1)
		{
			const uint32_t presdiv = (externalCrystalHz / (baudRateHz * 8U)) - 1;
			can.CR.B.PRESDIV = static_cast<uint8_t>(presdiv);
			can.CR.B.PROPSEG = 1; // prop seg  = 2 Tq
			can.CR.B.PSEG1   = 2; // phase seg1 = 3 Tq
			can.CR.B.PSEG2   = 1; // phase seg2 = 2 Tq  => 1+2+3+2 = 8 Tq
			can.CR.B.RJW     = 1;
		}
		else
		{
			const uint32_t presdiv = (externalCrystalHz / (baudRateHz * 16U)) - 1;
			can.CR.B.PRESDIV = static_cast<uint8_t>(presdiv);
			can.CR.B.PROPSEG = 5; // prop seg  = 6 Tq
			can.CR.B.PSEG1   = 5; // phase seg1 = 6 Tq
			can.CR.B.PSEG2   = 2; // phase seg2 = 3 Tq  => 1+6+6+3 = 16 Tq
			can.CR.B.RJW     = 1;
		}
		can.CR.B.CLKSRC = 0; // external oscillator
		can.CR.B.SMP    = 1; // three samples

		// Enable RXFIFO (FEN=1), format A (IDAM=0: one full ID per filter entry)
		// Disable self-reception, keep MAXMB=63 so MB63 is available for TX
		can.MCR.B.FEN    = 1;
		can.MCR.B.IDAM   = 0;
		can.MCR.B.SRXDIS = 1;
		can.MCR.B.MAXMB  = TX_MB;

		// Accept all frames: global RX mask = 0 (all bits don't care)
		can.RXGMASK.R = 0x00000000;

		// With FEN+IDAM=0, BUF[6] and BUF[7] hold the 8 ID filter table entries.
		// Writing 0 with mask=0 accepts everything.
		can.BUF[6].CS.R = 0;
		can.BUF[6].ID.R = 0;
		can.BUF[7].CS.R = 0;
		can.BUF[7].ID.R = 0;

		// Initialise TX mailbox (MB63) as inactive
		can.BUF[TX_MB].CS.B.CODE = 0x8; // TX_INACTIVE

		// Exit freeze mode
		can.MCR.B.HALT = 0;
		can.MCR.B.FRZ  = 0;
		while (can.MCR.B.FRZACK == 1) {}
	}

	void MPC5xxxFlexCAN2Service::PollFlexCAN(volatile struct FLEXCAN2_tag &can)
	{
		const uint8_t busNumber = 0;
		for(uint8_t i = 0; i < _numberOfCANPeripherals; ++i)
		{
			if (_canPeripherals[i] == &can)
			{
				busNumber = i;
				break;
			}
		}
		if(busNumber >= _numberOfCANPeripherals)
			return;
		// Drain the RXFIFO while not-empty (IFRL.BUF05I = bit 5)
		while (can.IFRL.R & (1U << 5))
		{
			// Snapshot the RXFIFO output register (BUF[0])
			const uint32_t cs = can.BUF[0].CS.R;
			const uint32_t id = can.BUF[0].ID.R;

			CANData_t data;
			for (uint8_t i = 0; i < 8; ++i)
				data.Data[i] = can.BUF[0].DATA.B[i];

			const uint8_t dlc = static_cast<uint8_t>((cs >> 16) & 0xFU);
			const bool    ide = (cs >> 21) & 1U;

			CANIdentifier_t identifier;
			identifier.CANBusNumber = busNumber;
			if (ide)
				// Extended: full 29-bit = (STD_ID << 18) | EXT_ID
				identifier.CANIdentifier = ((id >> 18) & 0x7FFU) << 18 | (id & 0x3FFFFU);
			else
				// Standard: 11-bit in STD_ID field
				identifier.CANIdentifier = (id >> 18) & 0x7FFU;

			// Clear RXFIFO not-empty flag before calling Receive so re-entrant frames are caught
			can.IFRL.R = (1U << 5);

			Receive(identifier, data, dlc);
		}
	}

	MPC5xxxFlexCAN2Service::MPC5xxxFlexCAN2Service(FLEXCAN2_tag **canPeripherals, CANBaudRate *canBaudRates, const uint8_t numberOfCANPeripherals, const uint32_t externalCrystalHz, const uint32_t externalCrystalHz = 8000000)
		: _numberOfCANPeripherals(numberOfCANPeripherals),
		  _canPeripherals(canPeripherals)
	{
		for(uint8_t i = 0; i < numberOfCANPeripherals; ++i)
		{
			InitFlexCAN(*canPeripherals[i], canBaudRates[i], externalCrystalHz);
		}
	}

	void MPC5xxxFlexCAN2Service::Send(const CANIdentifier_t identifier, const CANData_t data, const uint8_t dataLength)
	{
		if(identifier.CANBusNumber >= _numberOfCANPeripherals)
			return;
		volatile struct FLEXCAN2_tag &can = *_canPeripherals[identifier.CANBusNumber];
		volatile struct canbuf_t    &mb  = can.BUF[TX_MB];

		// Wait for MB63 to be free (not actively transmitting)
		while (mb.CS.B.CODE == 0xC || mb.CS.B.CODE == 0xE) {} //might want to turn this into a pseudo fifo using multiple mailboxes

		// Lock MB by writing TX_INACTIVE
		mb.CS.B.CODE = 0x8;

		// Fill ID
		const bool extended = identifier.CANIdentifier > 0x7FFU;
		if (extended)
		{
			mb.CS.B.IDE  = 1;
			mb.CS.B.SRR  = 1;
			mb.ID.B.STD_ID = static_cast<uint16_t>((identifier.CANIdentifier >> 18) & 0x7FFU);
			mb.ID.B.EXT_ID = identifier.CANIdentifier & 0x3FFFFU;
		}
		else
		{
			mb.CS.B.IDE    = 0;
			mb.CS.B.SRR    = 0;
			mb.ID.B.STD_ID = static_cast<uint16_t>(identifier.CANIdentifier);
			mb.ID.B.EXT_ID = 0;
		}

		// Fill data
		const uint8_t len = dataLength > 8 ? 8 : dataLength;
		for (uint8_t i = 0; i < len; ++i)
			mb.DATA.B[i] = data.Data[i];

		// Trigger TX: write full CS with CODE=0xC (DATA frame)
		mb.CS.B.RTR    = 0;
		mb.CS.B.LENGTH = len;
		mb.CS.B.CODE   = 0xC;
	}
}
