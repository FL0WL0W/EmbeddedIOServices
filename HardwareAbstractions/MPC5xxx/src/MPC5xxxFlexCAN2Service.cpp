#include "MPC5xxxFlexCAN2Service.h"

using namespace EmbeddedIOServices;

namespace MPC5xxx
{
	static constexpr uint8_t RX_MB_FIRST = 0;
	static constexpr uint8_t RX_MB_COUNT = 16;
	static constexpr uint8_t TX_MB = 63;
	static constexpr uint8_t RX_EMPTY = 0x4;
	static constexpr uint8_t TX_INACTIVE = 0x8;
	static constexpr uint8_t TX_DATA = 0xC;

	static void InitFlexCAN(volatile struct FLEXCAN2_tag &can, CANBaudRate baudRate,
		const uint32_t externalCrystalHz)
	{
		can.MCR.B.MDIS = 0;
		can.MCR.B.FRZ  = 1;
		can.MCR.B.HALT = 1;
		while (can.MCR.B.FRZACK == 0) {}

		const uint32_t baudRateHz = static_cast<uint32_t>(baudRate);
		if (baudRate == CANBaudRate::Mbps1)
		{
			can.CR.B.PRESDIV = static_cast<uint8_t>((externalCrystalHz / (baudRateHz * 8U)) - 1U);
			can.CR.B.PROPSEG = 1;
			can.CR.B.PSEG1   = 2;
			can.CR.B.PSEG2   = 1;
			can.CR.B.RJW     = 1;
		}
		else
		{
			can.CR.B.PRESDIV = static_cast<uint8_t>((externalCrystalHz / (baudRateHz * 16U)) - 1U);
			can.CR.B.PROPSEG = 5;
			can.CR.B.PSEG1   = 5;
			can.CR.B.PSEG2   = 2;
			can.CR.B.RJW     = 1;
		}
		can.CR.B.CLKSRC = 0;
		can.CR.B.SMP    = 1;

		// Use MB0..MB15 as an accept-all receive queue instead of the hardware FIFO.
		can.MCR.B.BCC    = 1; // Enable the individual RXIMR masks.
		can.MCR.B.SRXDIS = 1;
		can.MCR.B.MAXMB  = TX_MB;
		can.RXGMASK.R = 0;

		for (uint8_t i = RX_MB_FIRST; i < RX_MB_COUNT; ++i)
		{
			can.BUF[i].CS.R = 0;
			can.BUF[i].ID.R = 0;
			can.RXIMR[i].R = 0; // Every identifier bit is don't-care.
			can.BUF[i].CS.B.CODE = RX_EMPTY;
		}
		can.IFRL.R = 0x0000FFFFU;
		can.BUF[TX_MB].CS.B.CODE = TX_INACTIVE;

		can.MCR.B.HALT = 0;
		can.MCR.B.FRZ  = 0;
		while (can.MCR.B.FRZACK == 1) {}
	}

	void MPC5xxxFlexCAN2Service::PollFlexCAN(volatile struct FLEXCAN2_tag &can)
	{
		uint8_t busNumber = _numberOfCANPeripherals;
		for (uint8_t i = 0; i < _numberOfCANPeripherals; ++i)
		{
			if (_canPeripherals[i] == &can)
			{
				busNumber = i;
				break;
			}
		}
		if (busNumber >= _numberOfCANPeripherals)
			return;

		// FlexCAN fills the first matching empty mailbox. Stop at the first empty
		// one, then restart at MB0 after processing a batch so frames received
		// while callbacks ran are not missed.
		bool receivedInPass;
		do
		{
			receivedInPass = false;
			for (uint8_t mailbox = RX_MB_FIRST; mailbox < RX_MB_COUNT; ++mailbox)
			{
				const uint32_t flag = 1U << mailbox;
				if ((can.IFRL.R & flag) == 0)
					break;

				volatile struct canbuf_t &mb = can.BUF[mailbox];
				const uint32_t cs = mb.CS.R; // Locks this receive mailbox.
				const uint32_t id = mb.ID.R;
				CANData_t data;
				for (uint8_t i = 0; i < 8; ++i)
					data.Data[i] = mb.DATA.B[i];

				const uint8_t dlc = static_cast<uint8_t>((cs >> 16) & 0xFU);
				const bool ide = ((cs >> 21) & 1U) != 0;
				CANIdentifier_t identifier;
				identifier.CANBusNumber = busNumber;
				identifier.CANIdentifier = ide
					? ((((id >> 18) & 0x7FFU) << 18) | (id & 0x3FFFFU))
					: ((id >> 18) & 0x7FFU);

				(void)can.TIMER.R; // Unlock after reading the complete mailbox.
				can.IFRL.R = flag;
				mb.CS.B.CODE = RX_EMPTY;
				receivedInPass = true;
				Receive(identifier, data, dlc);
			}
		} while (receivedInPass);
	}

	MPC5xxxFlexCAN2Service::MPC5xxxFlexCAN2Service(volatile FLEXCAN2_tag *canPeripherals[],
		const CANBaudRate canBaudRates[], const uint8_t numberOfCANPeripherals,
		const uint32_t externalCrystalHz)
		: _numberOfCANPeripherals(numberOfCANPeripherals), _canPeripherals(canPeripherals)
	{
		for (uint8_t i = 0; i < numberOfCANPeripherals; ++i)
			InitFlexCAN(*canPeripherals[i], canBaudRates[i], externalCrystalHz);
	}

	void MPC5xxxFlexCAN2Service::Send(const CANIdentifier_t identifier, const CANData_t data,
		const uint8_t dataLength)
	{
		if (identifier.CANBusNumber >= _numberOfCANPeripherals)
			return;
		volatile struct canbuf_t &mb = _canPeripherals[identifier.CANBusNumber]->BUF[TX_MB];
		while (mb.CS.B.CODE == TX_DATA || mb.CS.B.CODE == 0xE) {}
		mb.CS.B.CODE = TX_INACTIVE;

		const bool extended = identifier.CANIdentifier > 0x7FFU;
		if (extended)
		{
			mb.CS.B.IDE = 1;
			mb.CS.B.SRR = 1;
			mb.ID.B.STD_ID = static_cast<uint16_t>((identifier.CANIdentifier >> 18) & 0x7FFU);
			mb.ID.B.EXT_ID = identifier.CANIdentifier & 0x3FFFFU;
		}
		else
		{
			mb.CS.B.IDE = 0;
			mb.CS.B.SRR = 0;
			mb.ID.B.STD_ID = static_cast<uint16_t>(identifier.CANIdentifier);
			mb.ID.B.EXT_ID = 0;
		}

		const uint8_t len = dataLength > 8 ? 8 : dataLength;
		for (uint8_t i = 0; i < len; ++i)
			mb.DATA.B[i] = data.Data[i];
		mb.CS.B.RTR = 0;
		mb.CS.B.LENGTH = len;
		mb.CS.B.CODE = TX_DATA;
	}
}
