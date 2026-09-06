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
		static constexpr volatile FLEXCAN2_tag* const FLEXCAN_PERIPHERALS[] = {
#ifdef CAN_A
			&CAN_A,
#endif
#ifdef CAN_B
			&CAN_B,
#endif
#ifdef CAN_C
			&CAN_C,
#endif
#ifdef CAN_D
			&CAN_D,
#endif
#ifdef CAN_E
			&CAN_E,
#endif
#ifdef CAN_F
			&CAN_F,
#endif
#ifdef CAN_G
			&CAN_G,
#endif
#ifdef CAN_H
			&CAN_H,
#endif
		};
		static constexpr uint8_t CANPeripheralCount = sizeof(FLEXCAN_PERIPHERALS) / sizeof(FLEXCAN_PERIPHERALS[0]);
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
		static uint8_t Initialize(volatile FLEXCAN2_tag& canPeripheral, CANBaudRate baudRate);
		static void PollFlexCAN(volatile FLEXCAN2_tag& can);
		void Send(EmbeddedIOServices::CANIdentifier_t identifier,
			EmbeddedIOServices::CANData_t data, uint8_t dataLength,
			EmbeddedIOServices::can_send_completion_callback_t completion = nullptr) override;
	};
}
#endif
