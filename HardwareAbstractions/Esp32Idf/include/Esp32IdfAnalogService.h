#include "IAnalogService.h"
#include "Esp32IdfConf.h"
#include "stdint.h"

#ifndef ESP32IDFANALOGSERVICE_H
#define ESP32IDFANALOGSERVICE_H

namespace Esp32
{
	class Esp32IdfAnalogService : public EmbeddedIOServices::IAnalogService
	{
	protected:
	
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
  		ADC_HandleTypeDef hadc1 = {0};
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
  		ADC_HandleTypeDef hadc2 = {0};
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
  		ADC_HandleTypeDef hadc3 = {0};
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
  		ADC_HandleTypeDef hadc4 = {0};
#endif
	public:
		Esp32IdfAnalogService();
		void InitPin(uint16_t pin);
		float ReadPin(uint16_t pin);
	};
}

#endif