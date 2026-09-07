#include "MPC5xxxDigitalService.h"
#include "MPC5xxxETPUGPIOCode.h"

#include <cstddef>
#include <cstdint>
#include <utility>

namespace
{
	constexpr std::size_t PinCount = sizeof(SIU.PCR) / sizeof(SIU.PCR[0]);
	constexpr std::uint8_t ExternalInterruptCount = 16U;
	constexpr std::uint8_t InvalidInterrupt = 0xFFU;
	constexpr std::uint8_t DigitalInterruptPriority = 2U;
	constexpr std::uint16_t ExternalInterrupt0Vector = 46U;
	constexpr std::uint16_t ExternalInterrupts4To15Vector = 50U;
	constexpr std::uint16_t ETPUAInterrupt0Vector = 68U;
	constexpr std::uint16_t ETPUBInterrupt0Vector = 243U;
	constexpr std::uint32_t GroupedExternalInterruptMask = 0xFFF0U;
	constexpr std::size_t ETPUGlobalBytes = 16U;
	constexpr std::uint32_t ETPUTCR1SystemClockDiv2 = 0x00008000U;
	constexpr std::uint8_t ETPUHighPriority = 3U;

	struct ETPUChannel
	{
		std::uint8_t engine;
		std::uint8_t channel;
		std::uint8_t pinAlternateFunction;
		bool valid;
	};

	struct ExternalInterruptPin
	{
		EmbeddedIOServices::digitalpin_t pin;
		std::uint8_t interrupt;
	};

	struct ETPUPinRange
	{
		EmbeddedIOServices::digitalpin_t firstPin;
		EmbeddedIOServices::digitalpin_t lastPin;
		std::uint8_t engine;
		std::uint8_t firstChannel;
		std::uint8_t pinAlternateFunction;
	};

#if defined(MPC5674F)
	// MPC5674F retains the legacy MPC55xx IRQ pads and adds GPIO450-455 as
	// alternate pads for IRQ0-5. Both pad sets feed the same SIU IRQ sources.
	constexpr ExternalInterruptPin ExternalInterruptPins[] = {
		{193U, 0U}, {194U, 1U}, {211U, 2U}, {212U, 3U},
		{208U, 4U}, {209U, 5U}, {146U, 6U}, {113U, 7U},
		{134U, 8U}, {135U, 9U}, {136U, 10U}, {137U, 11U},
		{138U, 12U}, {139U, 13U}, {140U, 14U}, {141U, 15U},
		{450U, 0U}, {451U, 1U}, {452U, 2U}, {453U, 3U},
		{454U, 4U}, {455U, 5U}
	};

	constexpr ETPUPinRange ETPUPinRanges[] = {
		{114U, 133U, 0U, 0U, 1U},
		// eTPUA24-27 share GPIO134-137 with the lower-latency SIU IRQ8-11 path.
		// GPIO142/143 require their ISEL8 pad-input selections below.
		{142U, 145U, 0U, 28U, 1U},
		{147U, 178U, 1U, 0U, 1U}
	};
#elif defined(MPC5566)
	constexpr ExternalInterruptPin ExternalInterruptPins[] = {
		{193U, 0U}, {194U, 1U}, {211U, 2U}, {212U, 3U},
		{208U, 4U}, {209U, 5U}, {146U, 6U}, {113U, 7U},
		{134U, 8U}, {135U, 9U}, {136U, 10U}, {137U, 11U},
		{138U, 12U}, {139U, 13U}, {140U, 14U}, {141U, 15U}
	};

	constexpr ETPUPinRange ETPUPinRanges[] = {
		{114U, 133U, 0U, 0U, 1U},
		{144U, 145U, 0U, 30U, 1U},
		{147U, 178U, 1U, 0U, 1U}
	};
#endif

	EmbeddedIOServices::callback_t SIUInterruptCallbacks[ExternalInterruptCount];
	EmbeddedIOServices::callback_t ETPUInterruptCallbacks[2][32];
	bool ETPUInitialized = false;

