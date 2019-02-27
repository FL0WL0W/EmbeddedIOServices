#include <stdint.h>
#include "Stm32HalAnalogService.h"

namespace Stm32
{
	Stm32HalAnalogService::Stm32HalAnalogService()
	{
	ADC_HandleTypeDef hadc;
	
	hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc.Init.ContinuousConvMode = DISABLE;
	hadc.Init.DiscontinuousConvMode = DISABLE;
	hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc.Init.NbrOfConversion = 1;
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
		hadc.Instance = ADC1;
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
		hadc.Instance = ADC2;
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
		hadc.Instance = ADC3;
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
		hadc.Instance = ADC4;
#endif

		HAL_ADC_Init(&hadc);
	}

	void Stm32HalAnalogService::InitPin(uint16_t pin)
	{
		if (pin == 0)
			return;

		ADC_HandleTypeDef hadc;
		
		hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
		hadc.Init.ContinuousConvMode = DISABLE;
		hadc.Init.DiscontinuousConvMode = DISABLE;
		hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
		hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
		hadc.Init.NbrOfConversion = 1;

  		ADC_ChannelConfTypeDef sConfig = {0};

		switch(pin)
		{
#ifdef ADC1_IN0_PIN
			case ADC1_IN0_PIN:
  				sConfig.Channel = ADC_CHANNEL_0;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN1_PIN
			case ADC1_IN1_PIN:
  				sConfig.Channel = ADC_CHANNEL_1;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN2_PIN
			case ADC1_IN2_PIN:
  				sConfig.Channel = ADC_CHANNEL_2;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN3_PIN
			case ADC1_IN3_PIN:
  				sConfig.Channel = ADC_CHANNEL_3;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN4_PIN
			case ADC1_IN4_PIN:
  				sConfig.Channel = ADC_CHANNEL_4;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN5_PIN
			case ADC1_IN5_PIN:
  				sConfig.Channel = ADC_CHANNEL_5;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN6_PIN
			case ADC1_IN6_PIN:
  				sConfig.Channel = ADC_CHANNEL_6;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN7_PIN
			case ADC1_IN7_PIN:
  				sConfig.Channel = ADC_CHANNEL_7;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN8_PIN
			case ADC1_IN8_PIN:
  				sConfig.Channel = ADC_CHANNEL_8;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN9_PIN
			case ADC1_IN9_PIN:
  				sConfig.Channel = ADC_CHANNEL_9;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN10_PIN
			case ADC1_IN10_PIN:
  				sConfig.Channel = ADC_CHANNEL_10;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN11_PIN
			case ADC1_IN11_PIN:
  				sConfig.Channel = ADC_CHANNEL_11;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN12_PIN
			case ADC1_IN12_PIN:
  				sConfig.Channel = ADC_CHANNEL_12;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN13_PIN
			case ADC1_IN13_PIN:
  				sConfig.Channel = ADC_CHANNEL_13;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN14_PIN
			case ADC1_IN14_PIN:
  				sConfig.Channel = ADC_CHANNEL_14;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN15_PIN
			case ADC1_IN15_PIN:
  				sConfig.Channel = ADC_CHANNEL_15;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC2_IN0_PIN
			case ADC2_IN0_PIN:
  				sConfig.Channel = ADC_CHANNEL_0;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN1_PIN
			case ADC2_IN1_PIN:
  				sConfig.Channel = ADC_CHANNEL_1;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN2_PIN
			case ADC2_IN2_PIN:
  				sConfig.Channel = ADC_CHANNEL_2;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN3_PIN
			case ADC2_IN3_PIN:
  				sConfig.Channel = ADC_CHANNEL_3;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN4_PIN
			case ADC2_IN4_PIN:
  				sConfig.Channel = ADC_CHANNEL_4;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN5_PIN
			case ADC2_IN5_PIN:
  				sConfig.Channel = ADC_CHANNEL_5;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN6_PIN
			case ADC2_IN6_PIN:
  				sConfig.Channel = ADC_CHANNEL_6;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN7_PIN
			case ADC2_IN7_PIN:
  				sConfig.Channel = ADC_CHANNEL_7;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN8_PIN
			case ADC2_IN8_PIN:
  				sConfig.Channel = ADC_CHANNEL_8;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN9_PIN
			case ADC2_IN9_PIN:
  				sConfig.Channel = ADC_CHANNEL_9;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN10_PIN
			case ADC2_IN10_PIN:
  				sConfig.Channel = ADC_CHANNEL_10;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN11_PIN
			case ADC2_IN11_PIN:
  				sConfig.Channel = ADC_CHANNEL_11;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN12_PIN
			case ADC2_IN12_PIN:
  				sConfig.Channel = ADC_CHANNEL_12;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN13_PIN
			case ADC2_IN13_PIN:
  				sConfig.Channel = ADC_CHANNEL_13;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN14_PIN
			case ADC2_IN14_PIN:
  				sConfig.Channel = ADC_CHANNEL_14;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN15_PIN
			case ADC2_IN15_PIN:
  				sConfig.Channel = ADC_CHANNEL_15;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC3_IN0_PIN
			case ADC3_IN0_PIN:
  				sConfig.Channel = ADC_CHANNEL_0;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN1_PIN
			case ADC3_IN1_PIN:
  				sConfig.Channel = ADC_CHANNEL_1;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN2_PIN
			case ADC3_IN2_PIN:
  				sConfig.Channel = ADC_CHANNEL_2;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN3_PIN
			case ADC3_IN3_PIN:
  				sConfig.Channel = ADC_CHANNEL_3;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN4_PIN
			case ADC3_IN4_PIN:
  				sConfig.Channel = ADC_CHANNEL_4;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN5_PIN
			case ADC3_IN5_PIN:
  				sConfig.Channel = ADC_CHANNEL_5;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN6_PIN
			case ADC3_IN6_PIN:
  				sConfig.Channel = ADC_CHANNEL_6;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN7_PIN
			case ADC3_IN7_PIN:
  				sConfig.Channel = ADC_CHANNEL_7;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN8_PIN
			case ADC3_IN8_PIN:
  				sConfig.Channel = ADC_CHANNEL_8;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN9_PIN
			case ADC3_IN9_PIN:
  				sConfig.Channel = ADC_CHANNEL_9;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN10_PIN
			case ADC3_IN10_PIN:
  				sConfig.Channel = ADC_CHANNEL_10;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN11_PIN
			case ADC3_IN11_PIN:
  				sConfig.Channel = ADC_CHANNEL_11;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN12_PIN
			case ADC3_IN12_PIN:
  				sConfig.Channel = ADC_CHANNEL_12;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN13_PIN
			case ADC3_IN13_PIN:
  				sConfig.Channel = ADC_CHANNEL_13;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN14_PIN
			case ADC3_IN14_PIN:
  				sConfig.Channel = ADC_CHANNEL_14;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN15_PIN
			case ADC3_IN15_PIN:
  				sConfig.Channel = ADC_CHANNEL_15;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC4_IN0_PIN
			case ADC4_IN0_PIN:
  				sConfig.Channel = ADC_CHANNEL_0;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN1_PIN
			case ADC4_IN1_PIN:
  				sConfig.Channel = ADC_CHANNEL_1;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN2_PIN
			case ADC4_IN2_PIN:
  				sConfig.Channel = ADC_CHANNEL_2;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN3_PIN
			case ADC4_IN3_PIN:
  				sConfig.Channel = ADC_CHANNEL_3;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN4_PIN
			case ADC4_IN4_PIN:
  				sConfig.Channel = ADC_CHANNEL_4;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN5_PIN
			case ADC4_IN5_PIN:
  				sConfig.Channel = ADC_CHANNEL_5;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN6_PIN
			case ADC4_IN6_PIN:
  				sConfig.Channel = ADC_CHANNEL_6;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN7_PIN
			case ADC4_IN7_PIN:
  				sConfig.Channel = ADC_CHANNEL_7;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN8_PIN
			case ADC4_IN8_PIN:
  				sConfig.Channel = ADC_CHANNEL_8;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN9_PIN
			case ADC4_IN9_PIN:
  				sConfig.Channel = ADC_CHANNEL_9;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN10_PIN
			case ADC4_IN10_PIN:
  				sConfig.Channel = ADC_CHANNEL_10;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN11_PIN
			case ADC4_IN11_PIN:
  				sConfig.Channel = ADC_CHANNEL_11;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN12_PIN
			case ADC4_IN12_PIN:
  				sConfig.Channel = ADC_CHANNEL_12;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN13_PIN
			case ADC4_IN13_PIN:
  				sConfig.Channel = ADC_CHANNEL_13;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN14_PIN
			case ADC4_IN14_PIN:
  				sConfig.Channel = ADC_CHANNEL_14;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN15_PIN
			case ADC4_IN15_PIN:
  				sConfig.Channel = ADC_CHANNEL_15;
				hadc.Instance = ADC4;
				break;
#endif
		}

		sConfig.Rank = ADC_REGULAR_RANK_1;
		sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
		
		HAL_ADC_ConfigChannel(&hadc, &sConfig);
		
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = PinToGPIO_Pin(pin);
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(PinToGPIO(pin), &GPIO_InitStruct);
	}
	
