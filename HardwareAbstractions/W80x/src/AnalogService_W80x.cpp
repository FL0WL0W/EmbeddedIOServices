#include "AnalogService_W80x.h"

#ifdef ANALOGSERVICE_W80X_H
namespace EmbeddedIOServices
{
	//modify registers directly instead of using HAL
	void AnalogService_W80x::InitPin(analogpin_t pin)
	{
        __HAL_RCC_ADC_CLK_ENABLE();
        __HAL_RCC_GPIO_CLK_ENABLE();
		_hadc.Instance = ADC;
		_hadc.Init.channel = PinToChannel(pin);
		_hadc.Init.freq = 1000;
		
		HAL_ADC_Init(&_hadc);
	}
	
	float AnalogService_W80x::ReadPin(analogpin_t pin)
	{
		_hadc.Init.channel = PinToChannel(pin);
		return 0.001f * HAL_ADC_GET_INPUT_VOLTAGE(&_hadc);
	}

	uint32_t AnalogService_W80x::PinToChannel(analogpin_t pin)
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

extern "C" void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC)
    {
        __HAL_RCC_ADC_CLK_ENABLE();
        __HAL_RCC_GPIO_CLK_ENABLE();
        
        //ADC_CHANNEL_0 : PA1
        //ADC_CHANNEL_1 : PA4
        //ADC_CHANNEL_2 : PA3
        //ADC_CHANNEL_3 : PA2
        //ADC_CHANNEL_0_1 : PA1 and PA4
        //ADC_CHANNEL_2_3 : PA3 and PA2
        if (hadc->Init.channel == ADC_CHANNEL_0)
        {
            __HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_1);
        }
        else if (hadc->Init.channel == ADC_CHANNEL_1)
        {
            __HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_4);
        }
        else if (hadc->Init.channel == ADC_CHANNEL_2)
        {
            __HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_3);
        }
        else if (hadc->Init.channel == ADC_CHANNEL_3)
        {
            __HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_2);
        }
        else if (hadc->Init.channel == ADC_CHANNEL_0_1)
        {
            __HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_1);
            __HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_4);
        }
        else if (hadc->Init.channel == ADC_CHANNEL_2_3)
        {
            __HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_3);
            __HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_2);
        }
    }
}
#endif