	bool IsPinInRange(EmbeddedIOServices::digitalpin_t pin)
	{
		return pin < PinCount;
	}

	std::uint8_t PinToExternalInterrupt(EmbeddedIOServices::digitalpin_t pin)
	{
		for (const ExternalInterruptPin& route : ExternalInterruptPins)
			if (route.pin == pin) return route.interrupt;
		return InvalidInterrupt;
	}

	ETPUChannel PinToETPUChannel(EmbeddedIOServices::digitalpin_t pin)
	{
		for (const ETPUPinRange& route : ETPUPinRanges)
		{
			if (pin >= route.firstPin && pin <= route.lastPin)
				return {route.engine,
					static_cast<std::uint8_t>(route.firstChannel + pin - route.firstPin),
					route.pinAlternateFunction, true};
		}
		return {0U, 0U, 0U, false};
	}

	void SelectETPUInputPad(const ETPUChannel& channel, bool selectPad)
	{
#if defined(MPC5674F)
		if (channel.engine != 0U) return;
		switch (channel.channel)
		{
			case 24U: SIU.ISEL8.B.ETPU24 = selectPad; break;
			case 25U: SIU.ISEL8.B.ETPU25 = selectPad; break;
			case 26U: SIU.ISEL8.B.ETPU26 = selectPad; break;
			case 27U: SIU.ISEL8.B.ETPU27 = selectPad; break;
			case 28U: SIU.ISEL8.B.ETPU28 = selectPad; break;
			case 29U: SIU.ISEL8.B.ETPU29 = selectPad; break;
			default: break;
		}
#else
		(void)channel;
		(void)selectPad;
#endif
	}

	std::uint32_t DisableExternalInterrupts()
	{
		std::uint32_t machineState;
		asm volatile("mfmsr %0\n\twrteei 0\n\tisync" : "=r"(machineState) :: "memory");
		return machineState;
	}

	void RestoreExternalInterrupts(std::uint32_t machineState)
	{
		if ((machineState & 0x00008000U) != 0U)
			asm volatile("wrteei 1\n\tisync" ::: "memory");
	}

