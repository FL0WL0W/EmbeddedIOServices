#include "HardwareAbstraction/HardwareAbstractionCollection.h"
#include "IFloatInputService.h"
#include "math.h"
#include "Packed.h"
#include "stdint.h"

using namespace HardwareAbstraction;

#if !defined(FLOATINPUTSERVICE_FREQUENCYPOLYNOMIAL_H) && defined(IFLOATINPUTSERVICE_H) && defined(HARDWAREABSTRACTIONCOLLECTION_H) 
#define FLOATINPUTSERVICE_FREQUENCYPOLYNOMIAL_H
namespace IOServices
{
	PACK(
	template<uint8_t Degree>
	struct FloatInputService_FrequencyPolynomialConfig
	{
	public:
		constexpr const unsigned int Size() const
		{
			return sizeof(FloatInputService_FrequencyPolynomialConfig<Degree>);
		}
		
		uint16_t PwmPin;
		uint16_t MinFrequency;
		uint16_t DotSampleRate;
		float A[Degree + 1];
		float MinValue;
		float MaxValue;
	});

	template<uint8_t Degree>
	class FloatInputService_FrequencyPolynomial : public IFloatInputService
	{
	protected:
		const HardwareAbstractionCollection *_hardwareAbstractionCollection;
		const FloatInputService_FrequencyPolynomialConfig<Degree> *_config;

		uint32_t _lastReadTick = 0;
		float _lastValue = 0;
		
	public:
		FloatInputService_FrequencyPolynomial(const HardwareAbstractionCollection *hardwareAbstractionCollection, const FloatInputService_FrequencyPolynomialConfig<Degree> *config)
		{
			_hardwareAbstractionCollection = hardwareAbstractionCollection;
			_config = config;

			_hardwareAbstractionCollection->PwmService->InitPin(_config->PwmPin, HardwareAbstraction::In, _config->MinFrequency);
		}

		void ReadValue() override
		{
			HardwareAbstraction::PwmValue pwmValue = _hardwareAbstractionCollection->PwmService->ReadPin(_config->PwmPin);

			if (pwmValue.Period <= 0)
				return;

			float frequency = 1 / pwmValue.Period;

			Value = _config->A[0];
			for (uint8_t i = 1; i <= Degree; i++)
				Value += _config->A[i] * powf(frequency, i);
			if (Value < _config->MinValue)
				Value = _config->MinValue;
			else if (Value > _config->MaxValue)
				Value = _config->MaxValue;

			float elapsedTime = _hardwareAbstractionCollection->TimerService->GetElapsedTime(_lastReadTick);
			if (elapsedTime * _config->DotSampleRate < 1.0)
				return;

			_lastReadTick = _hardwareAbstractionCollection->TimerService->GetTick();
			ValueDot = (Value - _lastValue) / elapsedTime;
			_lastValue = Value;
		}
	};
}
#endif
