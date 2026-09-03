#include "MPC5xxxAnalogService.h"

namespace
{
	constexpr uint32_t EndOfQueue = 0x80000000UL;
	constexpr uint32_t MessageTagShift = 20U;
	constexpr uint32_t LongSampleTimeShift = 18U;
	constexpr uint32_t ChannelNumberShift = 8U;
	constexpr uint32_t ADCControlRegisterAddress = 0x01U;
	constexpr uint32_t SixtyFourSampleCycles = 0x02U;

	constexpr uint16_t ADCEnable = 0x8000U;
	constexpr uint16_t ADCExternalMultiplexerEnable = 0x0800U;
	// Use the eQADC's maximum divide factor because this generic service does
	// not know the platform clock selected by its application.
	constexpr uint16_t ADCClockPrescaler = 0x001FU;

	constexpr uint16_t InvalidateFifo = 0x0200U;
	constexpr uint16_t SingleScanSoftwareTrigger = 0x0010U;
	constexpr uint16_t StartSingleScan = 0x0400U;

	constexpr uint32_t EndOfQueueFlag = 0x10000000UL;
	constexpr uint32_t ResultFifoNotEmpty = 0x00020000UL;
	// RFDF is write-one-to-clear when RFDS selects interrupt/software drain.
	// Popping the last RFIFO entry decrements RFCTR but does not clear RFDF.
	constexpr uint32_t ClearableFifoFlags = 0xF80A0000UL;

	constexpr float Maximum12BitResult = 4095.0F;
	constexpr uint16_t UncalibratedResultMask = 0x3FFCU;
	constexpr uint8_t UncalibratedResultShift = 2U;
}

namespace MPC5xxx
{
	MPC5xxxAnalogService::MPC5xxxAnalogService(volatile EQADC_tag& module,
		float referenceVoltage)
		: _module(module),
		  _referenceVoltage(referenceVoltage > 0.0F ? referenceVoltage : 5.0F)
	{
		Initialize(ADCConverter::ADC0);
		Initialize(ADCConverter::ADC1);
	}

	void MPC5xxxAnalogService::InitPin(EmbeddedIOServices::analogpin_t pin)
	{
		(void)pin;
	}

	float MPC5xxxAnalogService::ReadPin(EmbeddedIOServices::analogpin_t pin)
	{
		if (!IsValidPin(pin))
			return 0.0F;

		const ADCConverter converter = ConverterForPin(pin);

		uint16_t result = 0U;
		uint32_t command = EndOfQueue
			| (static_cast<uint32_t>(converter) << 25U)
			| (static_cast<uint32_t>(Fifo) << MessageTagShift)
			| (static_cast<uint32_t>(pin) << ChannelNumberShift);
		if (IsExternalMuxChannel(pin))
		{
			// The reference manual recommends 64 or 128 ADC clock cycles
			// so an external mux and the ADC sample capacitor can settle.
			command |= SixtyFourSampleCycles << LongSampleTimeShift;
		}

		if (!ExecuteCommand(command, true, result))
			return 0.0F;

		// With CAL clear, the eQADC returns its 12-bit conversion as a
		// right-justified 14-bit value with two zero low bits.
		const uint16_t conversion = static_cast<uint16_t>(
			(result & UncalibratedResultMask) >> UncalibratedResultShift);
		return (static_cast<float>(conversion) * _referenceVoltage)
			/ Maximum12BitResult;
	}

	void MPC5xxxAnalogService::Initialize(ADCConverter converter)
	{
		PrepareFifo();

		const uint32_t commandHeader = EndOfQueue
			| (static_cast<uint32_t>(converter) << 25U)
			| ADCControlRegisterAddress;
		uint16_t unusedResult = 0U;

		ExecuteCommand(commandHeader, false, unusedResult);

		const uint16_t control = ADCEnable | ADCClockPrescaler
			| (converter == ADCConverter::ADC1
				? ADCExternalMultiplexerEnable
				: 0U);
		const uint32_t enableCommand = commandHeader
			| (static_cast<uint32_t>(control) << 8U);
		ExecuteCommand(enableCommand, false, unusedResult);
	}

	bool MPC5xxxAnalogService::ExecuteCommand(uint32_t command,
		bool expectsResult, uint16_t& result)
	{
		// A previous failed conversion may have left a stale result behind.
		while (_module.FISR[Fifo].B.RFCTR != 0U)
			(void)_module.RFPR[Fifo].R;

		_module.FISR[Fifo].R = ClearableFifoFlags;
		_module.CFPR[Fifo].R = command;
		_module.CFCR[Fifo].R = SingleScanSoftwareTrigger | StartSingleScan;

		const uint32_t completionFlag = expectsResult
			? ResultFifoNotEmpty
			: EndOfQueueFlag;
		uint32_t remaining = WaitLimit;
		while ((_module.FISR[Fifo].R & completionFlag) == 0U && remaining != 0U)
			--remaining;

		if (remaining == 0U)
		{
			PrepareFifo();
			return false;
		}

		if (expectsResult)
			result = _module.RFPR[Fifo].B.RFPOP;

		_module.FISR[Fifo].R = ClearableFifoFlags;
		return true;
	}

	void MPC5xxxAnalogService::PrepareFifo()
	{
		_module.IDCR[Fifo].R = 0U;
		_module.CFCR[Fifo].R = 0U;
		_module.CFCR[Fifo].R = InvalidateFifo;

		while (_module.FISR[Fifo].B.RFCTR != 0U)
			(void)_module.RFPR[Fifo].R;

		_module.FISR[Fifo].R = ClearableFifoFlags;
	}

	MPC5xxxAnalogService::ADCConverter MPC5xxxAnalogService::ConverterForPin(
		EmbeddedIOServices::analogpin_t pin) const
	{
		return IsExternalMuxChannel(pin)
			? ADCConverter::ADC1
			: ADCConverter::ADC0;
	}

	bool MPC5xxxAnalogService::IsValidPin(
		EmbeddedIOServices::analogpin_t pin) const
	{
		if (pin == NullPin)
			return false;

		return pin < AnalogChannelCount || IsExternalMuxChannel(pin);
	}

	bool MPC5xxxAnalogService::IsExternalMuxChannel(
		EmbeddedIOServices::analogpin_t pin) const
	{
		return (pin >= 64U && pin <= 95U)
			|| (pin >= 224U && pin <= 255U);
	}
}
