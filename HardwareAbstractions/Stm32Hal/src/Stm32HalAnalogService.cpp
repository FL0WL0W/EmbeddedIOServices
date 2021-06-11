#include <stdint.h>
#include "Stm32HalAnalogService.h"

namespace Stm32
{
	Stm32HalAnalogService::Stm32HalAnalogService()
	{
#if \
defined(ADC1_IN0_PIN) || \
defined(ADC1_IN1_PIN) || \
defined(ADC1_IN2_PIN) || \
defined(ADC1_IN3_PIN) || \
defined(ADC1_IN4_PIN) || \
defined(ADC1_IN5_PIN) || \
defined(ADC1_IN6_PIN) || \
defined(ADC1_IN7_PIN) || \
defined(ADC1_IN8_PIN) || \
defined(ADC1_IN9_PIN) || \
defined(ADC1_IN10_PIN) || \
defined(ADC1_IN11_PIN) || \
defined(ADC1_IN12_PIN) || \
defined(ADC1_IN13_PIN) || \
defined(ADC1_IN14_PIN) || \
defined(ADC1_IN15_PIN)
  		__HAL_RCC_ADC1_CLK_ENABLE();
		hadc1.Instance = ADC1;

		hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
		hadc1.Init.Resolution = ADC_RESOLUTION_12B;
		hadc1.Init.ScanConvMode = DISABLE;
		hadc1.Init.ContinuousConvMode = DISABLE;
		hadc1.Init.DiscontinuousConvMode = DISABLE;
		hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
		hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
		hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
		hadc1.Init.NbrOfConversion = 1;
		hadc1.Init.DMAContinuousRequests = DISABLE;
		hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
		HAL_ADC_Init(&hadc1);
#endif

#if \
defined(ADC2_IN0_PIN) || \
defined(ADC2_IN1_PIN) || \
defined(ADC2_IN2_PIN) || \
defined(ADC2_IN3_PIN) || \
defined(ADC2_IN4_PIN) || \
defined(ADC2_IN5_PIN) || \
defined(ADC2_IN6_PIN) || \
defined(ADC2_IN7_PIN) || \
defined(ADC2_IN8_PIN) || \
defined(ADC2_IN9_PIN) || \
defined(ADC2_IN10_PIN) || \
defined(ADC2_IN11_PIN) || \
defined(ADC2_IN12_PIN) || \
defined(ADC2_IN13_PIN) || \
defined(ADC2_IN14_PIN) || \
defined(ADC2_IN15_PIN)
  		__HAL_RCC_ADC2_CLK_ENABLE();
		hadc2.Instance = ADC2;
		hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
		hadc2.Init.ContinuousConvMode = DISABLE;
		hadc2.Init.DiscontinuousConvMode = DISABLE;
		hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
		hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
		hadc2.Init.NbrOfConversion = 1;
		HAL_ADC_Init(&hadc2);
#endif

#if \
defined(ADC3_IN0_PIN) || \
defined(ADC3_IN1_PIN) || \
defined(ADC3_IN2_PIN) || \
defined(ADC3_IN3_PIN) || \
defined(ADC3_IN4_PIN) || \
defined(ADC3_IN5_PIN) || \
defined(ADC3_IN6_PIN) || \
defined(ADC3_IN7_PIN) || \
defined(ADC3_IN8_PIN) || \
defined(ADC3_IN9_PIN) || \
defined(ADC3_IN10_PIN) || \
defined(ADC3_IN11_PIN) || \
defined(ADC3_IN12_PIN) || \
defined(ADC3_IN13_PIN) || \
defined(ADC3_IN14_PIN) || \
defined(ADC3_IN15_PIN)
  		__HAL_RCC_ADC3_CLK_ENABLE();
		hadc3.Instance = ADC3;
		hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
		hadc3.Init.ContinuousConvMode = DISABLE;
		hadc3.Init.DiscontinuousConvMode = DISABLE;
		hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
		hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
		hadc3.Init.NbrOfConversion = 1;
		HAL_ADC_Init(&hadc3);
#endif

#if \
defined(ADC4_IN0_PIN) || \
defined(ADC4_IN1_PIN) || \
defined(ADC4_IN2_PIN) || \
defined(ADC4_IN3_PIN) || \
defined(ADC4_IN4_PIN) || \
defined(ADC4_IN5_PIN) || \
defined(ADC4_IN6_PIN) || \
defined(ADC4_IN7_PIN) || \
defined(ADC4_IN8_PIN) || \
defined(ADC4_IN9_PIN) || \
defined(ADC4_IN10_PIN) || \
defined(ADC4_IN11_PIN) || \
defined(ADC4_IN12_PIN) || \
defined(ADC4_IN13_PIN) || \
defined(ADC4_IN14_PIN) || \
defined(ADC4_IN15_PIN)
  		__HAL_RCC_ADC4_CLK_ENABLE();
		hadc4.Instance = ADC4;
		hadc4.Init.ScanConvMode = ADC_SCAN_DISABLE;
		hadc4.Init.ContinuousConvMode = DISABLE;
		hadc4.Init.DiscontinuousConvMode = DISABLE;
		hadc4.Init.ExternalTrigConv = ADC_SOFTWARE_START;
		hadc4.Init.DataAlign = ADC_DATAALIGN_RIGHT;
		hadc4.Init.NbrOfConversion = 1;
		HAL_ADC_Init(&hadc4);
#endif
	}

