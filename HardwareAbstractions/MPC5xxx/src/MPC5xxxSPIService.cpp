#include "MPC5xxxSPIService.h"
#include "MPC5xxxSystemClockService.h"
#include "CircularBuffer.h"

#include <new>
#include <utility>

namespace
{
	constexpr std::size_t kMaximumBusCount = 4U;
	constexpr std::size_t kQueueCapacity = 8U;
	constexpr std::size_t kDSPIHardwareFifoDepth = 4U;
	constexpr std::size_t kClockTransferAttributeCount = 8U;
	constexpr std::size_t kMaximumMajorLoopCount = 0x7FFFU;

	constexpr std::uint32_t kHalt = 0x00000001U;
	constexpr std::uint32_t kMaster = 0x80000000U;
	constexpr std::uint32_t kOverwriteOnReceiveOverflow = 0x01000000U;
	constexpr std::uint32_t kAllChipSelectsInactiveHigh = 0x003F0000U;
	constexpr std::uint32_t kClearTransmitFifo = 0x00000800U;
	constexpr std::uint32_t kClearReceiveFifo = 0x00000400U;
	constexpr std::uint32_t kContinuousChipSelect = 0x80000000U;
	constexpr std::uint32_t kEndOfQueue = 0x08000000U;
	constexpr std::uint32_t kClockTransferAttributeShift = 28U;
	constexpr std::uint32_t kChipSelectShift = 16U;
	constexpr std::uint32_t kEndOfQueueFlag = 0x10000000U;
	constexpr std::uint32_t kEndOfQueueInterruptEnable = 0x10000000U;
	constexpr std::uint32_t kStatusFlagsToClear = 0x9A0A0000U;
	constexpr std::uint32_t kTransmitAndReceiveDMAEnable = 0x03030000U;

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

	struct DMAChannels
	{
		std::uint8_t Transmit;
		std::uint8_t Receive;
	};

	struct EncodedDelay
	{
		std::uint32_t Prescaler;
		std::uint32_t Scaler;
	};

	volatile EDMA_tag& DMAController()
	{
#if defined(MPC5566)
		return EDMA;
#elif defined(MPC5674F)
		return EDMA_A;
#endif
	}

	bool ChannelsForDSPI(volatile DSPI_tag* const dspi, DMAChannels& channels)
	{
		if (dspi == &DSPI_A) channels = {32U, 33U};
		else if (dspi == &DSPI_B) channels = {12U, 13U};
		else if (dspi == &DSPI_C) channels = {14U, 15U};
		else if (dspi == &DSPI_D) channels = {16U, 17U};
		else return false;
		return true;
	}

	std::uint16_t DMAInterruptVector(const std::uint8_t channel)
	{
		return channel < 32U
			? static_cast<std::uint16_t>(11U + channel)
			: static_cast<std::uint16_t>(179U + channel);
	}

	std::uint16_t EndOfQueueInterruptVector(volatile DSPI_tag* const dspi)
	{
		if (dspi == &DSPI_B) return 132U;
		if (dspi == &DSPI_C) return 137U;
		if (dspi == &DSPI_D) return 142U;
		if (dspi == &DSPI_A) return 276U;
		return 0U;
	}

