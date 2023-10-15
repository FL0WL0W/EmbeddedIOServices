#include <stdint.h>
#include "Esp32IdfDigitalService.h"
#include "hal/cpu_hal.h"
#include "esp_attr.h"
#include "esp_rom_gpio.h"

using namespace EmbeddedIOServices;

namespace Esp32
{
	EmbeddedIOServices::callback_t Esp32IdfDigitalService::ExternalInterruptCallback[GPIO_NUM_MAX];
	gpio_hal_context_t Esp32IdfDigitalService::hal = {GPIO_HAL_GET_HW(GPIO_PORT_0)};
	intr_handle_t Esp32IdfDigitalService::gpio_isr_handle;

	Esp32IdfDigitalService::Esp32IdfDigitalService()
	{
		//register interrupt
		const int intr_alloc_flags = ESP_INTR_FLAG_LEVEL3 | ESP_INTR_FLAG_IRAM;
		esp_intr_alloc(ETS_GPIO_INTR_SOURCE, intr_alloc_flags, Esp32IdfDigitalService::DigitalInterrupt, 0, &gpio_isr_handle);
	}

	void Esp32IdfDigitalService::InitPin(uint16_t pin, PinDirection direction)
	{		
		if (pin == 0xFFFF)
			return;

		const gpio_num_t gpio_num = static_cast<gpio_num_t>(pin);

		if (direction == In) {
    		gpio_hal_input_enable(&hal, gpio_num);
		} else {
    		gpio_hal_output_disable(&hal, gpio_num);
		}

		if (direction == Out) {
			gpio_hal_output_enable(&hal, gpio_num);
			esp_rom_gpio_connect_out_signal(gpio_num, SIG_GPIO_OUT_IDX, false, false);
		} else {
    		gpio_hal_od_disable(&hal, gpio_num);
		}
	}
	
	bool IRAM_ATTR Esp32IdfDigitalService::ReadPin(uint16_t pin)
	{
		if (pin == 0xFFFF)
			return false;
		
		return gpio_hal_get_level(&hal, static_cast<gpio_num_t>(pin)) == 1;
	}
	
	void IRAM_ATTR Esp32IdfDigitalService::WritePin(uint16_t pin, bool value)
	{
		if (pin == 0xFFFF)
			return;
		
        gpio_hal_set_level(&hal, static_cast<gpio_num_t>(pin), value);
	}
	
#define DIGITALINTERRUPTCORE cpu_hal_get_core_id()

	void Esp32IdfDigitalService::AttachInterrupt(uint16_t pin, callback_t callBack)
	{
		if (pin == 0xFFFF)
			return;

			pin = 35;
		
		InitPin(pin, In);
		const gpio_num_t gpio_num = static_cast<gpio_num_t>(pin);
    	gpio_hal_set_intr_type(&hal, gpio_num, GPIO_INTR_ANYEDGE);
    	gpio_hal_intr_enable_on_core(&hal, gpio_num, DIGITALINTERRUPTCORE);
		ExternalInterruptCallback[pin] = callBack;
	}

	void Esp32IdfDigitalService::DetachInterrupt(uint16_t pin)
	{
		if (pin == 0xFFFF)
			return;
		
    	gpio_hal_intr_disable(&hal, static_cast<gpio_num_t>(pin));
	}

	void IRAM_ATTR Esp32IdfDigitalService::DigitalInterrupt(void *arg) 
	{
        gpio_hal_set_level(&hal, static_cast<gpio_num_t>(5), true);
        gpio_hal_set_level(&hal, static_cast<gpio_num_t>(5), false);

		//read status to get interrupt status for GPIO0-31
		uint32_t gpio_intr_status;
		gpio_hal_get_intr_status(&hal, DIGITALINTERRUPTCORE, &gpio_intr_status);

		if (gpio_intr_status) {
			DigitalInterruptLoop(&hal, gpio_intr_status, 0);
		}

		uint32_t gpio_intr_status_h;
		gpio_hal_get_intr_status_high(&hal, DIGITALINTERRUPTCORE, &gpio_intr_status_h);

		if (gpio_intr_status_h) {
			DigitalInterruptLoop(&hal, gpio_intr_status_h, 32);
		}
	}
}