#ifndef MPC56XX_SYSTEM_CLOCK_SERVICE_H
#define MPC56XX_SYSTEM_CLOCK_SERVICE_H
#if defined(MPC5674F)
#include "MPC5xxxSystemClockService.h"
namespace MPC5xxx {
class MPC56xxSystemClockService final : public MPC5xxxSystemClockService {
public:
	static bool Initialize(std::uint32_t referenceClockHz, std::uint32_t requestedSystemClockHz,
		std::uint32_t lockTimeoutIterations = 1000000U);
private:
	static MPC56xxSystemClockService _instance;
	static std::uint32_t _referenceClockHz;
	static bool _ready;
	MPC56xxSystemClockService();
	bool ReadyImplementation() const override { return _ready; }
	std::uint32_t GetExternalCrystalFrequencyImplementation() const override { return _referenceClockHz; }
	std::uint32_t SystemClockHzImplementation() const override;
	std::uint32_t PeripheralClockHzImplementation() const override { return SystemClockHzImplementation(); }
}; }
#endif
#endif
