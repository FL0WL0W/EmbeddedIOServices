#include "MPC5xxxDigitalService.h"

namespace
{
	constexpr size_t PinCount = sizeof(SIU.PCR) / sizeof(SIU.PCR[0]);

	bool IsPinInRange(EmbeddedIOServices::digitalpin_t pin)
	{
		return pin < PinCount;
	}
}

namespace MPC5xxx
{
	void MPC5xxxDigitalService::InitPin(EmbeddedIOServices::digitalpin_t pin,
		EmbeddedIOServices::PinDirection direction)
	{
		if (!IsPinInRange(pin))
			return;

		// PA == 0 selects GPIO on both the MPC5566 and MPC5674F.
		SIU.PCR[pin].B.PA = 0;
		SIU.PCR[pin].B.OBE = direction == EmbeddedIOServices::Out;
		SIU.PCR[pin].B.IBE = direction == EmbeddedIOServices::In;
	}

	bool MPC5xxxDigitalService::ReadPin(EmbeddedIOServices::digitalpin_t pin)
	{
		if (!IsPinInRange(pin))
			return false;

#if defined(MPC5674F)
		// The extended input window covers all 512 GPIO numbers. The first 256
		// inputs are aliases of the legacy GPDI window.
		return SIU.GPDI0_511[pin].B.PDI != 0;
#else
		return SIU.GPDI[pin].B.PDI != 0;
#endif
	}

	void MPC5xxxDigitalService::WritePin(EmbeddedIOServices::digitalpin_t pin, bool value)
	{
		if (!IsPinInRange(pin))
			return;

		SIU.GPDO[pin].B.PDO = value;
	}

	void MPC5xxxDigitalService::AttachInterrupt(EmbeddedIOServices::digitalpin_t pin,
		EmbeddedIOServices::callback_t callBack)
	{
		(void)pin;
		(void)callBack;
	}

	void MPC5xxxDigitalService::DetachInterrupt(EmbeddedIOServices::digitalpin_t pin)
	{
		(void)pin;
	}
}