	float Stm32HalAnalogService::ReadPin(uint16_t pin)
	{
		ADC_HandleTypeDef hadc;
		
		hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
		hadc.Init.ContinuousConvMode = DISABLE;
		hadc.Init.DiscontinuousConvMode = DISABLE;
		hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
		hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
		hadc.Init.NbrOfConversion = 1;
		
		float adcConversionValue = 1;

  		ADC_ChannelConfTypeDef sConfig = {0};

		switch(pin)
		{
#ifdef ADC1_IN0_PIN
			case ADC1_IN0_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_0;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN1_PIN
			case ADC1_IN1_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_1;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN2_PIN
			case ADC1_IN2_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_2;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN3_PIN
			case ADC1_IN3_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_3;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN4_PIN
			case ADC1_IN4_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_4;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN5_PIN
			case ADC1_IN5_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_5;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN6_PIN
			case ADC1_IN6_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_6;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN7_PIN
			case ADC1_IN7_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_7;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN8_PIN
			case ADC1_IN8_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_8;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN9_PIN
			case ADC1_IN9_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_9;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN10_PIN
			case ADC1_IN10_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_10;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN11_PIN
			case ADC1_IN11_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_11;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN12_PIN
			case ADC1_IN12_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_12;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN13_PIN
			case ADC1_IN13_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_13;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN14_PIN
			case ADC1_IN14_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_14;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC1_IN15_PIN
			case ADC1_IN15_PIN:
				adcConversionValue = ADC1_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_15;
				hadc.Instance = ADC1;
				break;
#endif
#ifdef ADC2_IN0_PIN
			case ADC2_IN0_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_0;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN1_PIN
			case ADC2_IN1_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_1;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN2_PIN
			case ADC2_IN2_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_2;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN3_PIN
			case ADC2_IN3_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_3;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN4_PIN
			case ADC2_IN4_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_4;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN5_PIN
			case ADC2_IN5_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_5;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN6_PIN
			case ADC2_IN6_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_6;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN7_PIN
			case ADC2_IN7_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_7;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN8_PIN
			case ADC2_IN8_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_8;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN9_PIN
			case ADC2_IN9_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_9;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN10_PIN
			case ADC2_IN10_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_10;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN11_PIN
			case ADC2_IN11_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_11;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN12_PIN
			case ADC2_IN12_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_12;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN13_PIN
			case ADC2_IN13_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_13;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN14_PIN
			case ADC2_IN14_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_14;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC2_IN15_PIN
			case ADC2_IN15_PIN:
				adcConversionValue = ADC2_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_15;
				hadc.Instance = ADC2;
				break;
#endif
#ifdef ADC3_IN0_PIN
			case ADC3_IN0_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_0;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN1_PIN
			case ADC3_IN1_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_1;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN2_PIN
			case ADC3_IN2_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_2;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN3_PIN
			case ADC3_IN3_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_3;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN4_PIN
			case ADC3_IN4_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_4;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN5_PIN
			case ADC3_IN5_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_5;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN6_PIN
			case ADC3_IN6_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_6;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN7_PIN
			case ADC3_IN7_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_7;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN8_PIN
			case ADC3_IN8_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_8;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN9_PIN
			case ADC3_IN9_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_9;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN10_PIN
			case ADC3_IN10_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_10;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN11_PIN
			case ADC3_IN11_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_11;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN12_PIN
			case ADC3_IN12_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_12;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN13_PIN
			case ADC3_IN13_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_13;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN14_PIN
			case ADC3_IN14_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_14;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC3_IN15_PIN
			case ADC3_IN15_PIN:
				adcConversionValue = ADC3_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_15;
				hadc.Instance = ADC3;
				break;
#endif
#ifdef ADC4_IN0_PIN
			case ADC4_IN0_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_0;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN1_PIN
			case ADC4_IN1_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_1;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN2_PIN
			case ADC4_IN2_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_2;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN3_PIN
			case ADC4_IN3_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_3;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN4_PIN
			case ADC4_IN4_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_4;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN5_PIN
			case ADC4_IN5_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_5;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN6_PIN
			case ADC4_IN6_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_6;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN7_PIN
			case ADC4_IN7_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_7;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN8_PIN
			case ADC4_IN8_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_8;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN9_PIN
			case ADC4_IN9_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_9;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN10_PIN
			case ADC4_IN10_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_10;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN11_PIN
			case ADC4_IN11_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_11;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN12_PIN
			case ADC4_IN12_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_12;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN13_PIN
			case ADC4_IN13_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_13;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN14_PIN
			case ADC4_IN14_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_14;
				hadc.Instance = ADC4;
				break;
#endif
#ifdef ADC4_IN15_PIN
			case ADC4_IN15_PIN:
				adcConversionValue = ADC4_VOLTAGE_CONVERSION_CONSTANT;
  				sConfig.Channel = ADC_CHANNEL_15;
				hadc.Instance = ADC4;
				break;
#endif
		}

		sConfig.Rank = ADC_REGULAR_RANK_1;
		sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;

		HAL_ADC_ConfigChannel(&hadc, &sConfig);
		HAL_ADC_Start(&hadc);
		if(HAL_ADC_PollForConversion(&hadc, 100000))
		{
			return HAL_ADC_GetValue(&hadc) * adcConversionValue;
		}
	}
}