	bool InitializeETPU()
	{
		if (ETPUInitialized) return true;

		const std::uint32_t machineState = DisableExternalInterrupts();
		ETPU.ECR_A.B.MDIS = 1U;
		ETPU.ECR_B.B.MDIS = 1U;
		std::uint32_t timeout = 100000U;
		while ((ETPU.ECR_A.B.STF == 0U || ETPU.ECR_B.B.STF == 0U) && --timeout != 0U) {}
		if (timeout == 0U)
		{
			RestoreExternalInterrupts(machineState);
			return false;
		}

		const std::size_t codeBytes = MPC5xxx::ETPUGPIOCode::CodeWordCount * sizeof(std::uint32_t);
		const std::size_t codeRAMBytes = (static_cast<std::size_t>(ETPU.MCR.B.SCMSIZE) + 1U) * 2048U;
		if (codeBytes > codeRAMBytes)
		{
			RestoreExternalInterrupts(machineState);
			return false;
		}

		ETPU.MCR.B.VIS = 1U;
		timeout = 100U;
		while (ETPU.MCR.B.VIS == 0U && --timeout != 0U) {}
		if (timeout == 0U)
		{
			RestoreExternalInterrupts(machineState);
			return false;
		}

		volatile std::uint32_t* const codeRAM = &ETPU_CODE_RAM;
		for (std::size_t word = 0U; word < MPC5xxx::ETPUGPIOCode::CodeWordCount; ++word)
			codeRAM[word] = MPC5xxx::ETPUGPIOCode::Code[word];
		for (std::size_t word = MPC5xxx::ETPUGPIOCode::CodeWordCount;
			word < codeRAMBytes / sizeof(std::uint32_t); ++word)
			codeRAM[word] = 0U;

		ETPU.MCR.B.VIS = 0U;
		ETPU.MISCCMPR.R = MPC5xxx::ETPUGPIOCode::MiscSignature;
		ETPU.MCR.R = 0U;
		ETPU.ECR_A.R = 0U;
		ETPU.ECR_B.R = 0U;
		ETPU.TBCR_A.R = ETPUTCR1SystemClockDiv2;
		ETPU.TBCR_B.R = ETPUTCR1SystemClockDiv2;
		ETPU.REDCR_A.R = 0U;
		ETPU.REDCR_B.R = 0U;
		ETPU.CIER_A.R = 0U;
		ETPU.CIER_B.R = 0U;
		ETPU.CDTRER_A.R = 0U;
		ETPU.CDTRER_B.R = 0U;
		ETPU.CISR_A.R = 0xFFFFFFFFU;
		ETPU.CISR_B.R = 0xFFFFFFFFU;
		ETPU.CDTRSR_A.R = 0xFFFFFFFFU;
		ETPU.CDTRSR_B.R = 0xFFFFFFFFU;

		volatile std::uint32_t* const dataRAM = &ETPU_DATA_RAM;
		const std::size_t dataRAMBytes = ETPU_DATA_RAM_END -
			reinterpret_cast<std::uintptr_t>(dataRAM) + sizeof(std::uint32_t);
		for (std::size_t word = 0U; word < dataRAMBytes / sizeof(std::uint32_t); ++word)
			dataRAM[word] = 0U;
		for (std::size_t word = 0U; word < MPC5xxx::ETPUGPIOCode::GlobalWordCount; ++word)
			dataRAM[word] = MPC5xxx::ETPUGPIOCode::Globals[word];

		ETPU.MCR.B.GTBE = 1U;
		asm volatile("mbar" ::: "memory");
		ETPUInitialized = true;
		RestoreExternalInterrupts(machineState);
		return true;
	}

	std::uint16_t ETPUVector(const ETPUChannel& channel)
	{
		return static_cast<std::uint16_t>(
			(channel.engine == 0U ? ETPUAInterrupt0Vector : ETPUBInterrupt0Vector) + channel.channel);
	}

	volatile std::uint32_t* ETPUParameterFrame(const ETPUChannel& channel)
	{
		const std::size_t linearChannel = static_cast<std::size_t>(channel.engine) * 32U + channel.channel;
		return reinterpret_cast<volatile std::uint32_t*>(
			reinterpret_cast<std::uintptr_t>(&ETPU_DATA_RAM) + ETPUGlobalBytes +
			linearChannel * MPC5xxx::ETPUGPIOCode::ParameterBytes);
	}

	void ServiceSIUInterrupts(std::uint32_t interruptMask)
	{
		const std::uint32_t pending = SIU.EISR.R & SIU.DIRER.R & interruptMask;
		SIU.EISR.R = pending;
		asm volatile("mbar" ::: "memory");
		for (std::uint8_t interrupt = 0U; interrupt < ExternalInterruptCount; ++interrupt)
		{
			const std::uint32_t mask = 1UL << interrupt;
			if ((pending & mask) != 0U && SIUInterruptCallbacks[interrupt])
				SIUInterruptCallbacks[interrupt]();
		}
	}

	void ServiceETPUInterrupt(std::uint8_t engine, std::uint8_t channel)
	{
		const std::uint32_t mask = 1UL << channel;
		if (engine == 0U)
		{
			ETPU.CISR_A.R = mask;
			ETPU.CDTRSR_A.R = mask;
			ETPU.CIOSR_A.R = mask;
			ETPU.CDTROSR_A.R = mask;
		}
		else
		{
			ETPU.CISR_B.R = mask;
			ETPU.CDTRSR_B.R = mask;
			ETPU.CIOSR_B.R = mask;
			ETPU.CDTROSR_B.R = mask;
		}
		asm volatile("mbar" ::: "memory");
		if (ETPUInterruptCallbacks[engine][channel])
			ETPUInterruptCallbacks[engine][channel]();
	}
}

