#ifndef MPC55XX_SYSTEM_CLOCK_SERVICE_H
#define MPC55XX_SYSTEM_CLOCK_SERVICE_H
#if defined(MPC5532) || defined(MPC5533) || defined(MPC5534) || \
	defined(MPC5553) || defined(MPC5554) || defined(MPC5561) || \
	defined(MPC5565) || defined(MPC5566) || defined(MPC5567)
#include "MPC5xxxSystemClockService.h"
namespace MPC5xxx {
class MPC55xxSystemClockService final : public MPC5xxxSystemClockService {
public:
	static bool Initialize(std::uint32_t referenceClockHz, std::uint32_t requestedSystemClockHz,
		std::uint32_t lockTimeoutIterations = 1000000U);
private:
	static MPC55xxSystemClockService _instance;
	static std::uint32_t _referenceClockHz;
	static bool _ready;
	MPC55xxSystemClockService();
	bool ReadyImplementation() const override { return _ready; }
	std::uint32_t GetExternalCrystalFrequencyImplementation() const override { return _referenceClockHz; }
	std::uint32_t SystemClockHzImplementation() const override;
	std::uint32_t PeripheralClockHzImplementation() const override { return SystemClockHzImplementation(); }
}; }
#endif
#endif
