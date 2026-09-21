#include "IDigitalService.h"
#include "MPC5xxx.h"

#ifndef MPC5XXXDIGITALSERVICE_H
#define MPC5XXXDIGITALSERVICE_H

namespace MPC5xxx
{
	class MPC5xxxDigitalService : public EmbeddedIOServices::IDigitalService
	{
	private:
		const std::uint8_t _interruptPriority;

	public:
		/**
		 * @param interruptPriority INTC priority used by attached SIU/eTPU
		 * interrupts. Zero leaves the sources unavailable to the CPU so pins may
		 * instead be sampled with ReadPin().
		 */
		explicit MPC5xxxDigitalService(std::uint8_t interruptPriority);

		void InitPin(EmbeddedIOServices::digitalpin_t pin,
			EmbeddedIOServices::PinDirection direction) override;
		bool ReadPin(EmbeddedIOServices::digitalpin_t pin) override;
		void WritePin(EmbeddedIOServices::digitalpin_t pin, bool value) override;

		void AttachInterrupt(EmbeddedIOServices::digitalpin_t pin,
			EmbeddedIOServices::callback_t callBack) override;
		void DetachInterrupt(EmbeddedIOServices::digitalpin_t pin) override;
	};
}

#endif
