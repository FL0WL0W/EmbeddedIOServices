#include "MPC55xxSystemClockService.h"
#if defined(MPC5532) || defined(MPC5533) || defined(MPC5534) || \
	defined(MPC5553) || defined(MPC5554) || defined(MPC5561) || \
	defined(MPC5565) || defined(MPC5566) || defined(MPC5567)
#include "MPC5xxx.h"

namespace {
constexpr std::uint32_t PredividerMask = 0x70000000U;
constexpr std::uint32_t MultiplierMask = 0x0F800000U;
constexpr std::uint32_t ReducedDividerMask = 0x00380000U;
constexpr std::uint32_t FailureMask = 0x00024000U;
constexpr std::uint32_t ModulationDepthMask = 0x00000C00U;
struct Settings { std::uint8_t pre, mul, rfd; std::uint32_t hz; bool valid; };

Settings FindSettings(std::uint32_t referenceHz, std::uint32_t requestedHz)
{
	Settings best = {0U, 0U, 0U, 0U, false};
	for (std::uint8_t pre = 0U; pre <= 4U; ++pre) {
		if (referenceHz / (pre + 1U) < 4000000U) continue;
		for (std::uint8_t mul = 0U; mul <= 31U; ++mul) {
			const std::uint64_t ico = static_cast<std::uint64_t>(referenceHz) * (mul + 4U) / (pre + 1U);
			if (ico < 48000000U) continue;
			for (std::uint8_t rfd = 0U; rfd < 7U; ++rfd) {
				const std::uint32_t hz = static_cast<std::uint32_t>(ico >> rfd);
				if (hz <= requestedHz && hz > best.hz) best = {pre, mul, rfd, hz, true};
			}
		}
	}
	return best;
}

std::uint32_t Apply(std::uint32_t value, const Settings& s, std::uint8_t rfd)
{
	value &= ~(PredividerMask | MultiplierMask | ReducedDividerMask | FailureMask | ModulationDepthMask);
	return value | (static_cast<std::uint32_t>(s.pre) << 28U)
		| (static_cast<std::uint32_t>(s.mul) << 23U)
		| (static_cast<std::uint32_t>(rfd) << 19U);
}
}

namespace MPC5xxx {
	MPC55xxSystemClockService MPC55xxSystemClockService::_instance;
	std::uint32_t MPC55xxSystemClockService::_referenceClockHz = 0U;
	bool MPC55xxSystemClockService::_ready = false;

	MPC55xxSystemClockService::MPC55xxSystemClockService() = default;

	bool MPC55xxSystemClockService::Initialize(std::uint32_t referenceHz,
		std::uint32_t requestedHz, std::uint32_t timeout)
	{
		_referenceClockHz = referenceHz;
		if (referenceHz < 8000000U || referenceHz > 20000000U || requestedHz == 0U || timeout == 0U)
			return false;
		if (FMPLL.SYNSR.B.LOCK != 0U && _instance.SystemClockHzImplementation() == requestedHz) {
			_ready = true;
			return true;
		}
		const Settings settings = FindSettings(referenceHz, requestedHz);
		if (!settings.valid) return false;
		const std::uint32_t original = FMPLL.SYNCR.R;
		const std::uint32_t failureEnables = original & FailureMask;
		FMPLL.SYNCR.R = Apply(original, settings, settings.rfd + 1U);
		asm volatile("mbar 0" ::: "memory");
		while (FMPLL.SYNSR.B.LOCK == 0U) if (--timeout == 0U) return false;
		FMPLL.SYNCR.R = Apply(original, settings, settings.rfd) | failureEnables;
		asm volatile("mbar 0" ::: "memory");
		_ready = true;
		return true;
	}

	std::uint32_t MPC55xxSystemClockService::SystemClockHzImplementation() const
	{
		if (_referenceClockHz == 0U) return 0U;
		return static_cast<std::uint32_t>(static_cast<std::uint64_t>(_referenceClockHz)
			* (FMPLL.SYNCR.B.MFD + 4U) / ((FMPLL.SYNCR.B.PREDIV + 1U) * (1U << FMPLL.SYNCR.B.RFD)));
	}
}
#endif
