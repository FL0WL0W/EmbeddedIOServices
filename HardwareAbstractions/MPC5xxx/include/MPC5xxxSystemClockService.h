#ifndef MPC5XXX_SYSTEM_CLOCK_SERVICE_H
#define MPC5XXX_SYSTEM_CLOCK_SERVICE_H
#include <cstdint>
namespace MPC5xxx {
class MPC5xxxSystemClockService {
public:
	static bool HasInstance();
	static bool Ready();
	static std::uint32_t GetExternalCrystalFrequency();
	static std::uint32_t SystemClockHz();
	static std::uint32_t PeripheralClockHz();
protected:
	MPC5xxxSystemClockService();
	virtual ~MPC5xxxSystemClockService() = default;
private:
	static const MPC5xxxSystemClockService* _instance;
	virtual bool ReadyImplementation() const = 0;
	virtual std::uint32_t GetExternalCrystalFrequencyImplementation() const = 0;
	virtual std::uint32_t SystemClockHzImplementation() const = 0;
	virtual std::uint32_t PeripheralClockHzImplementation() const = 0;
}; }
#endif
