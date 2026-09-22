#include "MPC5xxxSPIService.h"
#include "MPC5xxxSystemClockService.h"

#include <new>
#include <utility>

namespace
{
	constexpr std::size_t kMaximumBusCount = 4U;
	constexpr std::size_t kQueueCapacity = 8U;
	constexpr std::uint32_t kHalt = 0x00000001U;
	constexpr std::uint32_t kMaster = 0x80000000U;
	constexpr std::uint32_t kOverwriteOnReceiveOverflow = 0x01000000U;
	constexpr std::uint32_t kAllChipSelectsInactiveHigh = 0x003F0000U;
	constexpr std::uint32_t kClearTransmitFifo = 0x00000800U;
	constexpr std::uint32_t kClearReceiveFifo = 0x00000400U;
	constexpr std::uint32_t kContinuousChipSelect = 0x80000000U;
	constexpr std::uint32_t kClockTransferAttributeShift = 28U;
	constexpr std::uint32_t kChipSelectShift = 16U;
	constexpr std::uint32_t kReceiveFifoDrainFlag = 0x00020000U;
	constexpr std::uint32_t kReceiveFifoDrainInterruptEnable = 0x00020000U;
	constexpr std::uint32_t kStatusFlagsToClear = 0x9A0A0000U;
	constexpr std::size_t kHardwareFifoDepth = 4U;
	constexpr std::size_t kClockTransferAttributeCount = 8U;
	constexpr std::uint16_t kBaudPrescalers[] = {2U, 3U, 5U, 7U};
	constexpr std::uint16_t kBaudScalers[] = {
		2U, 4U, 6U, 8U, 16U, 32U, 64U, 128U,
		256U, 512U, 1024U, 2048U, 4096U, 8192U, 16384U, 32768U,
	};
	constexpr std::uint16_t kDelayPrescalers[] = {1U, 3U, 5U, 7U};
	constexpr std::uint32_t kDelayScalers[] = {
		2U, 4U, 8U, 16U, 32U, 64U, 128U, 256U,
		512U, 1024U, 2048U, 4096U, 8192U, 16384U, 32768U, 65536U,
	};

	struct EncodedDelay
	{
		std::uint32_t Prescaler;
		std::uint32_t Scaler;
	};

	bool ExternalInterruptsEnabled()
	{
		std::uint32_t machineState;
		asm volatile("mfmsr %0" : "=r"(machineState));
		return (machineState & 0x00008000U) != 0U;
	}

	std::uint16_t ReceiveDrainVector(volatile DSPI_tag* dspi)
	{
		if (dspi == &DSPI_A) return 279U;
		if (dspi == &DSPI_B) return 135U;
		if (dspi == &DSPI_C) return 140U;
		if (dspi == &DSPI_D) return 145U;
		return 0xFFFFU;
	}

	EncodedDelay EncodeDelay(std::uint32_t nanoseconds, std::uint32_t clockHz)
	{
		// Compare candidate delays as fractions instead of dividing each one
		// into nanoseconds.  The common clockHz denominator does not affect
		// their ordering, and avoiding 64-bit division keeps __udivdi3 out of
		// small bare-metal builds.
		const std::uint64_t requestedScaled =
			static_cast<std::uint64_t>(nanoseconds) * clockHz;
		std::uint64_t bestError = ~static_cast<std::uint64_t>(0U);
		EncodedDelay best = {0U, 0U};
		for (std::uint32_t prescaler = 0U; prescaler < 4U; ++prescaler)
		{
			for (std::uint32_t scaler = 0U; scaler < 16U; ++scaler)
			{
				const std::uint64_t actualScaled =
					(static_cast<std::uint64_t>(kDelayPrescalers[prescaler]) *
					 kDelayScalers[scaler] * 1000000000ULL);
				const std::uint64_t error = actualScaled > requestedScaled
					? actualScaled - requestedScaled
					: requestedScaled - actualScaled;
				if (error >= bestError)
					continue;
				bestError = error;
				best.Prescaler = prescaler;
				best.Scaler = scaler;
			}
		}
		return best;
	}
}