namespace MPC5xxx
{
	void MPC5xxxDigitalService::InitPin(EmbeddedIOServices::digitalpin_t pin,
		EmbeddedIOServices::PinDirection direction)
	{
		if (!IsPinInRange(pin)) return;
		SIU.PCR[pin].B.PA = 0U;
		SIU.PCR[pin].B.OBE = direction == EmbeddedIOServices::Out;
		SIU.PCR[pin].B.IBE = direction == EmbeddedIOServices::In;
	}

	bool MPC5xxxDigitalService::ReadPin(EmbeddedIOServices::digitalpin_t pin)
	{
		if (!IsPinInRange(pin)) return false;
#if defined(MPC5674F)
		return SIU.GPDI0_511[pin].B.PDI != 0;
#else
		return SIU.GPDI[pin].B.PDI != 0;
#endif
	}

	void MPC5xxxDigitalService::WritePin(EmbeddedIOServices::digitalpin_t pin, bool value)
	{
		if (IsPinInRange(pin)) SIU.GPDO[pin].B.PDO = value;
	}

	void MPC5xxxDigitalService::AttachInterrupt(EmbeddedIOServices::digitalpin_t pin,
		EmbeddedIOServices::callback_t callBack)
	{
		if (!IsPinInRange(pin)) return;
		if (!callBack)
		{
			DetachInterrupt(pin);
			return;
		}

		const std::uint8_t interrupt = PinToExternalInterrupt(pin);
		if (interrupt != InvalidInterrupt)
		{
			const std::uint32_t machineState = DisableExternalInterrupts();
			const std::uint32_t mask = 1UL << interrupt;
			SIU.DIRER.R &= ~mask;
			SIUInterruptCallbacks[interrupt] = std::move(callBack);
			SIU.PCR[pin].B.PA = 2U;
			SIU.PCR[pin].B.OBE = 0U;
			SIU.PCR[pin].B.IBE = 1U;
			SIU.EIISR.R &= ~(3UL << (interrupt * 2U));
			if (interrupt < 4U) SIU.DIRSR.R &= ~mask;
			SIU.IREER.R |= mask;
			SIU.IFEER.R |= mask;
			SIU.EISR.R = mask;
			const std::uint16_t vector = interrupt < 4U
				? static_cast<std::uint16_t>(ExternalInterrupt0Vector + interrupt)
				: ExternalInterrupts4To15Vector;
			INTC.PSR[vector].R = DigitalInterruptPriority;
			SIU.DIRER.R |= mask;
			asm volatile("mbar" ::: "memory");
			RestoreExternalInterrupts(machineState);
			return;
		}

		const ETPUChannel channel = PinToETPUChannel(pin);
		if (!channel.valid || !InitializeETPU()) return;

		const std::uint32_t machineState = DisableExternalInterrupts();
		const std::uint32_t mask = 1UL << channel.channel;
		const std::size_t hardwareChannel = channel.engine == 0U ? channel.channel : channel.channel + 64U;
		volatile std::uint32_t* const parameters = ETPUParameterFrame(channel);
		for (std::size_t word = 0U; word < ETPUGPIOCode::ParameterBytes / sizeof(std::uint32_t); ++word)
			parameters[word] = 0U;

		if (channel.engine == 0U) ETPU.CIER_A.R &= ~mask;
		else ETPU.CIER_B.R &= ~mask;
		ETPUInterruptCallbacks[channel.engine][channel.channel] = std::move(callBack);
		SelectETPUInputPad(channel, true);
		SIU.PCR[pin].B.PA = channel.pinAlternateFunction;
		SIU.PCR[pin].B.OBE = 0U;
		SIU.PCR[pin].B.IBE = 1U;

		const std::uintptr_t parameterOffset = reinterpret_cast<std::uintptr_t>(parameters) -
			reinterpret_cast<std::uintptr_t>(&ETPU_DATA_RAM);
		ETPU.CHAN[hardwareChannel].CR.R =
			(static_cast<std::uint32_t>(ETPUHighPriority) << 28U) |
			(static_cast<std::uint32_t>(ETPUGPIOCode::FunctionNumber) << 16U) |
			static_cast<std::uint32_t>(parameterOffset >> 3U);
		ETPU.CHAN[hardwareChannel].SCR.R = 0U;
		ETPU.CHAN[hardwareChannel].HSRR.R = ETPUGPIOCode::InputEitherEdgesHSR;

		std::uint32_t timeout = 100000U;
		while (ETPU.CHAN[hardwareChannel].HSRR.R != 0U && --timeout != 0U) {}
		if (timeout == 0U)
		{
			ETPU.CHAN[hardwareChannel].CR.B.CPR = 0U;
			ETPUInterruptCallbacks[channel.engine][channel.channel] = nullptr;
			SelectETPUInputPad(channel, false);
			SIU.PCR[pin].B.PA = 0U;
			SIU.PCR[pin].B.IBE = 1U;
			RestoreExternalInterrupts(machineState);
			return;
		}
		if (channel.engine == 0U)
		{
			ETPU.CISR_A.R = mask;
			ETPU.CDTRSR_A.R = mask;
			ETPU.CIER_A.R |= mask;
		}
		else
		{
			ETPU.CISR_B.R = mask;
			ETPU.CDTRSR_B.R = mask;
			ETPU.CIER_B.R |= mask;
		}
		INTC.PSR[ETPUVector(channel)].R = DigitalInterruptPriority;
		asm volatile("mbar" ::: "memory");
		RestoreExternalInterrupts(machineState);
	}

