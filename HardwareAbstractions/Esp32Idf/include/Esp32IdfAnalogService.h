#include "IAnalogService.h"
#include "Esp32IdfConf.h"
#include "stdint.h"
#include "esp_adc_cal.h"

#ifndef ESP32IDFANALOGSERVICE_H
#define ESP32IDFANALOGSERVICE_H

namespace Esp32
{
	class Esp32IdfAnalogService : public EmbeddedIOServices::IAnalogService
	{
	protected: 
		adc_atten_t _atten;
		esp_adc_cal_characteristics_t *_adc1_chars;
		esp_adc_cal_characteristics_t *_adc2_chars;
	public:
		Esp32IdfAnalogService(adc_atten_t atten);
		void InitPin(uint16_t pin);
		float ReadPin(uint16_t pin);
	};
}

#endif