namespace MPC5xxx
{
	struct SPIQueuedTransfer
	{
		MPC5xxxSPIService* Endpoint = nullptr;
		std::uint8_t* Data = nullptr;
		std::size_t Length = 0U;
		EmbeddedIOServices::spi_transfer_callback_t CompletionCallback;
		volatile bool Completed = false;
	};

	struct SPIBusState
	{
		volatile DSPI_tag* DSPI = nullptr;
		std::uint8_t InterruptPriority = 0U;
		SPIQueuedTransfer Queue[kQueueCapacity];
		std::size_t QueueHead = 0U;
		std::size_t QueueTail = 0U;
		std::size_t QueueCount = 0U;
		volatile std::size_t TransmitFrameIndex = 0U;
		volatile std::size_t ReceiveFrameIndex = 0U;
		volatile bool Active = false;
	};

	static SPIBusState buses[kMaximumBusCount];

	static SPIBusState* FindBus(volatile DSPI_tag* dspi)
	{
		for (std::size_t i = 0U; i < kMaximumBusCount; ++i)
			if (buses[i].DSPI == dspi)
				return &buses[i];
		return nullptr;
	}

	static SPIBusState* AcquireBus(
		volatile DSPI_tag* dspi,
		const MPC5xxxSPIServiceConfiguration& configuration,
		const std::uint8_t interruptPriority)
	{
		for (std::size_t i = 0U; i < kMaximumBusCount; ++i)
		{
			SPIBusState& bus = buses[i];
			if (bus.DSPI == dspi)
				return bus.InterruptPriority == interruptPriority ? &bus : nullptr;
			if (bus.DSPI != nullptr)
				continue;

			bus.DSPI = dspi;
			bus.InterruptPriority = interruptPriority;
			std::uint32_t moduleConfiguration =
				kMaster |
				kOverwriteOnReceiveOverflow |
				kAllChipSelectsInactiveHigh;
			if (configuration.chipSelectActiveHigh)
				moduleConfiguration &=
					~(1U << (16U + configuration.chipSelect));

			dspi->MCR.R = moduleConfiguration |
				kClearTransmitFifo | kClearReceiveFifo | kHalt;
			dspi->TCR.R = 0U;
			dspi->RSER.R = 0U;
			dspi->SR.R = kStatusFlagsToClear;
			const std::uint16_t receiveDrainVector = ReceiveDrainVector(dspi);
			if (receiveDrainVector == 0xFFFFU)
			{
				bus.DSPI = nullptr;
				return nullptr;
			}
			INTC.PSR[receiveDrainVector].R = interruptPriority;
			if (interruptPriority != 0U)
				dspi->RSER.R = kReceiveFifoDrainInterruptEnable;
			dspi->MCR.R = moduleConfiguration;
			return &bus;
		}
		return nullptr;
	}

	static void ConfigureChipSelectPolarity(
		volatile DSPI_tag* dspi,
		const MPC5xxxSPIServiceConfiguration& configuration)
	{
		const std::uint32_t previous = dspi->MCR.R;
		std::uint32_t updated = previous | kHalt;
		const std::uint32_t polarityBit =
			1U << (16U + configuration.chipSelect);
		if (configuration.chipSelectActiveHigh)
			updated &= ~polarityBit;
		else
			updated |= polarityBit;
		dspi->MCR.R = updated;
		dspi->MCR.R = (updated & ~kHalt) | (previous & kHalt);
	}

	MPC5xxxSPIService::MPC5xxxSPIService(
		volatile DSPI_tag* dspi,
		const MPC5xxxSPIServiceConfiguration& configuration,
		const std::uint8_t interruptPriority)
		: _dspi(dspi),
		  _configuration(configuration)
	{
		if (dspi == nullptr || configuration.clockSpeedHz == 0U ||
			configuration.bitsPerWord < 4U ||
			configuration.bitsPerWord > 16U)
			return;
		_bus = AcquireBus(dspi, configuration, interruptPriority);
		if (_bus != nullptr)
			ConfigureChipSelectPolarity(dspi, configuration);
	}

