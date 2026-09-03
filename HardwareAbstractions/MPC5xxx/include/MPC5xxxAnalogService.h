#include "IAnalogService.h"
#include "MPC5xxx.h"

#ifndef MPC5XXXANALOGSERVICE_H
#define MPC5XXXANALOGSERVICE_H

namespace MPC5xxx
{
	/**
	 * @brief Blocking, single-conversion analog input service for the eQADC.
	 *
	 * Pins are eQADC channel numbers. Channels 0 through 39 are direct analog
	 * inputs. Channels 64 through 95 are external-multiplexer inputs on ANW,
	 * ANX, ANY, and ANZ. Devices with the additional ANR, ANS, ANT, and ANU
	 * inputs also provide external-multiplexer channels 224 through 255.
	 * Direct channels are converted by ADC0 and external-mux channels by ADC1.
	 * All commands and results use FIFO 5; do not share it with another eQADC
	 * user. Both converters are initialized by the constructor. Pad multiplexing
	 * is intentionally left to the application, so InitPin() is a no-op.
	 */
	class MPC5xxxAnalogService : public EmbeddedIOServices::IAnalogService
	{
	public:
		MPC5xxxAnalogService(volatile EQADC_tag& module,
			float referenceVoltage = 5.0F);

		void InitPin(EmbeddedIOServices::analogpin_t pin) override;
		float ReadPin(EmbeddedIOServices::analogpin_t pin) override;

	private:
		enum class ADCConverter : uint8_t
		{
			ADC0 = 0,
			ADC1 = 1
		};

		static constexpr EmbeddedIOServices::analogpin_t NullPin = 0xFFFFU;
		static constexpr EmbeddedIOServices::analogpin_t AnalogChannelCount = 40U;
		static constexpr uint8_t Fifo = 5U;
		static constexpr uint32_t WaitLimit = 100000U;

		void Initialize(ADCConverter converter);
		bool ExecuteCommand(uint32_t command, bool expectsResult, uint16_t& result);
		void PrepareFifo();
		ADCConverter ConverterForPin(EmbeddedIOServices::analogpin_t pin) const;
		bool IsValidPin(EmbeddedIOServices::analogpin_t pin) const;
		bool IsExternalMuxChannel(EmbeddedIOServices::analogpin_t pin) const;

		volatile EQADC_tag& _module;
		float _referenceVoltage;
	};
}

#endif
