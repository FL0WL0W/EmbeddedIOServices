#include "MPC5xxxEMIOSTimerService.h"

#include "MPC5xxxSystemClockService.h"

#include <cstdint>

using namespace EmbeddedIOServices;

namespace
{
	constexpr tick_t MaximumCounterDelay = 0x00FFFFFFU;
	constexpr std::uint32_t ClearChannelStatus = 0x80008001U;
	// CCR[UCPREN] | CCR[FEN] | MODE=MC up-counter, internal clock.
	constexpr std::uint32_t ModulusCounterConfiguration =
		0x02000000U | 0x00020000U | 0x00000010U;

	constexpr std::uint32_t ExternalInterruptEnable = 0x00008000U;
	constexpr std::uint32_t DecrementerInterruptEnable = 0x04000000U;

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

	tick_t ReadTimeBase()
	{
		tick_t tick;
		asm volatile("mftb %0" : "=r"(tick));
		return tick;
	}

	void DisableCoreDecrementerInterrupt()
	{
		std::uint32_t timerControl;
		asm volatile("mfspr %0, 340" : "=r"(timerControl));
		timerControl &= ~DecrementerInterruptEnable;
		asm volatile("mtspr 340, %0" :: "r"(timerControl) : "memory");
	}

	constexpr std::uint16_t InterruptVectorForChannel(const std::uint8_t channel)
	{
		return channel < 16U
			? static_cast<std::uint16_t>(51U + channel)
			: static_cast<std::uint16_t>(202U + channel - 16U);
	}
}

namespace MPC5xxx
{
	MPC5xxxEMIOSTimerService*
		MPC5xxxEMIOSTimerService::_instances[MPC5xxxEMIOSTimerService::ChannelCount] = {};

	MPC5xxxEMIOSTimerService::MPC5xxxEMIOSTimerService(
		const std::uint8_t channel,
		const std::uint8_t interruptPriority)
		: _channel(channel)
	{
		if (channel >= ChannelCount || !MPC5xxxSystemClockService::Ready())
			return;

		const std::uint32_t machineState = DisableExternalInterrupts();
		DisableCoreDecrementerInterrupt();

		const std::uint16_t interruptVector = InterruptVectorForChannel(channel);
		INTC.PSR[interruptVector].R = 0U;
		EMIOS.CH[channel].CCR.R = 0U;
		EMIOS.CH[channel].CSR.R = ClearChannelStatus;

		if(!(EMIOS.MCR.B.GPREN == 1U && EMIOS.MCR.B.MDIS == 0U && EMIOS.MCR.B.GTBE == 1U && EMIOS.MCR.B.GPRE == 0U))
		{
			// Configure the shared module clock once. Later timer instances can be
			// attached without pausing channels that are already running.
			EMIOS.MCR.B.GPREN = 0U;
			EMIOS.MCR.B.MDIS = 0U;
			EMIOS.MCR.B.GTBE = 1U;
			EMIOS.MCR.B.GPRE = 0U;
			EMIOS.MCR.B.GPREN = 1U;
		}

		_instances[channel] = this;
		INTC.PSR[interruptVector].R = interruptPriority;
		asm volatile("mbar" ::: "memory");
		RestoreExternalInterrupts(machineState);
	}

	tick_t MPC5xxxEMIOSTimerService::GetTick()
	{
		return ReadTimeBase();
	}

	tick_t MPC5xxxEMIOSTimerService::GetTicksPerSecond()
	{
		return MPC5xxxSystemClockService::SystemClockHz();
	}

	void MPC5xxxEMIOSTimerService::ScheduleCallBack(tick_t tick)
	{
		const std::uint32_t machineState = DisableExternalInterrupts();
		const tick_t now = ReadTimeBase();
		tick_t delay = tick - now;
		if (delay == 0U || (delay & 0x80000000U) != 0U)
			delay = 1U;
		else if (delay > MaximumCounterDelay)
			delay = MaximumCounterDelay;

		// GPIO mode stops and clears the channel counter. The reference manual
		// requires passing through GPIO mode before selecting another UC mode.
		EMIOS.CH[_channel].CCR.R = 0U;
		EMIOS.CH[_channel].CSR.R = ClearChannelStatus;
		EMIOS.CH[_channel].CADR.R = delay;
		// Entering MC mode resets the counter to zero and starts the relative
		// countdown-equivalent. A delay of one produces the earliest hardware
		// interrupt without any already-passed absolute compare race.
		EMIOS.CH[_channel].CCR.R = ModulusCounterConfiguration;
		asm volatile("mbar" ::: "memory");
		RestoreExternalInterrupts(machineState);
	}

	void MPC5xxxEMIOSTimerService::CompareInterrupt()
	{
		EMIOS.CH[_channel].CSR.R = ClearChannelStatus;
		asm volatile("mbar" ::: "memory");
		EMIOS.CH[_channel].CCR.R = 0U;
		ReturnCallBack();
	}

	void MPC5xxxEMIOSTimerService::HandleInterrupt(const std::uint8_t channel)
	{
		if (channel >= ChannelCount)
			return;
		MPC5xxxEMIOSTimerService* const instance = _instances[channel];
		if (instance != nullptr)
		{
			instance->CompareInterrupt();
			return;
		}

		// All eMIOS vectors are consumed by this driver. Quiesce an unowned
		// channel rather than leaving an asserted source in the default handler.
		EMIOS.CH[channel].CCR.R = 0U;
		EMIOS.CH[channel].CSR.R = ClearChannelStatus;
	}
}

#define DEFINE_EMIOS_CHANNEL_HANDLER(channel) \
	extern "C" void EMIOS_Channel##channel##_Handler() \
	{ \
		MPC5xxx::MPC5xxxEMIOSTimerService::HandleInterrupt(channel); \
	}

DEFINE_EMIOS_CHANNEL_HANDLER(0)
DEFINE_EMIOS_CHANNEL_HANDLER(1)
DEFINE_EMIOS_CHANNEL_HANDLER(2)
DEFINE_EMIOS_CHANNEL_HANDLER(3)
DEFINE_EMIOS_CHANNEL_HANDLER(4)
DEFINE_EMIOS_CHANNEL_HANDLER(5)
DEFINE_EMIOS_CHANNEL_HANDLER(6)
DEFINE_EMIOS_CHANNEL_HANDLER(7)
DEFINE_EMIOS_CHANNEL_HANDLER(8)
DEFINE_EMIOS_CHANNEL_HANDLER(9)
DEFINE_EMIOS_CHANNEL_HANDLER(10)
DEFINE_EMIOS_CHANNEL_HANDLER(11)
DEFINE_EMIOS_CHANNEL_HANDLER(12)
DEFINE_EMIOS_CHANNEL_HANDLER(13)
DEFINE_EMIOS_CHANNEL_HANDLER(14)
DEFINE_EMIOS_CHANNEL_HANDLER(15)
DEFINE_EMIOS_CHANNEL_HANDLER(16)
DEFINE_EMIOS_CHANNEL_HANDLER(17)
DEFINE_EMIOS_CHANNEL_HANDLER(18)
DEFINE_EMIOS_CHANNEL_HANDLER(19)
DEFINE_EMIOS_CHANNEL_HANDLER(20)
DEFINE_EMIOS_CHANNEL_HANDLER(21)
DEFINE_EMIOS_CHANNEL_HANDLER(22)
DEFINE_EMIOS_CHANNEL_HANDLER(23)

#undef DEFINE_EMIOS_CHANNEL_HANDLER
