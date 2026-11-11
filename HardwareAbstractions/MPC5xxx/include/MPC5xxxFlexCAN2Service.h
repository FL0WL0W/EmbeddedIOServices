#ifndef MPC5XXXFLEXCAN2SERVICE_H
#define MPC5XXXFLEXCAN2SERVICE_H

#include "ICANService.h"
#include "MPC5xxx.h"
#include "stdint.h"

namespace MPC5xxx
{
	enum class CANBaudRate : uint32_t { Kbps125=125000, Kbps250=250000, Kbps500=500000, Mbps1=1000000 };

	class MPC5xxxFlexCAN2Service : public EmbeddedIOServices::ICANService
	{
		static constexpr uint8_t CANPeripheralCount =
			0U
		#ifdef CAN_A
			+ 1U
		#endif
		#ifdef CAN_B
			+ 1U
		#endif
		#ifdef CAN_C
			+ 1U
		#endif
		#ifdef CAN_D
			+ 1U
		#endif
		#ifdef CAN_E
			+ 1U
		#endif
		#ifdef CAN_F
			+ 1U
		#endif
		#ifdef CAN_G
			+ 1U
		#endif
		#ifdef CAN_H
			+ 1U
		#endif
			;

		static volatile FLEXCAN2_tag* const FLEXCAN_PERIPHERALS[CANPeripheralCount];
		static uint8_t BusNumberFromPeripheral(volatile FLEXCAN2_tag& can);
		static_assert(CANPeripheralCount > 0U, "No FlexCAN peripheral defined");
		static_assert(CANPeripheralCount <= 8U, "CAN bus number is three bits");
		static constexpr uint8_t RX_MB_FIRST = 0;
		static constexpr uint8_t RX_MB_COUNT = 32;
		static constexpr uint8_t RX_EMPTY = 0x4;
		static constexpr uint8_t TX_MB_FIRST = 32U;
		static constexpr uint8_t TX_MB_COUNT = 32U;
		static constexpr uint8_t TX_INACTIVE = 0x8;
		static constexpr uint8_t TX_DATA = 0xC;

		struct TransmitQueueState
		{
			EmbeddedIOServices::can_send_completion_callback_t CompletionCallbacks[TX_MB_COUNT];
			uint8_t Head = 0U;
			uint8_t Tail = 0U;
			uint8_t Count = 0U;
			uint32_t OverflowCount = 0U;
		};

		static TransmitQueueState _transmitQueues[CANPeripheralCount];
		static MPC5xxxFlexCAN2Service _instance;
		MPC5xxxFlexCAN2Service() = default;

	public:
		static MPC5xxxFlexCAN2Service& Instance();
		/**
		 * @param interruptPriority Priority for all mailbox vectors belonging to
		 * this FlexCAN module. Zero disables mailbox interrupts and selects polling
		 * through PollFlexCAN().
		 */
		static uint8_t Initialize(volatile FLEXCAN2_tag& canPeripheral,
			CANBaudRate baudRate, uint8_t interruptPriority);
		static void PollFlexCAN(volatile FLEXCAN2_tag& can);
		void Send(EmbeddedIOServices::CANIdentifier_t identifier,
			EmbeddedIOServices::CANData_t data, uint8_t dataLength,
			EmbeddedIOServices::can_send_completion_callback_t completion = nullptr) override;
	};
}
#endif
