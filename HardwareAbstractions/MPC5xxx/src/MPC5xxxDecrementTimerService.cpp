#include "MPC5xxxDecrementTimerService.h"

#include "MPC5xxxSystemClockService.h"

#include <cstdint>
#include <limits>

using namespace EmbeddedIOServices;

namespace
{
	constexpr std::uint32_t DecrementerInterruptEnable = 0x04000000U;
	constexpr std::uint32_t DecrementerAutoReloadEnable = 0x00400000U;
	constexpr std::uint32_t DecrementerInterruptStatus = 0x08000000U;
	constexpr std::uint32_t ExternalInterruptEnable = 0x00008000U;

	std::uint32_t DisableExternalInterrupts()
	{
		std::uint32_t machineState;
		asm volatile(
			"mfmsr %0\n\twrteei 0\n\tisync"
			: "=r"(machineState)
			:
			: "memory");
		return machineState;
	}

	void RestoreExternalInterrupts(const std::uint32_t machineState)
	{
		if ((machineState & ExternalInterruptEnable) != 0U)
			asm volatile("wrteei 1\n\tisync" ::: "memory");
	}

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
}

namespace MPC5xxx
{
	MPC5xxxDecrementTimerService MPC5xxxDecrementTimerService::_instance;

	MPC5xxxDecrementTimerService& MPC5xxxDecrementTimerService::Instance()
	{
		return _instance;
	}

	bool MPC5xxxDecrementTimerService::Initialize()
	{
		if (!MPC5xxxSystemClockService::Ready())
			return false;

		// Prevent an inherited or immediately-expiring decrementer value from
		// firing as soon as the application enables interrupts.
		WriteDecrementer(static_cast<std::uint32_t>(
			std::numeric_limits<std::int32_t>::max()));

		// Preserve the core watchdog and fixed-interval timer configuration.
		// Scheduling is one-shot, so inherited decrementer auto-reload must be
		// disabled before DIE is enabled.
		std::uint32_t timerControl = ReadTimerControl();
		timerControl &= ~DecrementerAutoReloadEnable;
		timerControl |= DecrementerInterruptEnable;
		WriteTimerControl(timerControl);
		return true;
	}

	tick_t MPC5xxxDecrementTimerService::GetTick()
	{
		tick_t value;
		asm volatile("mftb %0" : "=r"(value));
		return value;
	}

	tick_t MPC5xxxDecrementTimerService::GetTicksPerSecond()
	{
		return MPC5xxxSystemClockService::SystemClockHz();
	}

	void MPC5xxxDecrementTimerService::ScheduleCallBack(const tick_t tick)
	{
		// Keep the time-base snapshot and decrementer write together: a
		// higher-priority ISR between them would make the delay stale.
		const std::uint32_t machineState = DisableExternalInterrupts();

		const tick_t now = GetTick();
		tick_t delay = tick - now;
		// A target at or behind the current time must still produce a future
		// zero-to-negative decrementer transition. ITimerService only supports
		// deadlines less than half a tick range into the future.
		if (delay == 0U || (delay & 0x80000000U) != 0U)
			delay = 1U;
		WriteDecrementer(delay);
		RestoreExternalInterrupts(machineState);
	}

	void MPC5xxxDecrementTimerService::TimerInterrupt()
	{
		ReturnCallBack();
	}
}

extern "C" void Decrementer_Handler()
{
	MPC5xxx::MPC5xxxDecrementTimerService::Instance().TimerInterrupt();
}
