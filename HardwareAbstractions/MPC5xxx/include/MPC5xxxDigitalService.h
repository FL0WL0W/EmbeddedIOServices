#include "IDigitalService.h"
#include "MPC5xxx.h"

#ifndef MPC5XXXDIGITALSERVICE_H
#define MPC5XXXDIGITALSERVICE_H

namespace MPC5xxx
{
	class MPC5xxxDigitalService : public EmbeddedIOServices::IDigitalService
	{
	public:
		void InitPin(EmbeddedIOServices::digitalpin_t pin,
			EmbeddedIOServices::PinDirection direction) override;
		bool ReadPin(EmbeddedIOServices::digitalpin_t pin) override;
		void WritePin(EmbeddedIOServices::digitalpin_t pin, bool value) override;

		// Interrupt support is intentionally deferred.
		void AttachInterrupt(EmbeddedIOServices::digitalpin_t pin,
			EmbeddedIOServices::callback_t callBack) override;
		void DetachInterrupt(EmbeddedIOServices::digitalpin_t pin) override;
	};
}

#endif