	EncodedDelay EncodeDelay(
		const std::uint32_t nanoseconds,
		const std::uint32_t clockHz)
	{
		const std::uint64_t requestedScaled =
			static_cast<std::uint64_t>(nanoseconds) * clockHz;
		std::uint64_t bestError = ~static_cast<std::uint64_t>(0U);
		EncodedDelay best = {0U, 0U};
		for (std::uint32_t prescaler = 0U; prescaler < 4U; ++prescaler)
		{
			for (std::uint32_t scaler = 0U; scaler < 16U; ++scaler)
			{
				const std::uint64_t actualScaled =
					static_cast<std::uint64_t>(kDelayPrescalers[prescaler]) *
					kDelayScalers[scaler] * 1000000000ULL;
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
		const std::uint8_t* TransmitData = nullptr;
		std::uint8_t* ReceiveData = nullptr;
		std::size_t FrameCount = 0U;
		std::size_t CompletedFrameCount = 0U;
		std::size_t BatchFrameCount = 0U;
		std::uint32_t ClockTransferAttributes[kClockTransferAttributeCount] = {};
		std::size_t ClockTransferAttributeCount = 0U;
		EmbeddedIOServices::spi_transfer_callback_t CompletionCallback;
	};

	struct SPIBusState
	{
		volatile DSPI_tag* DSPI = nullptr;
		DMAChannels Channels = {};
		std::uint8_t InterruptPriority = 0U;
		EmbeddedIOServices::CircularBuffer<
			SPIQueuedTransfer,
			kQueueCapacity> Queue;
		SPIQueuedTransfer ActiveTransfer;
		volatile std::uint32_t Active = 0U;
		volatile bool UsingDMA = false;
	};

	static SPIBusState buses[kMaximumBusCount];

	static SPIBusState* FindBusByReceiveDMAChannel(const std::uint8_t channel)
	{
		for (std::size_t i = 0U; i < kMaximumBusCount; ++i)
			if (buses[i].DSPI != nullptr && buses[i].Channels.Receive == channel)
				return &buses[i];
		return nullptr;
	}

	static SPIBusState* FindBusByDSPI(volatile DSPI_tag* const dspi)
	{
		for (std::size_t i = 0U; i < kMaximumBusCount; ++i)
			if (buses[i].DSPI == dspi)
				return &buses[i];
		return nullptr;
	}

	static SPIBusState* AcquireBus(
		volatile DSPI_tag* const dspi,
		const MPC5xxxSPIServiceConfiguration& configuration,
		const std::uint8_t interruptPriority)
	{
		DMAChannels channels;
		if (!ChannelsForDSPI(dspi, channels) || interruptPriority == 0U)
			return nullptr;

		for (std::size_t i = 0U; i < kMaximumBusCount; ++i)
		{
			SPIBusState& bus = buses[i];
			if (bus.DSPI == dspi)
				return bus.InterruptPriority == interruptPriority ? &bus : nullptr;
			if (bus.DSPI != nullptr)
				continue;

			bus.DSPI = dspi;
			bus.Channels = channels;
			bus.InterruptPriority = interruptPriority;
			std::uint32_t moduleConfiguration =
				kMaster | kOverwriteOnReceiveOverflow |
				kAllChipSelectsInactiveHigh | kHalt;
			if (configuration.chipSelectActiveHigh)
				moduleConfiguration &=
					~(1U << (16U + configuration.chipSelect));
			dspi->MCR.R = moduleConfiguration |
				kClearTransmitFifo | kClearReceiveFifo;
			dspi->TCR.R = 0U;
			dspi->RSER.R = 0U;
			dspi->SR.R = kStatusFlagsToClear;

			volatile EDMA_tag& dma = DMAController();
			dma.CERQR.R = channels.Transmit;
			dma.CERQR.R = channels.Receive;
			dma.CIRQR.R = channels.Transmit;
			dma.CIRQR.R = channels.Receive;
			dma.CDSBR.R = channels.Transmit;
			dma.CDSBR.R = channels.Receive;
			dma.CPR[channels.Transmit].R = channels.Transmit;
			dma.CPR[channels.Receive].R = channels.Receive;
			INTC.PSR[DMAInterruptVector(channels.Receive)].R = interruptPriority;
			INTC.PSR[EndOfQueueInterruptVector(dspi)].R = interruptPriority;
			return &bus;
		}
		return nullptr;
	}

	static void ConfigureChipSelectPolarity(
		volatile DSPI_tag* const dspi,
		const MPC5xxxSPIServiceConfiguration& configuration)
	{
		std::uint32_t moduleConfiguration = dspi->MCR.R | kHalt;
		const std::uint32_t polarityBit =
			1U << (16U + configuration.chipSelect);
		if (configuration.chipSelectActiveHigh)
			moduleConfiguration &= ~polarityBit;
		else
			moduleConfiguration |= polarityBit;
		dspi->MCR.R = moduleConfiguration;
	}

	MPC5xxxSPIService::MPC5xxxSPIService(
		volatile DSPI_tag* const dspi,
		const MPC5xxxSPIServiceConfiguration& configuration,
		const std::uint8_t interruptPriority,
		const std::size_t stagingBufferSize)
		: _dspi(dspi), _configuration(configuration)
	{
		if (dspi == nullptr || configuration.clockSpeedHz == 0U ||
			configuration.bitsPerWord < 4U ||
			configuration.bitsPerWord > 16U)
			return;
		const std::size_t bytesPerFrame =
			(configuration.bitsPerWord + 7U) / 8U;
		if (stagingBufferSize != 0U)
		{
			_stagingFrameCapacity = stagingBufferSize / bytesPerFrame;
			if (_stagingFrameCapacity == 0U ||
				_stagingFrameCapacity > kMaximumMajorLoopCount)
				return;
			_transmitStagingBuffer =
				new (std::nothrow) std::uint32_t[_stagingFrameCapacity];
			_receiveStagingBuffer =
				new (std::nothrow) std::uint32_t[_stagingFrameCapacity];
			if (_transmitStagingBuffer == nullptr ||
				_receiveStagingBuffer == nullptr)
			{
				delete[] _transmitStagingBuffer;
				delete[] _receiveStagingBuffer;
				_transmitStagingBuffer = nullptr;
				_receiveStagingBuffer = nullptr;
				_stagingFrameCapacity = 0U;
				return;
			}
		}
		_bus = AcquireBus(dspi, configuration, interruptPriority);
		if (_bus != nullptr)
			ConfigureChipSelectPolarity(dspi, configuration);
	}

	MPC5xxxSPIService::~MPC5xxxSPIService()
	{
		delete[] _transmitStagingBuffer;
		delete[] _receiveStagingBuffer;
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
		const std::uint32_t peripheralClockHz =
			MPC5xxxSystemClockService::PeripheralClockHz();
		for (std::uint32_t doubleBaud = 0U; doubleBaud <= 1U; ++doubleBaud)
		{
			for (std::uint32_t prescaler = 0U; prescaler < 4U; ++prescaler)
			{
				for (std::uint32_t scaler = 0U; scaler < 16U; ++scaler)
				{
					const std::uint32_t actual =
						(peripheralClockHz * (doubleBaud + 1U)) /
						(kBaudPrescalers[prescaler] * kBaudScalers[scaler]);
					const std::uint32_t error = actual > _configuration.clockSpeedHz
						? actual - _configuration.clockSpeedHz
						: _configuration.clockSpeedHz - actual;
					if (error >= bestBaudError)
						continue;
					bestBaudError = error;
					baudFields =
						(doubleBaud << 31U) | (prescaler << 16U) | scaler;
				}
			}
		}

		const EncodedDelay chipSelectToClock = EncodeDelay(
			timing.chipSelectToClockNanoseconds, peripheralClockHz);
		const EncodedDelay afterClock = EncodeDelay(
			timing.afterClockNanoseconds, peripheralClockHz);
		const EncodedDelay afterTransfer = EncodeDelay(
			timing.afterTransferNanoseconds, peripheralClockHz);
		std::uint32_t attributes =
			baudFields |
			((static_cast<std::uint32_t>(_configuration.bitsPerWord) - 1U) << 27U) |
			(chipSelectToClock.Prescaler << 22U) |
			(afterClock.Prescaler << 20U) |
			(afterTransfer.Prescaler << 18U) |
			(chipSelectToClock.Scaler << 12U) |
			(afterClock.Scaler << 8U) |
			(afterTransfer.Scaler << 4U);
		if (_configuration.leastSignificantBitFirst) attributes |= 0x01000000U;
		if (_configuration.clockPhase == SPIClockPhase::CaptureOnTrailingEdge)
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
		return _bus != nullptr && !_bus->Queue.Full();
	}

	bool MPC5xxxSPIService::Transfer(
		const std::uint8_t* const txData,
		std::uint8_t* const rxData,
		const std::size_t length,
		EmbeddedIOServices::spi_transfer_callback_t completionCallback)
	{
		const std::size_t bytesPerFrame =
			(_configuration.bitsPerWord + 7U) / 8U;
		if (_bus == nullptr || txData == nullptr || length == 0U ||
			(length % bytesPerFrame) != 0U)
			return false;
		const std::size_t frameCount = length / bytesPerFrame;
		if (frameCount > kDSPIHardwareFifoDepth &&
			_stagingFrameCapacity == 0U)
			return false;

		SPIQueuedTransfer transfer;
		transfer.Endpoint = this;
		transfer.TransmitData = txData;
		transfer.ReceiveData = rxData;
		transfer.FrameCount = frameCount;
		transfer.CompletionCallback = std::move(completionCallback);

		// A transaction carries the complete CTAR set it needs. Publishing it to
		// the circular queue is atomic at the slot level, and the values are not
		// written to hardware until this transaction exclusively owns the bus.
		for (std::size_t frame = 0U; frame < frameCount; ++frame)
		{
			const std::uint32_t attributes =
				BuildClockTransferAttributes(TimingForFrame(frame));
			std::size_t index = 0U;
			while (index < transfer.ClockTransferAttributeCount &&
				transfer.ClockTransferAttributes[index] != attributes)
				++index;
			if (index != transfer.ClockTransferAttributeCount)
				continue;
			if (transfer.ClockTransferAttributeCount ==
				kClockTransferAttributeCount)
				return false;
			transfer.ClockTransferAttributes[
				transfer.ClockTransferAttributeCount++] = attributes;
		}
		if (!_bus->Queue.Push(transfer))
			return false;

		StartNextQueuedTransfer();
		return true;
	}

	void MPC5xxxSPIService::StartNextQueuedTransfer()
	{
		if (_bus == nullptr)
			return;
		std::uint32_t expected = 0U;
		if (!__atomic_compare_exchange_n(
				&_bus->Active,
				&expected,
				1U,
				false,
				__ATOMIC_RELAXED,
				__ATOMIC_RELAXED))
			return;
		asm volatile("" ::: "memory");
		if (_bus->Queue.Pop(_bus->ActiveTransfer) == 0U)
		{
			__atomic_store_n(&_bus->Active, 0U, __ATOMIC_RELAXED);
			return;
		}
		SPIQueuedTransfer& transfer = _bus->ActiveTransfer;
		MPC5xxxSPIService* const endpoint = transfer.Endpoint;
		SPIBusState& bus = *_bus;

		volatile DSPI_tag& dspi = *bus.DSPI;
		dspi.MCR.R |= kHalt;
		asm volatile("mbar" ::: "memory");
		while (dspi.SR.B.TXRXS != 0U) { }
		dspi.RSER.R = 0U;
		dspi.MCR.R |= kClearTransmitFifo | kClearReceiveFifo;
		for (std::size_t i = 0U;
			i < transfer.ClockTransferAttributeCount;
			++i)
			dspi.CTAR[i].R = transfer.ClockTransferAttributes[i];

		volatile EDMA_tag& dma = DMAController();
		const std::uint8_t tx = bus.Channels.Transmit;
		const std::uint8_t rx = bus.Channels.Receive;
		dma.CERQR.R = tx;
		dma.CERQR.R = rx;
		dma.CIRQR.R = rx;
		dma.CDSBR.R = tx;
		dma.CDSBR.R = rx;

		dspi.SR.R = kStatusFlagsToClear;
		if (transfer.FrameCount <= kDSPIHardwareFifoDepth)
		{
			// The complete transaction fits in the four-entry DSPI TX/RX FIFOs.
			// Avoid the eDMA setup cost and finish from the EOQ interrupt.
			bus.UsingDMA = false;
			transfer.BatchFrameCount = transfer.FrameCount;
			const std::size_t bytesPerFrame =
				(endpoint->_configuration.bitsPerWord + 7U) / 8U;
			for (std::size_t frame = 0U; frame < transfer.FrameCount; ++frame)
			{
				const std::uint32_t attributes =
					endpoint->BuildClockTransferAttributes(
						endpoint->TimingForFrame(frame));
				std::size_t ctarIndex = 0U;
				while (transfer.ClockTransferAttributes[ctarIndex] != attributes)
					++ctarIndex;
				std::uint16_t transmitted =
					transfer.TransmitData[frame * bytesPerFrame];
				if (bytesPerFrame == 2U)
					transmitted = static_cast<std::uint16_t>(
						(static_cast<std::uint16_t>(
							transfer.TransmitData[frame * 2U]) << 8U) |
						transfer.TransmitData[frame * 2U + 1U]);
				const std::uint32_t pushWord =
					(frame + 1U == transfer.FrameCount
						? kEndOfQueue : kContinuousChipSelect) |
					(static_cast<std::uint32_t>(ctarIndex) <<
						kClockTransferAttributeShift) |
					((1U << endpoint->_configuration.chipSelect) <<
						kChipSelectShift) |
					transmitted;
				dspi.PUSHR.R = pushWord;
			}
			dspi.RSER.R = kEndOfQueueInterruptEnable;
			asm volatile("mbar" ::: "memory");
			dspi.MCR.R &= ~kHalt;
			return;
		}
		bus.UsingDMA = true;
		endpoint->StartNextDMABatch();
	}

	void MPC5xxxSPIService::StartNextDMABatch()
	{
		SPIQueuedTransfer& transfer = _bus->ActiveTransfer;
		const std::size_t remainingFrames =
			transfer.FrameCount - transfer.CompletedFrameCount;
		transfer.BatchFrameCount = remainingFrames < _stagingFrameCapacity
			? remainingFrames : _stagingFrameCapacity;
		const std::size_t bytesPerFrame =
			(_configuration.bitsPerWord + 7U) / 8U;

		for (std::size_t batchFrame = 0U;
			batchFrame < transfer.BatchFrameCount;
			++batchFrame)
		{
			const std::size_t frame =
				transfer.CompletedFrameCount + batchFrame;
			const std::uint32_t attributes =
				BuildClockTransferAttributes(TimingForFrame(frame));
			std::size_t ctarIndex = 0U;
			while (transfer.ClockTransferAttributes[ctarIndex] != attributes)
				++ctarIndex;
			std::uint16_t transmitted =
				transfer.TransmitData[frame * bytesPerFrame];
			if (bytesPerFrame == 2U)
				transmitted = static_cast<std::uint16_t>(
					(static_cast<std::uint16_t>(
						transfer.TransmitData[frame * 2U]) << 8U) |
					transfer.TransmitData[frame * 2U + 1U]);
			_transmitStagingBuffer[batchFrame] =
				(frame + 1U == transfer.FrameCount
					? kEndOfQueue : kContinuousChipSelect) |
				(static_cast<std::uint32_t>(ctarIndex) <<
					kClockTransferAttributeShift) |
				((1U << _configuration.chipSelect) << kChipSelectShift) |
				transmitted;
		}

		volatile DSPI_tag& dspi = *_bus->DSPI;
		volatile EDMA_tag& dma = DMAController();
		const std::uint8_t tx = _bus->Channels.Transmit;
		const std::uint8_t rx = _bus->Channels.Receive;
		dma.CERQR.R = tx;
		dma.CERQR.R = rx;
		dma.CIRQR.R = rx;
		dma.CDSBR.R = tx;
		dma.CDSBR.R = rx;
		volatile EDMA_tag::tcd_t& txTCD = dma.TCD[tx];
		txTCD.SADDR = reinterpret_cast<std::uint32_t>(_transmitStagingBuffer);
		txTCD.SMOD = 0U; txTCD.SSIZE = 2U;
		txTCD.DMOD = 0U; txTCD.DSIZE = 2U;
		txTCD.SOFF = 4; txTCD.NBYTES = 4U;
		txTCD.SLAST = -static_cast<std::int32_t>(
			transfer.BatchFrameCount * 4U);
		txTCD.DADDR = reinterpret_cast<std::uint32_t>(&dspi.PUSHR.R);
		txTCD.CITERE_LINK = 0U;
		txTCD.CITER = static_cast<std::uint16_t>(transfer.BatchFrameCount);
		txTCD.DOFF = 0; txTCD.DLAST_SGA = 0;
		txTCD.BITERE_LINK = 0U;
		txTCD.BITER = static_cast<std::uint16_t>(transfer.BatchFrameCount);
		txTCD.BWC = 0U; txTCD.MAJORLINKCH = 0U;
		txTCD.DONE = 0U; txTCD.ACTIVE = 0U;
		txTCD.MAJORE_LINK = 0U; txTCD.E_SG = 0U;
		txTCD.D_REQ = 1U; txTCD.INT_HALF = 0U;
		txTCD.INT_MAJ = 0U; txTCD.START = 0U;

		volatile EDMA_tag::tcd_t& rxTCD = dma.TCD[rx];
		rxTCD.SADDR = reinterpret_cast<std::uint32_t>(&dspi.POPR.R);
		rxTCD.SMOD = 0U; rxTCD.SSIZE = 2U;
		rxTCD.DMOD = 0U; rxTCD.DSIZE = 2U;
		rxTCD.SOFF = 0; rxTCD.NBYTES = 4U; rxTCD.SLAST = 0;
		rxTCD.DADDR = reinterpret_cast<std::uint32_t>(_receiveStagingBuffer);
		rxTCD.CITERE_LINK = 0U;
		rxTCD.CITER = static_cast<std::uint16_t>(transfer.BatchFrameCount);
		rxTCD.DOFF = 4;
		rxTCD.DLAST_SGA = -static_cast<std::int32_t>(
			transfer.BatchFrameCount * 4U);
		rxTCD.BITERE_LINK = 0U;
		rxTCD.BITER = static_cast<std::uint16_t>(transfer.BatchFrameCount);
		rxTCD.BWC = 0U; rxTCD.MAJORLINKCH = 0U;
		rxTCD.DONE = 0U; rxTCD.ACTIVE = 0U;
		rxTCD.MAJORE_LINK = 0U; rxTCD.E_SG = 0U;
		rxTCD.D_REQ = 1U; rxTCD.INT_HALF = 0U;
		rxTCD.INT_MAJ = 1U; rxTCD.START = 0U;

		dspi.RSER.R = kTransmitAndReceiveDMAEnable;
		asm volatile("mbar" ::: "memory");
		dma.SERQR.R = rx;
		dma.SERQR.R = tx;
		asm volatile("mbar" ::: "memory");
		dspi.MCR.R &= ~kHalt;
	}

	void MPC5xxxSPIService::CopyReceivedDMABatch()
	{
		SPIQueuedTransfer& transfer = _bus->ActiveTransfer;
		const std::size_t bytesPerFrame =
			(_configuration.bitsPerWord + 7U) / 8U;
		if (transfer.ReceiveData != nullptr)
		{
			for (std::size_t batchFrame = 0U;
				batchFrame < transfer.BatchFrameCount;
				++batchFrame)
			{
				const std::size_t frame =
					transfer.CompletedFrameCount + batchFrame;
				const std::uint16_t received = static_cast<std::uint16_t>(
					_receiveStagingBuffer[batchFrame]);
				if (bytesPerFrame == 2U)
				{
					transfer.ReceiveData[frame * 2U] =
						static_cast<std::uint8_t>(received >> 8U);
					transfer.ReceiveData[frame * 2U + 1U] =
						static_cast<std::uint8_t>(received);
				}
				else
					transfer.ReceiveData[frame] =
						static_cast<std::uint8_t>(received);
			}
		}
		transfer.CompletedFrameCount += transfer.BatchFrameCount;
	}

	void MPC5xxxSPIService::HandleDMAInterrupt(const std::uint8_t channel)
	{
		volatile EDMA_tag& dma = DMAController();
		dma.CIRQR.R = channel;
		SPIBusState* const bus = FindBusByReceiveDMAChannel(channel);
		if (bus == nullptr || bus->Active == 0U || !bus->UsingDMA)
			return;

		volatile DSPI_tag& dspi = *bus->DSPI;
		dspi.RSER.R = 0U;
		SPIQueuedTransfer& transfer = bus->ActiveTransfer;
		const bool finalBatch =
			transfer.CompletedFrameCount + transfer.BatchFrameCount ==
			transfer.FrameCount;
		if (finalBatch)
		{
			while (dspi.SR.B.TXRXS != 0U) { }
			asm volatile("mbar" ::: "memory");
		}
		transfer.Endpoint->CopyReceivedDMABatch();
		if (transfer.CompletedFrameCount < transfer.FrameCount)
		{
			transfer.Endpoint->StartNextDMABatch();
			return;
		}
		dspi.SR.R = kEndOfQueueFlag;
		CompleteTransfer(*bus);
	}

	void MPC5xxxSPIService::HandleEndOfQueueInterrupt(
		volatile DSPI_tag* const dspiAddress)
	{
		SPIBusState* const bus = FindBusByDSPI(dspiAddress);
		if (bus == nullptr || bus->Active == 0U || bus->UsingDMA)
			return;

		volatile DSPI_tag& dspi = *dspiAddress;
		dspi.RSER.R = 0U;
		dspi.SR.R = kEndOfQueueFlag;
		CompleteTransfer(*bus);
	}

	void MPC5xxxSPIService::CompleteTransfer(SPIBusState& bus)
	{
		SPIQueuedTransfer& queued = bus.ActiveTransfer;
		MPC5xxxSPIService* const completedEndpoint = queued.Endpoint;

		if (!bus.UsingDMA)
		{
			MPC5xxxSPIService* const endpoint = queued.Endpoint;
			const std::size_t bytesPerFrame =
				(endpoint->_configuration.bitsPerWord + 7U) / 8U;
			for (std::size_t frame = 0U; frame < queued.FrameCount; ++frame)
			{
				const std::uint16_t received =
					static_cast<std::uint16_t>(bus.DSPI->POPR.R);
				if (queued.ReceiveData == nullptr)
					continue;
				if (bytesPerFrame == 2U)
				{
					queued.ReceiveData[frame * 2U] =
						static_cast<std::uint8_t>(received >> 8U);
					queued.ReceiveData[frame * 2U + 1U] =
						static_cast<std::uint8_t>(received);
				}
				else
					queued.ReceiveData[frame] =
						static_cast<std::uint8_t>(received);
			}
		}

		auto completionCallback = std::move(queued.CompletionCallback);
		queued = {};
		bus.UsingDMA = false;
		asm volatile("" ::: "memory");
		__atomic_store_n(&bus.Active, 0U, __ATOMIC_RELAXED);

		if (completionCallback)
			completionCallback();

		// A callback may enqueue and start another transfer itself. The ownership
		// claim makes this harmless: either that path wins, or this one drains the
		// next queued transaction.
		completedEndpoint->StartNextQueuedTransfer();
	}
}

extern "C" void DSPI_A_EndOfQueue_Handler()
{
	MPC5xxx::MPC5xxxSPIService::HandleEndOfQueueInterrupt(&DSPI_A);
}

extern "C" void DSPI_B_EndOfQueue_Handler()
{
	MPC5xxx::MPC5xxxSPIService::HandleEndOfQueueInterrupt(&DSPI_B);
}

extern "C" void DSPI_C_EndOfQueue_Handler()
{
	MPC5xxx::MPC5xxxSPIService::HandleEndOfQueueInterrupt(&DSPI_C);
}

extern "C" void DSPI_D_EndOfQueue_Handler()
{
	MPC5xxx::MPC5xxxSPIService::HandleEndOfQueueInterrupt(&DSPI_D);
}

extern "C" void EDMA_Channel13_Handler()
{
	MPC5xxx::MPC5xxxSPIService::HandleDMAInterrupt(13U);
}

extern "C" void EDMA_Channel15_Handler()
{
	MPC5xxx::MPC5xxxSPIService::HandleDMAInterrupt(15U);
}

extern "C" void EDMA_Channel17_Handler()
{
	MPC5xxx::MPC5xxxSPIService::HandleDMAInterrupt(17U);
}

extern "C" void EDMA_Channel33_Handler()
{
	MPC5xxx::MPC5xxxSPIService::HandleDMAInterrupt(33U);
}