	void MPC5xxxDigitalService::DetachInterrupt(EmbeddedIOServices::digitalpin_t pin)
	{
		if (!IsPinInRange(pin)) return;
		const std::uint8_t interrupt = PinToExternalInterrupt(pin);
		if (interrupt != InvalidInterrupt)
		{
			const std::uint32_t machineState = DisableExternalInterrupts();
			const std::uint32_t mask = 1UL << interrupt;
			SIU.DIRER.R &= ~mask;
			SIU.IREER.R &= ~mask;
			SIU.IFEER.R &= ~mask;
			SIU.EISR.R = mask;
			SIUInterruptCallbacks[interrupt] = nullptr;
			if (interrupt < 4U) INTC.PSR[ExternalInterrupt0Vector + interrupt].R = 0U;
			else if ((SIU.DIRER.R & GroupedExternalInterruptMask) == 0U)
				INTC.PSR[ExternalInterrupts4To15Vector].R = 0U;
			asm volatile("mbar" ::: "memory");
			RestoreExternalInterrupts(machineState);
			return;
		}

		const ETPUChannel channel = PinToETPUChannel(pin);
		if (!channel.valid || !ETPUInitialized) return;
		const std::uint32_t machineState = DisableExternalInterrupts();
		const std::uint32_t mask = 1UL << channel.channel;
		const std::size_t hardwareChannel = channel.engine == 0U ? channel.channel : channel.channel + 64U;
		if (channel.engine == 0U) ETPU.CIER_A.R &= ~mask;
		else ETPU.CIER_B.R &= ~mask;
		ETPU.CHAN[hardwareChannel].CR.B.CPR = 0U;
		ETPUInterruptCallbacks[channel.engine][channel.channel] = nullptr;
		INTC.PSR[ETPUVector(channel)].R = 0U;
		SelectETPUInputPad(channel, false);
		SIU.PCR[pin].B.PA = 0U;
		SIU.PCR[pin].B.IBE = 1U;
		asm volatile("mbar" ::: "memory");
		RestoreExternalInterrupts(machineState);
	}
}

