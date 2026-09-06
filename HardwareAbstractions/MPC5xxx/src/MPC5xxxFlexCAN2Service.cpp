#include "MPC5xxxFlexCAN2Service.h"
#include "MPC5xxxSystemClockService.h"

#include <utility>

using namespace EmbeddedIOServices;

namespace MPC5xxx
{
	constexpr volatile FLEXCAN2_tag* const MPC5xxxFlexCAN2Service::FLEXCAN_PERIPHERALS[];

	MPC5xxxFlexCAN2Service MPC5xxxFlexCAN2Service::_instance;
	MPC5xxxFlexCAN2Service::TransmitQueueState
		MPC5xxxFlexCAN2Service::_transmitQueues[CANPeripheralCount];

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

	uint8_t MPC5xxxFlexCAN2Service::BusNumberFromPeripheral(volatile FLEXCAN2_tag& can)
	{
		for (uint8_t bus = 0U; bus < MPC5xxxFlexCAN2Service::CANPeripheralCount; ++bus)
			if (FLEXCAN_PERIPHERALS[bus] == &can)
				return bus;
		return 0xFFU;
	}
	
	void MPC5xxxFlexCAN2Service::PollFlexCAN(volatile struct FLEXCAN2_tag &can)
	{
		const uint8_t busNumber = BusNumberFromPeripheral(can);
		if (busNumber >= CANPeripheralCount)
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
				TX_MB_FIRST + completedSlot);
			if (MailboxInterruptPending(can, completedMailbox))
			{
				ClearMailboxInterruptFlag(can, completedMailbox);
				completion = std::move(
					transmitQueue.CompletionCallbacks[completedSlot]);
				transmitQueue.CompletionCallbacks[completedSlot] = nullptr;
				transmitQueue.Head = static_cast<uint8_t>(
					(transmitQueue.Head + 1U) % TX_MB_COUNT);
				--transmitQueue.Count;

				// Start the oldest already-queued frame before running the callback.
				// A frame submitted by that callback therefore joins the tail rather
				// than jumping ahead of existing traffic.
				if (transmitQueue.Count != 0U)
				{
					const uint8_t nextMailbox = static_cast<uint8_t>(
						TX_MB_FIRST + transmitQueue.Head);
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
				_instance.Receive(identifier, data, dlc);
			}
		} while (receivedInPass);
	}

	MPC5xxxFlexCAN2Service& MPC5xxxFlexCAN2Service::Instance()
	{
		return _instance;
	}

	uint8_t MPC5xxxFlexCAN2Service::Initialize(
		volatile FLEXCAN2_tag& can, const CANBaudRate baudRate)
	{
		const uint8_t busNumber = BusNumberFromPeripheral(can);
		if (busNumber >= CANPeripheralCount ||
			!MPC5xxxSystemClockService::HasInstance() ||
			!MPC5xxxSystemClockService::Instance().Ready())
			return 0xFFU;

		const uint32_t externalCrystalHz =
			MPC5xxxSystemClockService::Instance().GetExternalCrystalFrequency();

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

		return busNumber;
	}

	void MPC5xxxFlexCAN2Service::Send(
		const CANIdentifier_t identifier,
		const CANData_t data,
		const uint8_t dataLength,
		can_send_completion_callback_t completion)
	{
		if (identifier.CANBusNumber >= CANPeripheralCount)
			return;

		volatile FLEXCAN2_tag &can = *FLEXCAN_PERIPHERALS[identifier.CANBusNumber];
		TransmitQueueState &transmitQueue =
			_transmitQueues[identifier.CANBusNumber];
		const uint32_t previousMsr = DisableExternalInterrupts();
		if (transmitQueue.Count >= TX_MB_COUNT)
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
			TX_MB_FIRST + slot);
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
			(transmitQueue.Tail + 1U) % TX_MB_COUNT);
		++transmitQueue.Count;

		if (startImmediately)
		{
			ClearMailboxInterruptFlag(can, mailbox);
			mb.CS.B.CODE = TX_DATA;
		}
		RestoreExternalInterrupts(previousMsr);
	}
}

#define FLEXCAN_MAILBOX_HANDLER(module, peripheral, mailbox) \
	extern "C" void FlexCAN_##module##_Buffer##mailbox##_Handler() { \
		MPC5xxx::MPC5xxxFlexCAN2Service::PollFlexCAN(peripheral); }
#define FLEXCAN_MODULE_HANDLERS(module, peripheral) \
	FLEXCAN_MAILBOX_HANDLER(module, peripheral, 0) \
	FLEXCAN_MAILBOX_HANDLER(module, peripheral, 1) \
	FLEXCAN_MAILBOX_HANDLER(module, peripheral, 2) \
	FLEXCAN_MAILBOX_HANDLER(module, peripheral, 3) \
	FLEXCAN_MAILBOX_HANDLER(module, peripheral, 4) \
	FLEXCAN_MAILBOX_HANDLER(module, peripheral, 5) \
	FLEXCAN_MAILBOX_HANDLER(module, peripheral, 6) \
	FLEXCAN_MAILBOX_HANDLER(module, peripheral, 7) \
	FLEXCAN_MAILBOX_HANDLER(module, peripheral, 8) \
	FLEXCAN_MAILBOX_HANDLER(module, peripheral, 9) \
	FLEXCAN_MAILBOX_HANDLER(module, peripheral, 10) \
	FLEXCAN_MAILBOX_HANDLER(module, peripheral, 11) \
	FLEXCAN_MAILBOX_HANDLER(module, peripheral, 12) \
	FLEXCAN_MAILBOX_HANDLER(module, peripheral, 13) \
	FLEXCAN_MAILBOX_HANDLER(module, peripheral, 14) \
	FLEXCAN_MAILBOX_HANDLER(module, peripheral, 15) \
	extern "C" void FlexCAN_##module##_Buffers16To31_Handler() { \
		MPC5xxx::MPC5xxxFlexCAN2Service::PollFlexCAN(peripheral); } \
	extern "C" void FlexCAN_##module##_Buffers32To63_Handler() { \
		MPC5xxx::MPC5xxxFlexCAN2Service::PollFlexCAN(peripheral); }

#ifdef CAN_A
FLEXCAN_MODULE_HANDLERS(A, CAN_A)
#endif
#ifdef CAN_B
FLEXCAN_MODULE_HANDLERS(B, CAN_B)
#endif
#ifdef CAN_C
FLEXCAN_MODULE_HANDLERS(C, CAN_C)
#endif
#ifdef CAN_D
FLEXCAN_MODULE_HANDLERS(D, CAN_D)
#endif
#ifdef CAN_E
FLEXCAN_MODULE_HANDLERS(E, CAN_E)
#endif
#ifdef CAN_F
FLEXCAN_MODULE_HANDLERS(F, CAN_F)
#endif
#ifdef CAN_G
FLEXCAN_MODULE_HANDLERS(G, CAN_G)
#endif
#ifdef CAN_H
FLEXCAN_MODULE_HANDLERS(H, CAN_H)
#endif

#undef FLEXCAN_MODULE_HANDLERS
#undef FLEXCAN_MAILBOX_HANDLER
