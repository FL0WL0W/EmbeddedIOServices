#include "MPC5xxxFlexCAN2Service.h"

#include <utility>

using namespace EmbeddedIOServices;

namespace MPC5xxx
{
	static constexpr uint8_t RX_MB_FIRST = 0;
	static constexpr uint8_t RX_MB_COUNT = 32;
	static constexpr uint8_t RX_EMPTY = 0x4;
	static constexpr uint8_t TX_INACTIVE = 0x8;
	static constexpr uint8_t TX_DATA = 0xC;

	static uint32_t DisableExternalInterrupts()
	{
		uint32_t previousMsr;
		asm volatile(
			"mfmsr %0\n"
			"wrteei 0\n"
			"isync\n"
			: "=r"(previousMsr)
			:
			: "memory");
		return previousMsr;
	}

	static void RestoreExternalInterrupts(const uint32_t previousMsr)
	{
		// MSR[EE] is bit 16 in PowerPC bit numbering, represented by bit 15
		// in the integer value.
		if ((previousMsr & 0x00008000U) != 0U)
		{
			asm volatile(
				"wrteei 1\n"
				"isync\n"
				:
				:
				: "memory");
		}
	}

	static uint32_t MailboxInterruptFlag(const uint8_t mailbox)
	{
		return 1UL << (mailbox & 31U);
	}

	static void ClearMailboxInterruptFlag(
		volatile FLEXCAN2_tag &can,
		const uint8_t mailbox)
	{
		const uint32_t flag = MailboxInterruptFlag(mailbox);
		if (mailbox < 32U)
			can.IFRL.R = flag;
		else
			can.IFRH.R = flag;
	}

	static bool MailboxInterruptPending(
		volatile FLEXCAN2_tag &can,
		const uint8_t mailbox)
	{
		const uint32_t flag = MailboxInterruptFlag(mailbox);
		return mailbox < 32U
			? (can.IFRL.R & flag) != 0U
			: (can.IFRH.R & flag) != 0U;
	}

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
		can.MCR.R    	|= 0b00000000000000010000000000000000; // Enable the individual RXIMR masks.
		can.MCR.B.SRXDIS = 1;
		can.MCR.B.MAXMB  = 63U;
		can.RXGMASK.R = 0;

		for (uint8_t i = RX_MB_FIRST; i < RX_MB_COUNT; ++i)
		{
			can.BUF[i].CS.R = 0;
			can.BUF[i].ID.R = 0;
			can.RXIMR[i].R = 0; // Every identifier bit is don't-care.
			can.BUF[i].CS.B.CODE = RX_EMPTY;
		}
		for (uint8_t mailbox = 32U; mailbox < 64U; ++mailbox)
		{
			can.BUF[mailbox].CS.R = 0U;
			can.BUF[mailbox].ID.R = 0U;
			can.BUF[mailbox].CS.B.CODE = TX_INACTIVE;
		}
		can.IFRL.R = 0xFFFFFFFFU;
		can.IFRH.R = 0xFFFFFFFFU;

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

		// MB32..MB63 form a hardware-backed software FIFO. Frame contents live in
		// inactive message buffers and only the queue head is activated, so CAN-ID
		// arbitration cannot reorder queued frames.
		TransmitQueueState &transmitQueue = _transmitQueues[busNumber];
		can_send_completion_callback_t completion;
		const uint32_t previousMsr = DisableExternalInterrupts();
		if (transmitQueue.Count != 0U)
		{
			const uint8_t completedSlot = transmitQueue.Head;
			const uint8_t completedMailbox = static_cast<uint8_t>(
				TransmitMailboxFirst + completedSlot);
			if (MailboxInterruptPending(can, completedMailbox))
			{
				ClearMailboxInterruptFlag(can, completedMailbox);
				completion = std::move(
					transmitQueue.CompletionCallbacks[completedSlot]);
				transmitQueue.CompletionCallbacks[completedSlot] = nullptr;
				transmitQueue.Head = static_cast<uint8_t>(
					(transmitQueue.Head + 1U) % TransmitMailboxCount);
				--transmitQueue.Count;

				// Start the oldest already-queued frame before running the callback.
				// A frame submitted by that callback therefore joins the tail rather
				// than jumping ahead of existing traffic.
				if (transmitQueue.Count != 0U)
				{
					const uint8_t nextMailbox = static_cast<uint8_t>(
						TransmitMailboxFirst + transmitQueue.Head);
					ClearMailboxInterruptFlag(can, nextMailbox);
					can.BUF[nextMailbox].CS.B.CODE = TX_DATA;
				}
			}
		}
		RestoreExternalInterrupts(previousMsr);
		if (completion)
			completion();

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

				volatile canbuf_t &mb = can.BUF[mailbox];
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
		: _numberOfCANPeripherals(numberOfCANPeripherals),
		  _canPeripherals(canPeripherals),
		  _transmitQueues(numberOfCANPeripherals)
	{
		for (uint8_t i = 0; i < numberOfCANPeripherals; ++i)
			InitFlexCAN(*canPeripherals[i], canBaudRates[i], externalCrystalHz);
	}

	void MPC5xxxFlexCAN2Service::Send(
		const CANIdentifier_t identifier,
		const CANData_t data,
		const uint8_t dataLength,
		can_send_completion_callback_t completion)
	{
		if (identifier.CANBusNumber >= _numberOfCANPeripherals)
			return;

		volatile FLEXCAN2_tag &can = *_canPeripherals[identifier.CANBusNumber];
		TransmitQueueState &transmitQueue =
			_transmitQueues[identifier.CANBusNumber];
		const uint32_t previousMsr = DisableExternalInterrupts();
		if (transmitQueue.Count >= TransmitMailboxCount)
		{
			// The ICANService contract has no failure return. Preserve nonblocking
			// behavior and retain a diagnostic count if all 32 hardware slots are
			// occupied. Callback-paced ISO-TP keeps at most one of its own frames
			// outstanding, so reaching this limit indicates producer overload.
			++transmitQueue.OverflowCount;
			RestoreExternalInterrupts(previousMsr);
			return;
		}

		const bool startImmediately = transmitQueue.Count == 0U;
		const uint8_t slot = transmitQueue.Tail;
		const uint8_t mailbox = static_cast<uint8_t>(
			TransmitMailboxFirst + slot);
		volatile canbuf_t &mb = can.BUF[mailbox];
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
		transmitQueue.CompletionCallbacks[slot] = std::move(completion);
		transmitQueue.Tail = static_cast<uint8_t>(
			(transmitQueue.Tail + 1U) % TransmitMailboxCount);
		++transmitQueue.Count;

		if (startImmediately)
		{
			ClearMailboxInterruptFlag(can, mailbox);
			mb.CS.B.CODE = TX_DATA;
		}
		RestoreExternalInterrupts(previousMsr);
	}
}
