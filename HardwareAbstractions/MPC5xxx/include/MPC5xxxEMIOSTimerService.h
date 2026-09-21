#ifndef MPC5XXXEMIOSTIMERSERVICE_H
#define MPC5XXXEMIOSTIMERSERVICE_H

#include "ITimerService.h"

#include <cstdint>

namespace MPC5xxx
{
	/**
	 * Timer service backed by eMIOS counter bus A.
	 *
	 * The e200 time base supplies the public 32-bit tick. The selected eMIOS
	 * channel is armed as a relative, one-shot modulus up-counter for each
	 * scheduler deadline.
	 */
	class MPC5xxxEMIOSTimerService final : public EmbeddedIOServices::ITimerService
	{
	private:
		static constexpr std::uint8_t ChannelCount = 24U;
		static MPC5xxxEMIOSTimerService* _instances[ChannelCount];
		static bool _moduleInitialized;

		const std::uint8_t _channel;
		volatile bool _alarmArmed = false;
		bool _valid = false;

		void ScheduleCallBack(EmbeddedIOServices::tick_t tick) override;

	public:
		MPC5xxxEMIOSTimerService(std::uint8_t channel, std::uint8_t interruptPriority);
		MPC5xxxEMIOSTimerService(const MPC5xxxEMIOSTimerService&) = delete;
		MPC5xxxEMIOSTimerService& operator=(const MPC5xxxEMIOSTimerService&) = delete;

		EmbeddedIOServices::tick_t GetTick() override;
		EmbeddedIOServices::tick_t GetTicksPerSecond() override;

		void CompareInterrupt();
		static void HandleInterrupt(std::uint8_t channel);
	};
}

#endif
