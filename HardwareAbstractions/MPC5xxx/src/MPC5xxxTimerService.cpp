#include "MPC5xxxTimerService.h"

#include "MPC5xxxSystemClockService.h"

#include <cstdint>
#include <limits>

using namespace EmbeddedIOServices;

namespace
{
	constexpr std::uint32_t DecrementerInterruptEnable = 0x04000000U;
	constexpr std::uint32_t DecrementerAutoReloadEnable = 0x00400000U;
	constexpr std::uint32_t DecrementerInterruptStatus = 0x08000000U;
	constexpr std::uint32_t TimeBaseClockDivider = 4U;

	std::uint32_t ReadTimerControl()
	{
		std::uint32_t value;
		asm volatile("mfspr %0, 340" : "=r"(value));
		return value;
	}

	void WriteTimerControl(const std::uint32_t value)
	{
		asm volatile("mtspr 340, %0" :: "r"(value) : "memory");
	}

	void WriteDecrementer(const std::uint32_t value)
	{
		asm volatile("mtspr 22, %0" :: "r"(value) : "memory");
	}

	void ClearDecrementerInterrupt()
	{
		// TSR is write-one-to-clear. Do not disturb the core watchdog or FIT
		// status bits owned by their respective services.
		asm volatile(
			"mtspr 336, %0"
			:
			: "r"(DecrementerInterruptStatus)
			: "memory");
	}
}

namespace MPC5xxx
{
	MPC5xxxTimerService MPC5xxxTimerService::_instance;
	bool MPC5xxxTimerService::_initialized = false;

	MPC5xxxTimerService& MPC5xxxTimerService::Instance()
	{
		return _instance;
	}

	bool MPC5xxxTimerService::Initialize()
	{
		if (!MPC5xxxSystemClockService::Ready())
			return false;

		// Prevent an inherited or immediately-expiring decrementer value from
		// firing as soon as the application enables interrupts.
		WriteDecrementer(static_cast<std::uint32_t>(
			std::numeric_limits<std::int32_t>::max()));
		ClearDecrementerInterrupt();

		// Preserve the core watchdog and fixed-interval timer configuration.
		// Scheduling is one-shot, so inherited decrementer auto-reload must be
		// disabled before DIE is enabled.
		std::uint32_t timerControl = ReadTimerControl();
		timerControl &= ~DecrementerAutoReloadEnable;
		timerControl |= DecrementerInterruptEnable;
		_initialized = true;
		WriteTimerControl(timerControl);
		return true;
	}

	tick_t MPC5xxxTimerService::GetTick()
	{
		tick_t value;
		asm volatile("mftb %0" : "=r"(value));
		return value;
	}

	tick_t MPC5xxxTimerService::GetTicksPerSecond()
	{
		return MPC5xxxSystemClockService::SystemClockHz() /
			TimeBaseClockDivider;
	}

	void MPC5xxxTimerService::ScheduleCallBack(const tick_t tick)
	{
		ClearDecrementerInterrupt();

		const tick_t now = GetTick();
		tick_t delay = tick - now;
		// A target at or behind the current time must still produce a future
		// zero-to-negative decrementer transition. ITimerService only supports
		// deadlines less than half a tick range into the future.
		if (delay == 0U || (delay & 0x80000000U) != 0U)
			delay = 1U;
		WriteDecrementer(delay);
	}

	void MPC5xxxTimerService::TimerInterrupt()
	{
		ClearDecrementerInterrupt();
		if (_initialized)
			ReturnCallBack();
	}
}

extern "C" void Decrementer_Handler()
{
	MPC5xxx::MPC5xxxTimerService::Instance().TimerInterrupt();
}
