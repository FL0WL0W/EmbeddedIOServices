#include "MPC56xxSystemClockService.h"
#if defined(MPC5674F)
#include "MPC5xxx.h"
#include <algorithm>

namespace {
// MPC5674F Enhanced PLL register field masks and positions
constexpr std::uint32_t EPREDIV_MASK = 0x000F0000U;  // ESYNCR1[19:16]
constexpr std::uint32_t EMFD_MASK = 0x000000FFU;     // ESYNCR1[7:0]
constexpr std::uint32_t ERFD_MASK = 0x0000003FU;     // ESYNCR2[5:0]
constexpr std::uint32_t CLKCFG_MASK = 0x70000000U;   // ESYNCR1[30:28]

struct Settings { std::uint8_t pre, mul, rfd; std::uint32_t hz; bool valid; };

Settings FindSettings(std::uint32_t referenceHz, std::uint32_t requestedHz)
{
	Settings best = {0U, 0U, 0U, 0U, false};
	// EPREDIV is 4 bits (0-15), actual divider is EPREDIV + 1
	for (std::uint8_t pre = 0U; pre <= 15U; ++pre) {
		const std::uint32_t preDiv = pre + 1U;
		if (referenceHz / preDiv < 4000000U) continue;
		
		// ERFD is 6 bits (0-63), actual divider is 2^ERFD
		// For each rfd, calculate the mul needed instead of iterating all 255 values
		for (std::uint8_t rfd = 0U; rfd < 64U; ++rfd) {
			// Target ICO = requestedHz * 2^rfd (before final rfd division)
			// ICO = referenceHz * (mul + 16) / preDiv
			// Therefore: mul = (requestedHz * 2^rfd * preDiv / referenceHz) - 16
			const std::uint64_t targetIco64 = static_cast<std::uint64_t>(requestedHz) << rfd;
			const std::uint64_t mulTerm = (targetIco64 * preDiv) / referenceHz;
			
			if (mulTerm < 16U) continue;
			
			// EMFD is 8 bits (0-255), actual multiplier is EMFD + 16
			std::uint8_t mul = static_cast<std::uint8_t>(std::min(mulTerm - 16U, 255ULL));
			
			const std::uint64_t ico = static_cast<std::uint64_t>(referenceHz) * (mul + 16U) / preDiv;
			if (ico < 48000000U) continue;
			
			const std::uint32_t hz = static_cast<std::uint32_t>(ico >> rfd);
			if (hz <= requestedHz && hz > best.hz) best = {pre, mul, rfd, hz, true};
		}
	}
	return best;
}

void ApplyESYNCR1(std::uint32_t& esyncr1, const Settings& s)
{
	// Clear EPREDIV and EMFD fields, preserve CLKCFG
	esyncr1 &= ~(EPREDIV_MASK | EMFD_MASK);
	esyncr1 |= (static_cast<std::uint32_t>(s.pre) << 16U) | s.mul;
}

void ApplyESYNCR2(std::uint32_t& esyncr2, std::uint8_t rfd)
{
	// Clear ERFD field, preserve other bits
	esyncr2 &= ~ERFD_MASK;
	esyncr2 |= rfd;
}
}

namespace MPC5xxx {
	MPC56xxSystemClockService MPC56xxSystemClockService::_instance;
	std::uint32_t MPC56xxSystemClockService::_referenceClockHz = 0U;
	bool MPC56xxSystemClockService::_ready = false;

	MPC56xxSystemClockService::MPC56xxSystemClockService() = default;

	bool MPC56xxSystemClockService::Initialize(std::uint32_t referenceHz,
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
		
		std::uint32_t esyncr1 = FMPLL.ESYNCR1.R;
		std::uint32_t esyncr2 = FMPLL.ESYNCR2.R;
		
		// // First apply with RFD+1 for locking phase
		ApplyESYNCR1(esyncr1, settings);
		ApplyESYNCR2(esyncr2, settings.rfd + 1U);
		FMPLL.ESYNCR1.R = esyncr1;
		FMPLL.ESYNCR2.R = esyncr2;
		asm volatile("mbar 0" ::: "memory");
		
		// Wait for lock
		while (FMPLL.SYNSR.B.LOCK == 0U) if (--timeout == 0U) return false;
		
		// Apply final RFD value
		ApplyESYNCR2(esyncr2, settings.rfd);
		FMPLL.ESYNCR2.R = esyncr2;
		asm volatile("mbar 0" ::: "memory");
		
		_ready = true;
		return true;
	}

	std::uint32_t MPC56xxSystemClockService::SystemClockHzImplementation() const
	{
		if (_referenceClockHz == 0U) return 0U;
		const std::uint32_t eprediv = (FMPLL.ESYNCR1.R & EPREDIV_MASK) >> 16U;
		const std::uint32_t emfd = FMPLL.ESYNCR1.R & EMFD_MASK;
		const std::uint32_t erfd = FMPLL.ESYNCR2.R & ERFD_MASK;
		return static_cast<std::uint32_t>(static_cast<std::uint64_t>(_referenceClockHz)
			* (emfd + 16U) / ((eprediv + 1U) * (1U << erfd)));
	}
}
#endif
