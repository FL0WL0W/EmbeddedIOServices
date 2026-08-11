#include "ICANService.h"
#include "stdint.h"
#include "MPC5xxx.h"

#ifndef MPC5XXXFLEXCAN2SERVICE_H
#define MPC5XXXFLEXCAN2SERVICE_H

namespace MPC5xxx
{
	enum class CANBaudRate : uint32_t
	{
		Kbps125  = 125000,
		Kbps250  = 250000,
		Kbps500  = 500000,
		Mbps1    = 1000000,
	};

	class MPC5xxxFlexCAN2Service : public EmbeddedIOServices::ICANService
	{
	protected:
		const uint8_t _numberOfCANPeripherals;
		volatile FLEXCAN2_tag **_canPeripherals;
	public:
		MPC5xxxFlexCAN2Service(volatile FLEXCAN2_tag *canPeripherals[], const CANBaudRate canBaudRates[], const uint8_t numberOfCANPeripherals, const uint32_t externalCrystalHz = 8000000);
        void Send(const EmbeddedIOServices::CANIdentifier_t identifier, const EmbeddedIOServices::CANData_t data, const uint8_t dataLength);
		void PollFlexCAN(volatile struct FLEXCAN2_tag &can);
	};
}

#endif