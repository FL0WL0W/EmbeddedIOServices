#include "Service/HardwareAbstractionServiceBuilder.h"
#include "Service/VariableBusServiceBuilderRegister.h"
#include "Service/ServiceLocator.h"
#include "stdint.h"
#include "Packed.h"
#include "Interpolation.h"

#define FLOATVARIABLESERVICE_LOOKUPTABLETRANSLATION_H
namespace VariableBus
{
	PACK(
	template<uint8_t Degree>
	struct FloatVariableService_PolynomialTranslationConfig
	{
	private:
		FloatVariableService_PolynomialTranslationConfig()
		{
			
		}
		
	public:		
		constexpr const unsigned int Size() const
		{
			return sizeof(FloatVariableService_PolynomialTranslationConfig<Degree>);
		}

		float A[Degree+1];
		float MinValue;
		float MaxValue;
	});

	template<uint8_t Degree>
	class FloatVariableService_PolynomialTranslation
	{
	protected:
		float *_xVariable;
		const FloatVariableService_PolynomialTranslationConfig<Degree> *_config;
	public:		
		float Value = 0;
        FloatVariableService_PolynomialTranslation(float * const &xVariable, const FloatVariableService_PolynomialTranslationConfig<Degree> * const &config)
		{
			_xVariable = xVariable;
			_config = config;
		}

		virtual void TranslateValue()
		{
			float xVariable = *_xVariable;

			Value = _config->A[0];
			for (uint8_t i = 1; i <= Degree; i++)
				Value += _config->A[i] * powf(xVariable, i);
			if (Value < _config->MinValue)
				Value = _config->MinValue;
			else if (Value > _config->MaxValue)
				Value = _config->MaxValue;
		}

		static void BuildFloatVariableService_PolynomialTranslation(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
		{
			const uint8_t translations = Service::ServiceBuilder::CastAndOffset<uint8_t>(config, sizeOut);
			uint32_t *variableIds = reinterpret_cast<uint32_t *>(std::malloc(sizeof(uint32_t) * translations));
			float **xVariables = reinterpret_cast<float **>(std::malloc(sizeof(float *) * translations));
			for(int i = 0; i < translations; i++) 
			{
				variableIds[i] = Service::ServiceBuilder::CastAndOffset<uint16_t>(config, sizeOut);
				xVariables[i] = serviceLocator->LocateAndCast<float>(VARIABLE_BUS, Service::ServiceBuilder::CastAndOffset<uint16_t>(config, sizeOut));
			}
			const FloatVariableService_PolynomialTranslationConfig<Degree> *lookupTableConfig = reinterpret_cast<const FloatVariableService_PolynomialTranslationConfig<Degree> *>(config);
			sizeOut += lookupTableConfig->Size();

			for(int i = 0; i < translations; i++) 
			{
				FloatVariableService_PolynomialTranslation<Degree> *floatVariableService = new FloatVariableService_PolynomialTranslation<Degree>(xVariables[i], lookupTableConfig);
				serviceLocator->LocateAndCast<CallBackGroup>(TICK_CALL_BACK_GROUP)->Add(new CallBack<FloatVariableService_PolynomialTranslation<Degree>>(floatVariableService, &FloatVariableService_PolynomialTranslation<Degree>::TranslateValue));

				serviceLocator->Register(VARIABLE_BUS, variableIds[i], &floatVariableService->Value);
			}

			free(variableIds);
			free(xVariables);
		}
	};
}
