#ifndef MPC5XXXDECREMENTTIMERSERVICE_H
#define MPC5XXXDECREMENTTIMERSERVICE_H

#include "ITimerService.h"

namespace MPC5xxx
{
	/**
	 * Timer service backed by the e200 time base and decrementer.
	 *
	 * The time base supplies the free-running 32-bit tick. The decrementer is
	 * programmed as a one-shot alarm and enters through IVOR10 when it expires.
	 * There is one time base/decrementer pair per core, so this service is a
	 * singleton.
	 */
	class MPC5xxxDecrementTimerService final : public EmbeddedIOServices::ITimerService
	{
	private:
		static MPC5xxxDecrementTimerService _instance;
		static bool _initialized;

		MPC5xxxDecrementTimerService() = default;

		void ScheduleCallBack(EmbeddedIOServices::tick_t tick) override;

	public:
		static MPC5xxxDecrementTimerService& Instance();

		/**
		 * Enables the core decrementer interrupt and leaves its first expiration
		 * safely in the future. Global external interrupts need not be enabled.
		 * Calibrate() must be called separately after interrupts are enabled.
		 */
		static bool Initialize();

		EmbeddedIOServices::tick_t GetTick() override;
		EmbeddedIOServices::tick_t GetTicksPerSecond() override;

		/** Called by the IVOR10 C handler. */
		void TimerInterrupt();
	};
}

#endif