	SPIFrameTiming MPC5xxxSPIService::TimingForFrame(std::size_t) const
	{
		return {
			_configuration.chipSelectSetupNanoseconds,
			_configuration.chipSelectHoldNanoseconds,
			_configuration.interWordDelayNanoseconds,
		};
	}

	std::uint32_t MPC5xxxSPIService::BuildClockTransferAttributes(
		const SPIFrameTiming& timing) const
	{
		if (_clockTransferAttributesCached &&
			_cachedTiming.chipSelectToClockNanoseconds ==
				timing.chipSelectToClockNanoseconds &&
			_cachedTiming.afterClockNanoseconds == timing.afterClockNanoseconds &&
			_cachedTiming.afterTransferNanoseconds ==
				timing.afterTransferNanoseconds)
			return _cachedClockTransferAttributes;

		std::uint32_t bestBaudError = 0xFFFFFFFFU;
		std::uint32_t baudFields = 0U;
		const std::uint32_t peripheralClockHz = MPC5xxx::MPC5xxxSystemClockService::PeripheralClockHz();
		for (std::uint32_t doubleBaud = 0U; doubleBaud <= 1U; ++doubleBaud)
		{
			for (std::uint32_t prescaler = 0U; prescaler < 4U; ++prescaler)
			{
				for (std::uint32_t scaler = 0U; scaler < 16U; ++scaler)
				{
					const std::uint32_t actual =
						(peripheralClockHz * (doubleBaud + 1U)) /
						(kBaudPrescalers[prescaler] * kBaudScalers[scaler]);
					const std::uint32_t error =
						actual > _configuration.clockSpeedHz
							? actual - _configuration.clockSpeedHz
							: _configuration.clockSpeedHz - actual;
					if (error >= bestBaudError)
						continue;
					bestBaudError = error;
					baudFields =
						(doubleBaud << 31U) |
						(prescaler << 16U) |
						scaler;
				}
			}
		}

		const EncodedDelay chipSelectToClock = EncodeDelay(
			timing.chipSelectToClockNanoseconds,
			peripheralClockHz);
		const EncodedDelay afterClock = EncodeDelay(
			timing.afterClockNanoseconds,
			peripheralClockHz);
		const EncodedDelay afterTransfer = EncodeDelay(
			timing.afterTransferNanoseconds,
			peripheralClockHz);

		std::uint32_t attributes =
			baudFields |
			((static_cast<std::uint32_t>(_configuration.bitsPerWord) - 1U) << 27U) |
			(chipSelectToClock.Prescaler << 22U) |
			(afterClock.Prescaler << 20U) |
			(afterTransfer.Prescaler << 18U) |
			(chipSelectToClock.Scaler << 12U) |
			(afterClock.Scaler << 8U) |
			(afterTransfer.Scaler << 4U);
		if (_configuration.leastSignificantBitFirst)
			attributes |= 0x01000000U;
		if (_configuration.clockPhase ==
			SPIClockPhase::CaptureOnTrailingEdge)
			attributes |= 0x02000000U;
		if (_configuration.clockPolarity == SPIClockPolarity::IdleHigh)
			attributes |= 0x04000000U;
		_cachedTiming = timing;
		_cachedClockTransferAttributes = attributes;
		_clockTransferAttributesCached = true;
		return attributes;
	}

	bool MPC5xxxSPIService::Ready()
	{
		return _bus != nullptr && _bus->QueueCount < kQueueCapacity;
	}