	void Stm32HalAnalogService::InitPin(uint16_t pin)
	{
		if (pin == 0)
			return;

		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = PinToGPIO_Pin(pin);
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(PinToGPIO(pin), &GPIO_InitStruct);
	}
	
	float Stm32HalAnalogService::ReadPin(uint16_t pin)
	{
		float adcConversionValue = 1;
  		ADC_ChannelConfTypeDef sConfig = {0};
		ADC_HandleTypeDef *hadc = 0;

		switch(pin)
		{
#ifdef ADC1_IN0_PIN
			case ADC1_IN0_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_0;
				hadc = &hadc1;
				break;
#endif
#ifdef ADC1_IN1_PIN
			case ADC1_IN1_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_1;
				hadc = &hadc1;
				break;
#endif
#ifdef ADC1_IN2_PIN
			case ADC1_IN2_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_2;
				hadc = &hadc1;
				break;
#endif
#ifdef ADC1_IN3_PIN
			case ADC1_IN3_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_3;
				hadc = &hadc1;
				break;
#endif
#ifdef ADC1_IN4_PIN
			case ADC1_IN4_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_4;
				hadc = &hadc1;
				break;
#endif
#ifdef ADC1_IN5_PIN
			case ADC1_IN5_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_5;
				hadc = &hadc1;
				break;
#endif
#ifdef ADC1_IN6_PIN
			case ADC1_IN6_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_6;
				hadc = &hadc1;
				break;
#endif
#ifdef ADC1_IN7_PIN
			case ADC1_IN7_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_7;
				hadc = &hadc1;
				break;
#endif
#ifdef ADC1_IN8_PIN
			case ADC1_IN8_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_8;
				hadc = &hadc1;
				break;
#endif
#ifdef ADC1_IN9_PIN
			case ADC1_IN9_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_9;
				hadc = &hadc1;
				break;
#endif
#ifdef ADC1_IN10_PIN
			case ADC1_IN10_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_10;
				hadc = &hadc1;
				break;
#endif
#ifdef ADC1_IN11_PIN
			case ADC1_IN11_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_11;
				hadc = &hadc1;
				break;
#endif
#ifdef ADC1_IN12_PIN
			case ADC1_IN12_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_12;
				hadc = &hadc1;
				break;
#endif
#ifdef ADC1_IN13_PIN
			case ADC1_IN13_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_13;
				hadc = &hadc1;
				break;
#endif
#ifdef ADC1_IN14_PIN
			case ADC1_IN14_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_14;
				hadc = &hadc1;
				break;
#endif
#ifdef ADC1_IN15_PIN
			case ADC1_IN15_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_15;
				hadc = &hadc1;
				break;
#endif
#ifdef ADC2_IN0_PIN
			case ADC2_IN0_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_0;
				hadc = &hadc2;
				break;
#endif
#ifdef ADC2_IN1_PIN
			case ADC2_IN1_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_1;
				hadc = &hadc2;
				break;
#endif
#ifdef ADC2_IN2_PIN
			case ADC2_IN2_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_2;
				hadc = &hadc2;
				break;
#endif
#ifdef ADC2_IN3_PIN
			case ADC2_IN3_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_3;
				hadc = &hadc2;
				break;
#endif
#ifdef ADC2_IN4_PIN
			case ADC2_IN4_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_4;
				hadc = &hadc2;
				break;
#endif
#ifdef ADC2_IN5_PIN
			case ADC2_IN5_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_5;
				hadc = &hadc2;
				break;
#endif
#ifdef ADC2_IN6_PIN
			case ADC2_IN6_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_6;
				hadc = &hadc2;
				break;
#endif
#ifdef ADC2_IN7_PIN
			case ADC2_IN7_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_7;
				hadc = &hadc2;
				break;
#endif
#ifdef ADC2_IN8_PIN
			case ADC2_IN8_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_8;
				hadc = &hadc2;
				break;
#endif
#ifdef ADC2_IN9_PIN
			case ADC2_IN9_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_9;
				hadc = &hadc2;
				break;
#endif
#ifdef ADC2_IN10_PIN
			case ADC2_IN10_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_10;
				hadc = &hadc2;
				break;
#endif
#ifdef ADC2_IN11_PIN
			case ADC2_IN11_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_11;
				hadc = &hadc2;
				break;
#endif
#ifdef ADC2_IN12_PIN
			case ADC2_IN12_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_12;
				hadc = &hadc2;
				break;
#endif
#ifdef ADC2_IN13_PIN
			case ADC2_IN13_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_13;
				hadc = &hadc2;
				break;
#endif
#ifdef ADC2_IN14_PIN
			case ADC2_IN14_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_14;
				hadc = &hadc2;
				break;
#endif
#ifdef ADC2_IN15_PIN
			case ADC2_IN15_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_15;
				hadc = &hadc2;
				break;
#endif
#ifdef ADC3_IN0_PIN
			case ADC3_IN0_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_0;
				hadc = &hadc3;
				break;
#endif
#ifdef ADC3_IN1_PIN
			case ADC3_IN1_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_1;
				hadc = &hadc3;
				break;
#endif
#ifdef ADC3_IN2_PIN
			case ADC3_IN2_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_2;
				hadc = &hadc3;
				break;
#endif
#ifdef ADC3_IN3_PIN
			case ADC3_IN3_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_3;
				hadc = &hadc3;
				break;
#endif
#ifdef ADC3_IN4_PIN
			case ADC3_IN4_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_4;
				hadc = &hadc3;
				break;
#endif
#ifdef ADC3_IN5_PIN
			case ADC3_IN5_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_5;
				hadc = &hadc3;
				break;
#endif
#ifdef ADC3_IN6_PIN
			case ADC3_IN6_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_6;
				hadc = &hadc3;
				break;
#endif
#ifdef ADC3_IN7_PIN
			case ADC3_IN7_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_7;
				hadc = &hadc3;
				break;
#endif
#ifdef ADC3_IN8_PIN
			case ADC3_IN8_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_8;
				hadc = &hadc3;
				break;
#endif
#ifdef ADC3_IN9_PIN
			case ADC3_IN9_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_9;
				hadc = &hadc3;
				break;
#endif
#ifdef ADC3_IN10_PIN
			case ADC3_IN10_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_10;
				hadc = &hadc3;
				break;
#endif
#ifdef ADC3_IN11_PIN
			case ADC3_IN11_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_11;
				hadc = &hadc3;
				break;
#endif
#ifdef ADC3_IN12_PIN
			case ADC3_IN12_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_12;
				hadc = &hadc3;
				break;
#endif
#ifdef ADC3_IN13_PIN
			case ADC3_IN13_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_13;
				hadc = &hadc3;
				break;
#endif
#ifdef ADC3_IN14_PIN
			case ADC3_IN14_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_14;
				hadc = &hadc3;
				break;
#endif
#ifdef ADC3_IN15_PIN
			case ADC3_IN15_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_15;
				hadc = &hadc3;
				break;
#endif
#ifdef ADC4_IN0_PIN
			case ADC4_IN0_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_0;
				hadc = &hadc4;
				break;
#endif
#ifdef ADC4_IN1_PIN
			case ADC4_IN1_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_1;
				hadc = &hadc4;
				break;
#endif
#ifdef ADC4_IN2_PIN
			case ADC4_IN2_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_2;
				hadc = &hadc4;
				break;
#endif
#ifdef ADC4_IN3_PIN
			case ADC4_IN3_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_3;
				hadc = &hadc4;
				break;
#endif
#ifdef ADC4_IN4_PIN
			case ADC4_IN4_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_4;
				hadc = &hadc4;
				break;
#endif
#ifdef ADC4_IN5_PIN
			case ADC4_IN5_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_5;
				hadc = &hadc4;
				break;
#endif
#ifdef ADC4_IN6_PIN
			case ADC4_IN6_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_6;
				hadc = &hadc4;
				break;
#endif
#ifdef ADC4_IN7_PIN
			case ADC4_IN7_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_7;
				hadc = &hadc4;
				break;
#endif
#ifdef ADC4_IN8_PIN
			case ADC4_IN8_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_8;
				hadc = &hadc4;
				break;
#endif
#ifdef ADC4_IN9_PIN
			case ADC4_IN9_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_9;
				hadc = &hadc4;
				break;
#endif
#ifdef ADC4_IN10_PIN
			case ADC4_IN10_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_10;
				hadc = &hadc4;
				break;
#endif
#ifdef ADC4_IN11_PIN
			case ADC4_IN11_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_11;
				hadc = &hadc4;
				break;
#endif
#ifdef ADC4_IN12_PIN
			case ADC4_IN12_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_12;
				hadc = &hadc4;
				break;
#endif
#ifdef ADC4_IN13_PIN
			case ADC4_IN13_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_13;
				hadc = &hadc4;
				break;
#endif
#ifdef ADC4_IN14_PIN
			case ADC4_IN14_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_14;
				hadc = &hadc4;
				break;
#endif
#ifdef ADC4_IN15_PIN
			case ADC4_IN15_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_15;
				hadc = &hadc4;
				break;
#endif
		}

		sConfig.Rank = 1;
		sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;

		if(hadc != 0)
		{
			if (HAL_ADC_ConfigChannel(hadc, &sConfig) == HAL_OK)
			{
				if (HAL_ADC_Start(hadc) == HAL_OK)
				{
					if (HAL_ADC_PollForConversion(hadc, 1) == HAL_OK)
					{
						float value = HAL_ADC_GetValue(hadc) * adcConversionValue;
						if (HAL_ADC_Stop(hadc) == HAL_OK)
						{
							return value;
						}
					}
				}
			}
		}

		return -1000000;//return bogus value
	}
}