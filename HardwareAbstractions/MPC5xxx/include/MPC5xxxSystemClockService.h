#ifndef MPC5XXX_SYSTEM_CLOCK_SERVICE_H
#define MPC5XXX_SYSTEM_CLOCK_SERVICE_H
#include <cstdint>
namespace MPC5xxx {
class MPC5xxxSystemClockService {
public:
	virtual ~MPC5xxxSystemClockService() = default;
	static bool HasInstance();
	static const MPC5xxxSystemClockService& Instance();
	virtual bool Ready() const = 0;
	virtual std::uint32_t GetExternalCrystalFrequency() const = 0;
	virtual std::uint32_t SystemClockHz() const = 0;
	virtual std::uint32_t PeripheralClockHz() const = 0;
protected:
	static const MPC5xxxSystemClockService* _instance;
}; }
#endif