	bool MPC5xxxSPIService::Transfer(
		std::uint8_t* data,
		std::size_t length,
		EmbeddedIOServices::spi_transfer_callback_t completionCallback)
	{
		const std::size_t bytesPerFrame =
			(_configuration.bitsPerWord + 7U) / 8U;
		if (_bus == nullptr || data == nullptr || length == 0U ||
			(length % bytesPerFrame) != 0U)
			return false;

		std::uint8_t* const ownedData = new (std::nothrow) std::uint8_t[length];
		if (ownedData == nullptr)
			return false;
		for (std::size_t i = 0U; i < length; ++i)
			ownedData[i] = data[i];

		if (_bus->QueueCount >= kQueueCapacity)
		{
			delete[] ownedData;
			return false;
		}

		SPIQueuedTransfer& queued = _bus->Queue[_bus->QueueTail];
		queued.Endpoint = this;
		queued.Data = ownedData;
		queued.Length = length;
		queued.CompletionCallback = std::move(completionCallback);
		queued.Completed = false;
		_bus->QueueTail = (_bus->QueueTail + 1U) % kQueueCapacity;
		++_bus->QueueCount;

		StartNextQueuedTransfer();
		return true;
	}

	void MPC5xxxSPIService::StartNextQueuedTransfer()
	{
		if (_bus == nullptr)
			return;

		if (_bus->Active || _bus->QueueCount == 0U)
			return;
		SPIQueuedTransfer& transfer = _bus->Queue[_bus->QueueHead];
		if (transfer.Completed)
			return;

		_bus->Active = true;
		_bus->TransmitFrameIndex = 0U;
		_bus->ReceiveFrameIndex = 0U;
		// The calling context launches one frame and returns. From its first
		// receive onward, the ISR exclusively owns FIFO refill, so FIFO state is
		// never manipulated by two nested FillTransmitFifo() invocations.
		transfer.Endpoint->FillTransmitFifo(1U);
	}

	void MPC5xxxSPIService::FillTransmitFifo(const std::size_t maximumFrames)
	{
		SPIQueuedTransfer& transfer = _bus->Queue[_bus->QueueHead];
		MPC5xxxSPIService& endpoint = *transfer.Endpoint;
		const std::size_t bytesPerFrame =
			(endpoint._configuration.bitsPerWord + 7U) / 8U;
		const std::size_t frameCount = transfer.Length / bytesPerFrame;

		// TXCTR can drop as soon as a frame moves into the shifter. Bound the
		// number of in-flight frames instead, so the four-entry RX FIFO cannot
		// overflow if Service() is not called again before all four complete.
		std::size_t framesFilled = 0U;
		while (framesFilled < maximumFrames &&
			_bus->TransmitFrameIndex < frameCount &&
			_bus->TransmitFrameIndex - _bus->ReceiveFrameIndex <
				kHardwareFifoDepth)
		{
			const std::size_t frameIndex = _bus->TransmitFrameIndex;
			const std::size_t offset = frameIndex * bytesPerFrame;
			std::uint16_t transmitted = transfer.Data[offset];
			if (bytesPerFrame == 2U)
				transmitted = static_cast<std::uint16_t>(
					(static_cast<std::uint16_t>(transfer.Data[offset]) << 8U) |
					transfer.Data[offset + 1U]);

			// Each queued frame retains its own timing selection. This matters for
			// the MPM transfer, whose first two frames use different delays.
			const std::size_t ctarIndex =
				frameIndex % kClockTransferAttributeCount;
			_dspi->CTAR[ctarIndex].R = endpoint.BuildClockTransferAttributes(
				endpoint.TimingForFrame(frameIndex));
			const std::uint32_t push =
				(frameIndex + 1U != frameCount ? kContinuousChipSelect : 0U) |
				(static_cast<std::uint32_t>(ctarIndex) <<
					kClockTransferAttributeShift) |
				((1U << endpoint._configuration.chipSelect) <<
					kChipSelectShift) |
				transmitted;
			// Publish software progress before starting the peripheral. Even the
			// fastest supported SPI clock cannot complete before the following
			// store, but this ordering also makes that invariant explicit.
			++_bus->TransmitFrameIndex;
			++framesFilled;
			_dspi->PUSHR.R = push;
		}
	}

