#include "AnalogService_W806.h"

#ifdef ANALOGSERVICE_W806_H
namespace EmbeddedIOServices
{
	//modify registers directly instead of using HAL
	void AnalogService_W806::InitPin(analogpin_t pin)
	{
        __HAL_RCC_ADC_CLK_ENABLE();
        __HAL_RCC_GPIO_CLK_ENABLE();
		_hadc.Instance = ADC;
		_hadc.Init.channel = PinToChannel(pin);
		_hadc.Init.freq = ADC_FREQ_MAX;
		
		HAL_ADC_Init(&_hadc);
	}
	
	float AnalogService_W806::ReadPin(analogpin_t pin)
	{
		_hadc.Init.channel = PinToChannel(pin);
		return 0.001f * HAL_ADC_GET_INPUT_VOLTAGE(&_hadc);
	}

	uint32_t AnalogService_W806::PinToChannel(analogpin_t pin)
	{
		switch(pin)
		{
			default:
				return ADC_CHANNEL_0;
			case 2:
				return ADC_CHANNEL_1;
			case 3:
				return ADC_CHANNEL_2;
			case 4:
				return ADC_CHANNEL_3;
		}
	}
}
#endif
