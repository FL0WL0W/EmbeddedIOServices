#include "MPC56xxSystemClockService.h"
#if defined(MPC5674F)
#include "MPC5xxx.h"

namespace {
// MPC5674F Enhanced PLL register field masks and positions
constexpr std::uint32_t EPREDIV_MASK = 0x000F0000U;  // ESYNCR1[19:16]
constexpr std::uint32_t EMFD_MASK = 0x000000FFU;     // ESYNCR1[7:0]
constexpr std::uint32_t ERFD_MASK = 0x0000003FU;     // ESYNCR2[5:0]
constexpr std::uint32_t CLKCFG_MASK = 0x70000000U;   // ESYNCR1[30:28]

struct Settings { std::uint8_t pre, mul, rfd; std::uint32_t hz; bool valid; };

std::uint64_t CalculateIco(std::uint32_t referenceHz,
	std::uint32_t multiplier, std::uint32_t preDivider)
{
	// Calculate referenceHz * multiplier / preDivider exactly without a
	// 64-bit divide. Splitting referenceHz into quotient and remainder keeps
	// both divisions 32-bit while the potentially >4 GHz result remains wide.
	return static_cast<std::uint64_t>(referenceHz / preDivider) * multiplier +
		((referenceHz % preDivider) * multiplier) / preDivider;
}

Settings FindSettings(std::uint32_t referenceHz, std::uint32_t requestedHz)
{
	Settings best = {0U, 0U, 0U, 0U, false};
	// EPREDIV is 4 bits (0-15), actual divider is EPREDIV + 1
	for (std::uint8_t pre = 0U; pre <= 15U; ++pre) {
		const std::uint32_t preDiv = pre + 1U;
		if (referenceHz / preDiv < 4000000U) continue;

		// Search the small hardware field space directly. This is performed only
		// during clock initialization and avoids solving for EMFD with a 64-bit
		// division.
		for (std::uint16_t mul = 0U; mul <= 255U; ++mul) {
			const std::uint64_t ico = CalculateIco(referenceHz, mul + 16U, preDiv);
			if (ico < 48000000U) continue;

			// ERFD is 6 bits (0-63), and its actual divider is 2^ERFD.
			for (std::uint8_t rfd = 0U; rfd < 64U; ++rfd) {
				const std::uint64_t divided = ico >> rfd;
				if (divided == 0U) break;
				if (divided <= requestedHz && divided > best.hz)
					best = {pre, static_cast<std::uint8_t>(mul), rfd,
						static_cast<std::uint32_t>(divided), true};
			}
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
		return static_cast<std::uint32_t>(
			CalculateIco(_referenceClockHz, emfd + 16U, eprediv + 1U) >> erfd);
	}
}
#endif
