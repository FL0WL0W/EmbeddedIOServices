#include "ICANService.h"
#include "stdint.h"

#ifndef MPC5674FCANSERVICE_H
#define MPC5674FCANSERVICE_H

namespace MPC5674F
{
	enum class CANBaudRate : uint32_t
	{
		Disabled = 0,
		Kbps125  = 125000,
		Kbps250  = 250000,
		Kbps500  = 500000,
		Mbps1    = 1000000,
	};

	class MPC5674FCANService : public EmbeddedIOServices::ICANService
	{
	protected:
		uint8_t _configuredMask; // bitmask: bit0=CAN_A, bit1=CAN_B, bit2=CAN_C, bit3=CAN_D
	public:
		/// @param canABaudRate  Baud rate for CAN_A (Disabled = do not configure)
		/// @param canBBaudRate  Baud rate for CAN_B (Disabled = do not configure)
		/// @param canCBaudRate  Baud rate for CAN_C (Disabled = do not configure)
		/// @param canDBaudRate  Baud rate for CAN_D (Disabled = do not configure)
		/// @param canDAlternatePins  true = use alternate pad configuration for CAN_D TX/RX
		MPC5674FCANService(CANBaudRate canABaudRate, CANBaudRate canBBaudRate, CANBaudRate canCBaudRate, CANBaudRate canDBaudRate, bool canDAlternatePins = false);
        void Send(const EmbeddedIOServices::CANIdentifier_t identifier, const EmbeddedIOServices::CANData_t data, const uint8_t dataLength);
	};
}

#endif