	void MPC5xxxSPIService::ProcessHardware(volatile DSPI_tag& dspi)
	{
		SPIBusState* const bus = FindBus(&dspi);
		if (bus == nullptr)
		{
			dspi.SR.R = kReceiveFifoDrainFlag;
			return;
		}
		if (!bus->Active)
		{
			dspi.SR.R = kReceiveFifoDrainFlag;
			return;
		}
		if (dspi.SR.B.RXCTR == 0U)
		{
			dspi.SR.R = kReceiveFifoDrainFlag;
			return;
		}

		SPIQueuedTransfer& transfer = bus->Queue[bus->QueueHead];
		MPC5xxxSPIService& endpoint = *transfer.Endpoint;
		const std::size_t bytesPerFrame =
			(endpoint._configuration.bitsPerWord + 7U) / 8U;
		const std::size_t frameCount = transfer.Length / bytesPerFrame;
		while (dspi.SR.B.RXCTR != 0U &&
			bus->ReceiveFrameIndex < frameCount)
		{
			const std::uint16_t received =
				static_cast<std::uint16_t>(dspi.POPR.R);
			const std::size_t offset =
				bus->ReceiveFrameIndex * bytesPerFrame;
			if (bytesPerFrame == 2U)
			{
				transfer.Data[offset] =
					static_cast<std::uint8_t>(received >> 8U);
				transfer.Data[offset + 1U] =
					static_cast<std::uint8_t>(received);
			}
			else
			{
				transfer.Data[offset] = static_cast<std::uint8_t>(received);
			}
			++bus->ReceiveFrameIndex;
		}
		// RFDF remains asserted after the last POPR read in software mode.
		dspi.SR.R = kReceiveFifoDrainFlag;

		if (bus->ReceiveFrameIndex < frameCount)
		{
			endpoint.FillTransmitFifo(kHardwareFifoDepth);
			return;
		}

		// The ISR owns hardware progress only. Queue structure, callbacks, and
		// memory remain exclusively owned by the calling context in Service().
		bus->Active = false;
		transfer.Completed = true;
	}

	void MPC5xxxSPIService::ProcessCompletions(volatile DSPI_tag& dspi)
	{
		SPIBusState* const bus = FindBus(&dspi);
		if (bus == nullptr)
			return;

		while (bus->QueueCount != 0U)
		{
			SPIQueuedTransfer& transfer = bus->Queue[bus->QueueHead];
			if (!transfer.Completed)
				break;

			std::uint8_t* const completedData = transfer.Data;
			const std::size_t completedLength = transfer.Length;
			auto completionCallback = std::move(transfer.CompletionCallback);
			transfer.Endpoint = nullptr;
			transfer.Data = nullptr;
			transfer.Length = 0U;
			transfer.CompletionCallback = nullptr;
			transfer.Completed = false;
			bus->QueueHead = (bus->QueueHead + 1U) % kQueueCapacity;
			--bus->QueueCount;

			if (completionCallback)
				completionCallback(completedData, completedLength);
			delete[] completedData;
		}

		if (bus->QueueCount != 0U)
			bus->Queue[bus->QueueHead].Endpoint->StartNextQueuedTransfer();
	}

	void MPC5xxxSPIService::Service(volatile DSPI_tag& dspi)
	{
		ProcessHardware(dspi);
		ProcessCompletions(dspi);
	}
}

extern "C" void DSPI_A_ReceiveDrain_Handler()
{
	MPC5xxx::MPC5xxxSPIService::Service(DSPI_A);
}

extern "C" void DSPI_B_ReceiveDrain_Handler()
{
	MPC5xxx::MPC5xxxSPIService::Service(DSPI_B);
}

extern "C" void DSPI_C_ReceiveDrain_Handler()
{
	MPC5xxx::MPC5xxxSPIService::Service(DSPI_C);
}

extern "C" void DSPI_D_ReceiveDrain_Handler()
{
	MPC5xxx::MPC5xxxSPIService::Service(DSPI_D);
}
