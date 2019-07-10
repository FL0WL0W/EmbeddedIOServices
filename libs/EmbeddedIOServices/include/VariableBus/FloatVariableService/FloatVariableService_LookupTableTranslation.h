#include "Service/HardwareAbstractionServiceBuilder.h"
#include "Service/VariableBusServiceBuilderRegister.h"
#include "Service/ServiceLocator.h"
#include "stdint.h"
#include "Packed.h"
#include "Interpolation.h"

#if !defined(FLOATVARIABLESERVICE_LOOKUPTABLETRANSLATION_H)
#define FLOATVARIABLESERVICE_LOOKUPTABLETRANSLATION_H
namespace VariableBus
{
	PACK(
	template<typename K>
	struct FloatVariableService_LookupTableTranslationConfig
	{
	private:
		FloatVariableService_LookupTableTranslationConfig()
		{
			
		}
		
	public:		
		constexpr const unsigned int Size() const
		{
			return sizeof(FloatVariableService_LookupTableTranslationConfig) +
				(sizeof(K) * XResolution);
		}

		constexpr const K *Table() const { return reinterpret_cast<const K *>(this + 1); }
		
		float MinXValue;
		float MaxXValue;
		uint8_t XResolution;
		float PostMultiplier;
	});

	template<typename K>
	class FloatVariableService_LookupTableTranslation
	{
	protected:
		float *_xVariable;
		const FloatVariableService_LookupTableTranslationConfig<K> *_config;
	public:		
		float Value = 0;
        FloatVariableService_LookupTableTranslation(float * const &xVariable, const FloatVariableService_LookupTableTranslationConfig<K> * const &config)
		{
			_xVariable = xVariable;
			_config = config;
		}

		virtual void TranslateValue()
		{
			float xVariable = *_xVariable;

			Value = Interpolation::InterpolateTable1<K>(xVariable, _config->MaxXValue, _config->MinXValue, _config->XResolution, _config->Table()) * _config->PostMultiplier;
		}

		static void BuildFloatVariableService_LookupTableTranslation(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
		{
			const uint8_t translations = ServiceBuilder::CastAndOffset<uint8_t>(config, sizeOut);
			uint32_t *variableIds = reinterpret_cast<uint32_t *>(std::malloc(sizeof(uint32_t) * translations));
			float **xVariables = reinterpret_cast<float **>(std::malloc(sizeof(float *) * translations));
			for(int i = 0; i < translations; i++) 
			{
				variableIds[i] = ServiceBuilder::CastAndOffset<uint16_t>(config, sizeOut);
				xVariables[i] = serviceLocator->LocateAndCast<float>(VARIABLE_BUS, ServiceBuilder::CastAndOffset<uint16_t>(config, sizeOut));
			}
			const FloatVariableService_LookupTableTranslationConfig<K> *lookupTableConfig = reinterpret_cast<const FloatVariableService_LookupTableTranslationConfig<K> *>(config);
			sizeOut += lookupTableConfig->Size();

			for(int i = 0; i < translations; i++) 
			{
				FloatVariableService_LookupTableTranslation<K> *floatVariableService = new FloatVariableService_LookupTableTranslation<K>(xVariables[i], lookupTableConfig);
				serviceLocator->LocateAndCast<CallBackGroup>(TICK_CALL_BACK_GROUP)->Add(new CallBack<FloatVariableService_LookupTableTranslation<K>>(floatVariableService, &FloatVariableService_LookupTableTranslation<K>::TranslateValue));

				serviceLocator->Register(VARIABLE_BUS, variableIds[i], &floatVariableService->Value);
			}

			free(variableIds);
			free(xVariables);
		}
	};
}
#endif
