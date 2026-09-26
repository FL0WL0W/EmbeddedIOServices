#ifndef MPC5XXXSPI_SERVICE_H
#define MPC5XXXSPI_SERVICE_H

#include "ISPIService.h"
#include "MPC5xxx.h"

#include <cstddef>
#include <cstdint>

namespace MPC5xxx
{
	struct SPIBusState;

	enum class SPIClockPolarity : std::uint8_t
	{
		IdleLow,
		IdleHigh,
	};

	enum class SPIClockPhase : std::uint8_t
	{
		CaptureOnLeadingEdge,
		CaptureOnTrailingEdge,
	};

	/**
	 * @brief Readable configuration for one device attached to a DSPI module.
	 */
	struct MPC5xxxSPIServiceConfiguration
	{
		std::uint8_t chipSelect;
		std::uint32_t clockSpeedHz;
		std::uint8_t bitsPerWord;
		SPIClockPolarity clockPolarity;
		SPIClockPhase clockPhase;
		std::uint32_t chipSelectSetupNanoseconds;
		std::uint32_t chipSelectHoldNanoseconds;
		std::uint32_t interWordDelayNanoseconds;
		bool leastSignificantBitFirst;
		bool chipSelectActiveHigh;
	};

	/**
	 * @brief Advanced per-frame timing used only by unusual derived services.
	 * Ordinary users do not need to construct this type.
	 */
	struct SPIFrameTiming
	{
		std::uint32_t chipSelectToClockNanoseconds;
		std::uint32_t afterClockNanoseconds;
		std::uint32_t afterTransferNanoseconds;
	};

	/**
	 * @brief One SPI device endpoint on an MPC5xxx DSPI module.
	 *
	 * Services that use the same DSPI address automatically share a single
	 * internal transaction queue. Pin muxing is deliberately outside this
	 * class; applications may configure PCR registers themselves.
	 */
	class MPC5xxxSPIService : public EmbeddedIOServices::ISPIService
	{
	private:
		volatile DSPI_tag* const _dspi;
		const MPC5xxxSPIServiceConfiguration _configuration;
		SPIBusState* _bus = nullptr;
		mutable SPIFrameTiming _cachedTiming = {};
		mutable std::uint32_t _cachedClockTransferAttributes = 0U;
		mutable bool _clockTransferAttributesCached = false;

		void StartNextQueuedTransfer();
		static void CompleteTransfer(SPIBusState& bus);
		std::uint32_t BuildClockTransferAttributes(
			const SPIFrameTiming& timing) const;

	protected:
		/**
		 * @brief Optional processor/board hook. The base service never calls it.
		 * A processor-specific derived constructor may call its own override after
		 * the base constructor has completed. The default implementation is empty.
		 */
		virtual void ConfigurePins() {}

		/**
		 * @brief Return timing for a frame. Ordinary devices use the configured
		 * timing for every frame; unusual devices may override this one method.
		 */
		virtual SPIFrameTiming TimingForFrame(std::size_t frameIndex) const;

	public:
		/**
		 * @param interruptPriority Priority for the receive eDMA completion
		 * interrupt. All endpoints sharing a DSPI module must use the same priority.
		 */
		MPC5xxxSPIService(
			volatile DSPI_tag* dspi,
			const MPC5xxxSPIServiceConfiguration& configuration,
			std::uint8_t interruptPriority);

		bool Ready() override;

		bool Transfer(
			std::uint8_t* data,
			std::size_t length,
			EmbeddedIOServices::spi_transfer_callback_t completionCallback) override;

		/** @brief Internal eDMA vector dispatch entry. */
		static void HandleDMAInterrupt(std::uint8_t channel);
		/** @brief Internal DSPI end-of-queue vector dispatch entry. */
		static void HandleEndOfQueueInterrupt(volatile DSPI_tag* dspi);
	};
}

#endif
