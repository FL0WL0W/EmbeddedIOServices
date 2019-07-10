#include "Service/HardwareAbstractionServiceBuilder.h"
#include "Service/VariableBusServiceBuilderRegister.h"
#include "Service/ServiceLocator.h"
#include "Packed.h"
#include "Interpolation.h"

#if !defined(FLOATVARIABLESERVICE_2AXISTABLETRANSLATION_H)
#define FLOATVARIABLESERVICE_2AXISTABLETRANSLATION_H
namespace VariableBus
{
	PACK(
	template<typename K>
	struct FloatVariableService_2AxisTableTranslationConfig
	{
	private:
		FloatVariableService_2AxisTableTranslationConfig()
		{
			
		}
		
	public:		
		constexpr const unsigned int Size() const
		{
			return sizeof(FloatVariableService_2AxisTableTranslationConfig) +
				(sizeof(K) * XResolution * YResolution);
		}

		constexpr const K *Table() const { return reinterpret_cast<const K *>(this + 1); }
		
		float MinXValue;
		float MaxXValue;
		uint8_t XResolution;
		float MinYValue;
		float MaxYValue;
		uint8_t YResolution;
		float PostMultiplier;
	});

	template<typename K>
	class FloatVariableService_2AxisTableTranslation
	{
	protected:
		float *_xVariable;
		float *_yVariable;
		const FloatVariableService_2AxisTableTranslationConfig<K> *_config;
	public:		
		float Value = 0;
        FloatVariableService_2AxisTableTranslation(float * const &xVariable, float * const &yVariable, const FloatVariableService_2AxisTableTranslationConfig<K> * const &config)
		{
			_xVariable = xVariable;
			_yVariable = yVariable;
			_config = config;
		}

		virtual void TranslateValue()
		{
			float xVariable = *_xVariable;
			float yVariable = *_yVariable;

			Value = Interpolation::InterpolateTable2<K>(xVariable, _config->MaxXValue, _config->MinXValue, _config->XResolution, yVariable, _config->MaxYValue, _config->MinYValue, _config->YResolution, _config->Table()) * _config->PostMultiplier;
		}

		static void BuildFloatVariableService_2AxisTableTranslation(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
		{
			const uint8_t translations = ServiceBuilder::CastAndOffset<uint8_t>(config, sizeOut);
			uint32_t *variableIds = reinterpret_cast<uint32_t *>(std::malloc(sizeof(uint32_t) * translations));
			float **xVariables = reinterpret_cast<float **>(std::malloc(sizeof(float *) * translations));
			float **yVariables = reinterpret_cast<float **>(std::malloc(sizeof(float *) * translations));
			for(int i = 0; i < translations; i++) 
			{
				variableIds[i] = ServiceBuilder::CastAndOffset<uint16_t>(config, sizeOut);
				xVariables[i] = serviceLocator->LocateAndCast<float>(VARIABLE_BUS, ServiceBuilder::CastAndOffset<uint16_t>(config, sizeOut));
				yVariables[i] = serviceLocator->LocateAndCast<float>(VARIABLE_BUS, ServiceBuilder::CastAndOffset<uint16_t>(config, sizeOut));
			}
			const FloatVariableService_2AxisTableTranslationConfig<K> *tableConfig = reinterpret_cast<const FloatVariableService_2AxisTableTranslationConfig<K> *>(config);
			sizeOut += tableConfig->Size();

			for(int i = 0; i < translations; i++) 
			{
				FloatVariableService_2AxisTableTranslation<K> *floatVariableService = new FloatVariableService_2AxisTableTranslation<K>(xVariables[i], yVariables[i], tableConfig);
				serviceLocator->LocateAndCast<CallBackGroup>(TICK_CALL_BACK_GROUP)->Add(new CallBack<FloatVariableService_2AxisTableTranslation<K>>(floatVariableService, &FloatVariableService_2AxisTableTranslation<K>::TranslateValue));

				serviceLocator->Register(VARIABLE_BUS, variableIds[i], &floatVariableService->Value);
			}

			free(variableIds);
			free(xVariables);
			free(yVariables);
		}
	};
}
#endif