extern "C" void SIU_ExternalInterrupt0_Handler() { ServiceSIUInterrupts(1UL << 0U); }
extern "C" void SIU_ExternalInterrupt1_Handler() { ServiceSIUInterrupts(1UL << 1U); }
extern "C" void SIU_ExternalInterrupt2_Handler() { ServiceSIUInterrupts(1UL << 2U); }
extern "C" void SIU_ExternalInterrupt3_Handler() { ServiceSIUInterrupts(1UL << 3U); }
extern "C" void SIU_ExternalInterrupts4To15_Handler() { ServiceSIUInterrupts(GroupedExternalInterruptMask); }

#define DEFINE_ETPU_HANDLER(engineName, engineIndex, channel) \
	extern "C" void ETPU_##engineName##_Channel##channel##_Handler() \
	{ ServiceETPUInterrupt(engineIndex, channel); }

#define DEFINE_ETPU_ENGINE_HANDLERS(engineName, engineIndex) \
	DEFINE_ETPU_HANDLER(engineName, engineIndex, 0)  DEFINE_ETPU_HANDLER(engineName, engineIndex, 1)  \
	DEFINE_ETPU_HANDLER(engineName, engineIndex, 2)  DEFINE_ETPU_HANDLER(engineName, engineIndex, 3)  \
	DEFINE_ETPU_HANDLER(engineName, engineIndex, 4)  DEFINE_ETPU_HANDLER(engineName, engineIndex, 5)  \
	DEFINE_ETPU_HANDLER(engineName, engineIndex, 6)  DEFINE_ETPU_HANDLER(engineName, engineIndex, 7)  \
	DEFINE_ETPU_HANDLER(engineName, engineIndex, 8)  DEFINE_ETPU_HANDLER(engineName, engineIndex, 9)  \
	DEFINE_ETPU_HANDLER(engineName, engineIndex, 10) DEFINE_ETPU_HANDLER(engineName, engineIndex, 11) \
	DEFINE_ETPU_HANDLER(engineName, engineIndex, 12) DEFINE_ETPU_HANDLER(engineName, engineIndex, 13) \
	DEFINE_ETPU_HANDLER(engineName, engineIndex, 14) DEFINE_ETPU_HANDLER(engineName, engineIndex, 15) \
	DEFINE_ETPU_HANDLER(engineName, engineIndex, 16) DEFINE_ETPU_HANDLER(engineName, engineIndex, 17) \
	DEFINE_ETPU_HANDLER(engineName, engineIndex, 18) DEFINE_ETPU_HANDLER(engineName, engineIndex, 19) \
	DEFINE_ETPU_HANDLER(engineName, engineIndex, 20) DEFINE_ETPU_HANDLER(engineName, engineIndex, 21) \
	DEFINE_ETPU_HANDLER(engineName, engineIndex, 22) DEFINE_ETPU_HANDLER(engineName, engineIndex, 23) \
	DEFINE_ETPU_HANDLER(engineName, engineIndex, 24) DEFINE_ETPU_HANDLER(engineName, engineIndex, 25) \
	DEFINE_ETPU_HANDLER(engineName, engineIndex, 26) DEFINE_ETPU_HANDLER(engineName, engineIndex, 27) \
	DEFINE_ETPU_HANDLER(engineName, engineIndex, 28) DEFINE_ETPU_HANDLER(engineName, engineIndex, 29) \
	DEFINE_ETPU_HANDLER(engineName, engineIndex, 30) DEFINE_ETPU_HANDLER(engineName, engineIndex, 31)

DEFINE_ETPU_ENGINE_HANDLERS(A, 0U)
DEFINE_ETPU_ENGINE_HANDLERS(B, 1U)

#undef DEFINE_ETPU_ENGINE_HANDLERS
#undef DEFINE_ETPU_HANDLER
