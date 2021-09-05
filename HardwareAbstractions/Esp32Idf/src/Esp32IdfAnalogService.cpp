#include <stdint.h>
#include "Esp32IdfAnalogService.h"
#include "driver/adc.h"
#include "soc/adc_channel.h"

namespace Esp32
{
	struct adc_channel_t 
	{ 
		uint8_t adc : 1;
		uint8_t channel : 7;
	};

	adc_channel_t PinToChannel(uint16_t pin)
	{
		switch(pin)
		{

#ifdef ADC1_CHANNEL_0_GPIO_NUM
			case ADC1_CHANNEL_0_GPIO_NUM: return { 0, ADC1_CHANNEL_0 }; 
#endif
#ifdef ADC1_CHANNEL_1_GPIO_NUM
			case ADC1_CHANNEL_1_GPIO_NUM: return { 0, ADC1_CHANNEL_1 };
#endif
#ifdef ADC1_CHANNEL_2_GPIO_NUM
			case ADC1_CHANNEL_2_GPIO_NUM: return { 0, ADC1_CHANNEL_2 };
#endif
#ifdef ADC1_CHANNEL_3_GPIO_NUM
			case ADC1_CHANNEL_3_GPIO_NUM: return { 0, ADC1_CHANNEL_3 };
#endif
#ifdef ADC1_CHANNEL_4_GPIO_NUM
			case ADC1_CHANNEL_4_GPIO_NUM: return { 0, ADC1_CHANNEL_4 };
#endif
#ifdef ADC1_CHANNEL_5_GPIO_NUM
			case ADC1_CHANNEL_5_GPIO_NUM: return { 0, ADC1_CHANNEL_5 };
#endif
#ifdef ADC1_CHANNEL_6_GPIO_NUM
			case ADC1_CHANNEL_6_GPIO_NUM: return { 0, ADC1_CHANNEL_6 };
#endif
#ifdef ADC1_CHANNEL_7_GPIO_NUM
			case ADC1_CHANNEL_7_GPIO_NUM: return { 0, ADC1_CHANNEL_7 };
#endif
#ifdef ADC1_CHANNEL_8_GPIO_NUM
			case ADC1_CHANNEL_8_GPIO_NUM: return { 0, ADC1_CHANNEL_8 };
#endif
#ifdef ADC1_CHANNEL_9_GPIO_NUM
			case ADC1_CHANNEL_9_GPIO_NUM: return { 0, ADC1_CHANNEL_9 };
#endif
#ifdef ADC2_CHANNEL_0_GPIO_NUM
			case ADC2_CHANNEL_0_GPIO_NUM: return { 1, ADC2_CHANNEL_0 }; 
#endif
#ifdef ADC2_CHANNEL_1_GPIO_NUM
			case ADC2_CHANNEL_1_GPIO_NUM: return { 1, ADC2_CHANNEL_1 };
#endif
#ifdef ADC2_CHANNEL_2_GPIO_NUM
			case ADC2_CHANNEL_2_GPIO_NUM: return { 1, ADC2_CHANNEL_2 };
#endif
#ifdef ADC2_CHANNEL_3_GPIO_NUM
			case ADC2_CHANNEL_3_GPIO_NUM: return { 1, ADC2_CHANNEL_3 };
#endif
#ifdef ADC2_CHANNEL_4_GPIO_NUM
			case ADC2_CHANNEL_4_GPIO_NUM: return { 1, ADC2_CHANNEL_4 };
#endif
#ifdef ADC2_CHANNEL_5_GPIO_NUM
			case ADC2_CHANNEL_5_GPIO_NUM: return { 1, ADC2_CHANNEL_5 };
#endif
#ifdef ADC2_CHANNEL_6_GPIO_NUM
			case ADC2_CHANNEL_6_GPIO_NUM: return { 1, ADC2_CHANNEL_6 };
#endif
#ifdef ADC2_CHANNEL_7_GPIO_NUM
			case ADC2_CHANNEL_7_GPIO_NUM: return { 1, ADC2_CHANNEL_7 };
#endif
#ifdef ADC2_CHANNEL_8_GPIO_NUM
			case ADC2_CHANNEL_8_GPIO_NUM: return { 1, ADC2_CHANNEL_8 };
#endif
#ifdef ADC2_CHANNEL_9_GPIO_NUM
			case ADC2_CHANNEL_9_GPIO_NUM: return { 1, ADC2_CHANNEL_9 };
#endif
		}
		return { 1, 0x7F };
	}
static const adc_atten_t atten = ADC_ATTEN_DB_0;

	Esp32IdfAnalogService::Esp32IdfAnalogService(adc_atten_t atten) :
		_atten(atten)
	{
		//configure adc1 width
		adc1_config_width(static_cast<adc_bits_width_t>(ADC_WIDTH_MAX - 1));

		//calibrate adc1
		_adc1_chars = static_cast<esp_adc_cal_characteristics_t*>(calloc(1, sizeof(esp_adc_cal_characteristics_t)));
		esp_adc_cal_characterize(ADC_UNIT_1, atten, static_cast<adc_bits_width_t>(ADC_WIDTH_MAX - 1), 1100, _adc1_chars);
		_adc2_chars = static_cast<esp_adc_cal_characteristics_t*>(calloc(1, sizeof(esp_adc_cal_characteristics_t)));
		esp_adc_cal_characterize(ADC_UNIT_2, atten, static_cast<adc_bits_width_t>(ADC_WIDTH_MAX - 1), 1100, _adc2_chars);
	}

	void Esp32IdfAnalogService::InitPin(uint16_t pin)
	{
		adc_channel_t channel = PinToChannel(pin);
		if (channel.channel == 0x7F)
			return;

		//Configure ADC
		if (channel.adc == 0) {
			adc1_config_channel_atten(static_cast<adc1_channel_t>(channel.channel), _atten);
		} else {
			adc2_config_channel_atten(static_cast<adc2_channel_t>(channel.channel), _atten);
		}
	}
	
	float Esp32IdfAnalogService::ReadPin(uint16_t pin)
	{
		adc_channel_t channel = PinToChannel(pin);
		if (channel.channel == 0x7F)
			return -1000000;//return bogus value

		if (channel.adc == 0) {
        	uint32_t adc_reading = 0;
			adc_reading = adc1_get_raw(static_cast<adc1_channel_t>(channel.channel));
        	return esp_adc_cal_raw_to_voltage(adc_reading, _adc1_chars) * 0.001f;
		} else {
			int raw = 0;
			adc2_get_raw(static_cast<adc2_channel_t>(channel.channel), static_cast<adc_bits_width_t>(ADC_WIDTH_MAX - 1), &raw);
        	return esp_adc_cal_raw_to_voltage(raw, _adc2_chars) * 0.001f;
		}
	}
}