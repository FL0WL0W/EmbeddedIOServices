#include <stdint.h>
#include "Esp32IdfDigitalService.h"
#include "hal/cpu_hal.h"
#include "driver/gpio.h"
#include "esp_attr.h"
#include "esp_rom_gpio.h"

using namespace EmbeddedIOServices;

namespace Esp32
{
	EmbeddedIOServices::callback_t Esp32IdfDigitalService::ExternalInterruptCallback[GPIO_NUM_MAX];
	gpio_hal_context_t Esp32IdfDigitalService::hal = {GPIO_HAL_GET_HW(GPIO_PORT_0)};

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
		
		return gpio_ll_get_level(hal.dev, static_cast<gpio_num_t>(pin)) == 1;
	}
	
	void IRAM_ATTR Esp32IdfDigitalService::WritePin(uint16_t pin, bool value)
	{
		if (pin == 0xFFFF)
			return;
		
        gpio_ll_set_level(hal.dev, static_cast<gpio_num_t>(pin), value);
	}

	void Esp32IdfDigitalService::AttachInterrupt(uint16_t pin, callback_t callBack)
	{
		if (pin == 0xFFFF)
			return;
		
		gpio_install_isr_service(ESP_INTR_FLAG_LEVEL3);

		InitPin(pin, In);
		const gpio_num_t gpio_num = static_cast<gpio_num_t>(pin);
    	gpio_set_intr_type(gpio_num, GPIO_INTR_ANYEDGE);
    	gpio_intr_enable(gpio_num);

		ExternalInterruptCallback[pin] = callBack;
    	gpio_isr_handler_add((gpio_num_t)pin, DigitalInterrupt, (void *)pin);
	}

	void Esp32IdfDigitalService::DetachInterrupt(uint16_t pin)
	{
		if (pin == 0xFFFF)
			return;
		
    	gpio_hal_intr_disable(&hal, static_cast<gpio_num_t>(pin));
	}

	void IRAM_ATTR Esp32IdfDigitalService::DigitalInterrupt(void *arg) 
	{
		ExternalInterruptCallback[(uint16_t)((size_t)arg)]();
	}
}