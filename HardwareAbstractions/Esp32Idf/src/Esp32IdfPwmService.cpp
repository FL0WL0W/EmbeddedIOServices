#include "Esp32IdfPwmService.h"
#include <algorithm>
#include <math.h>
#include "driver/ledc.h"
#include "esp_clk_tree.h"
#include "esp_log.h"

using namespace EmbeddedIOServices;

namespace Esp32
{		
	void Esp32IdfPwmService::InitPin(pwmpin_t pin, PinDirection direction, uint16_t minFrequency)
	{
		if (pin == 0xFFFF)
			return;
			
		auto handleIterator = _pinHandleMap.find(pin);

		if(handleIterator != _pinHandleMap.end())
		{

		}
		else
		{
			uint8_t ledcTimer = _ledcTimerIdx++;
			if(ledcTimer >= LEDC_TIMER_MAX)
				return;
			uint8_t ledcChannel = ledcTimer;

			// Prepare and then apply the LEDC PWM timer configuration
			ledc_timer_config_t ledc_timer = {
				.speed_mode       = LEDC_LOW_SPEED_MODE,
				.duty_resolution  = (ledc_timer_bit_t)ledc_find_suitable_duty_resolution(80000000, minFrequency),
				.timer_num        = (ledc_timer_t)ledcTimer,
				.freq_hz          = minFrequency,
				.clk_cfg          = LEDC_USE_PLL_DIV_CLK
			};
			ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

			// Prepare and then apply the LEDC PWM channel configuration
			ledc_channel_config_t ledc_channel = {
				.gpio_num       = pin,
				.speed_mode     = LEDC_LOW_SPEED_MODE,
				.channel        = (ledc_channel_t)ledcChannel,
				.intr_type      = LEDC_INTR_DISABLE,
				.timer_sel      = (ledc_timer_t)ledcTimer,
				.duty           = 0, // Set duty to 0%
				.hpoint         = 0
			};
			ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

			_pinHandleMap.insert({ pin, { ledcTimer, ledcChannel } });
		}
	}
		
	PwmValue Esp32IdfPwmService::ReadPin(pwmpin_t pin)
	{
		PwmValue value = PwmValue();
		if (pin == 0xFFFF)
			return value;
		
		return value;
	}
	
	void Esp32IdfPwmService::WritePin(pwmpin_t pin, PwmValue value)
	{
		if (pin == 0xFFFF)
			return;

		if(value.Period == std::numeric_limits<float>::quiet_NaN() || value.PulseWidth == std::numeric_limits<float>::quiet_NaN())
			return;

		auto handleIterator = _pinHandleMap.find(pin);

		if(handleIterator != _pinHandleMap.end()) 
		{
			const int frequency = 1 / value.Period;
			//this is kinda hacky but it handle edgecase where output should be high
			if(value.PulseWidth < 0 || value.PulseWidth == std::numeric_limits<float>::infinity())
			{
				ledc_timer_set(LEDC_LOW_SPEED_MODE, (ledc_timer_t)handleIterator->second.Timer, 1, 1, LEDC_SCLK);
				ledc_set_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)handleIterator->second.Channel, 2);
				ledc_update_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)handleIterator->second.Channel);
			}
			else if(frequency < 1)
			{
				ledc_timer_set(LEDC_LOW_SPEED_MODE, (ledc_timer_t)handleIterator->second.Timer, 1, 1, LEDC_SCLK);
				ledc_set_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)handleIterator->second.Channel, 0);
				ledc_update_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)handleIterator->second.Channel);
			}
			else
			{
				const int duty_resolution = ledc_find_suitable_duty_resolution(80000000, frequency);
				const uint32_t precision = 1 << duty_resolution;
				const uint32_t div_param = (((uint64_t) 80000000 << 8) + frequency * precision / 2) / (frequency * precision);
				const uint32_t duty = (value.PulseWidth / value.Period) * precision;
				ledc_timer_set(LEDC_LOW_SPEED_MODE, (ledc_timer_t)handleIterator->second.Timer, div_param, duty_resolution, LEDC_SCLK);
				ledc_set_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)handleIterator->second.Channel, duty);
				ledc_update_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)handleIterator->second.Channel);
			}
		}
	}
}