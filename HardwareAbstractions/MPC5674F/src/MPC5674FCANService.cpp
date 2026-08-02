#include "MPC5674FCANService.h"
#include "MPC5674FClockService.h"
#include "MPC5674F.h"

using namespace EmbeddedIOServices;

namespace MPC5674F
{
	// Ordered CAN_A..CAN_D
	static volatile struct FLEXCAN2_tag * const _canPeripherals[4] = { &CAN_A, &CAN_B, &CAN_C, &CAN_D };

	// TX mailbox index — use MB63 (highest priority in local priority scheme)
	static constexpr uint8_t TX_MB = 63;
	static constexpr uint8_t RX_FIFO_FLAG = 5;
	static constexpr uint8_t RX_FIFO_INTERRUPT_PRIORITY = 1;

	// INTC vectors for FlexCAN BUF5, which is the RXFIFO not-empty source.
	static constexpr uint16_t RX_FIFO_INTERRUPT_VECTORS[4] = { 160, 288, 181, 316 };

	// One service instance pointer per CAN bus so ISRs can dispatch without global state.
	// The constructor installs the pointer before InitFlexCAN enables the interrupt.
	static MPC5674FCANService *_instances[4] = { nullptr, nullptr, nullptr, nullptr };

	static void InitFlexCAN(volatile struct FLEXCAN2_tag &can, CANBaudRate baudRate,
	                        uint16_t rxFifoInterruptVector)
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
			const uint32_t presdiv = (MPC5674F_CRYSTAL_HZ / (baudRateHz * 8U)) - 1;
			can.CR.B.PRESDIV = static_cast<uint8_t>(presdiv);
			can.CR.B.PROPSEG = 1; // prop seg  = 2 Tq
			can.CR.B.PSEG1   = 2; // phase seg1 = 3 Tq
			can.CR.B.PSEG2   = 1; // phase seg2 = 2 Tq  => 1+2+3+2 = 8 Tq
			can.CR.B.RJW     = 1;
		}
		else
		{
			const uint32_t presdiv = (MPC5674F_CRYSTAL_HZ / (baudRateHz * 16U)) - 1;
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

		// Clear a stale RXFIFO indication before enabling BUF5 as an interrupt source.
		can.IFRL.R = (1U << RX_FIFO_FLAG);
		can.IMRL.R |= (1U << RX_FIFO_FLAG);

		// Initialise TX mailbox (MB63) as inactive
		can.BUF[TX_MB].CS.B.CODE = 0x8; // TX_INACTIVE

		// Exit freeze mode
		can.MCR.B.HALT = 0;
		can.MCR.B.FRZ  = 0;
		while (can.MCR.B.FRZACK == 1) {}

		// The bootloader leaves all INTC sources except eMIOS 11 at priority zero.
		// Raising this source above CPR=0 makes the RXFIFO interrupt deliverable.
		INTC.PSR[rxFifoInterruptVector].B.PRI = RX_FIFO_INTERRUPT_PRIORITY;
	}

	static void PollFlexCAN(volatile struct FLEXCAN2_tag &can, uint8_t busNumber, MPC5674FCANService &service)
	{
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

			service.Receive(identifier, data, dlc);
		}
	}

	MPC5674FCANService::MPC5674FCANService(CANBaudRate canABaudRate, CANBaudRate canBBaudRate, CANBaudRate canCBaudRate, CANBaudRate canDBaudRate, bool canDAlternatePins)
		: _configuredMask(0)
	{
		if (canABaudRate != CANBaudRate::Disabled)
		{
			_instances[0] = this;
			InitFlexCAN(CAN_A, canABaudRate, RX_FIFO_INTERRUPT_VECTORS[0]);
			_configuredMask |= (1U << 0);
		}

		if (canBBaudRate != CANBaudRate::Disabled)
		{
			_instances[1] = this;
			InitFlexCAN(CAN_B, canBBaudRate, RX_FIFO_INTERRUPT_VECTORS[1]);
			_configuredMask |= (1U << 1);
		}

		if (canCBaudRate != CANBaudRate::Disabled)
		{
			_instances[2] = this;
			InitFlexCAN(CAN_C, canCBaudRate, RX_FIFO_INTERRUPT_VECTORS[2]);
			_configuredMask |= (1U << 2);
		}

		if (canDBaudRate != CANBaudRate::Disabled)
		{
			// Configure CAN_D pad assignment via SIU PCR
			// PA=1 selects primary CAN_D pins; PA=2 selects alternate CAN_D pins
			const uint16_t pa = canDAlternatePins ? 2 : 1;
			// TODO: confirm exact PCR indices from RM pad muxing table
			SIU.PCR[149].B.PA  = pa; // CAN_D TX
			SIU.PCR[150].B.PA  = pa; // CAN_D RX
			SIU.PCR[150].B.IBE = 1;  // enable input buffer for RX

			_instances[3] = this;
			InitFlexCAN(CAN_D, canDBaudRate, RX_FIFO_INTERRUPT_VECTORS[3]);
			_configuredMask |= (1U << 3);
		}
	}

	void MPC5674FCANService::Send(const CANIdentifier_t identifier, const CANData_t data, const uint8_t dataLength)
	{
		const uint8_t busIndex = identifier.CANBusNumber & 0x3;
		if (!(_configuredMask & (1U << busIndex)))
			return;

		volatile struct FLEXCAN2_tag &can = *_canPeripherals[busIndex];
		volatile struct canbuf_t    &mb  = can.BUF[TX_MB];

		// Wait for MB63 to be free (not actively transmitting)
		while (mb.CS.B.CODE == 0xC || mb.CS.B.CODE == 0xE) {}

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

	extern "C" void CAN_A_RXFIFO_Handler() { if (_instances[0]) PollFlexCAN(CAN_A, 0, *_instances[0]); }
	extern "C" void CAN_B_RXFIFO_Handler() { if (_instances[1]) PollFlexCAN(CAN_B, 1, *_instances[1]); }
	extern "C" void CAN_C_RXFIFO_Handler() { if (_instances[2]) PollFlexCAN(CAN_C, 2, *_instances[2]); }
	extern "C" void CAN_D_RXFIFO_Handler() { if (_instances[3]) PollFlexCAN(CAN_D, 3, *_instances[3]); }
}
