#include <stdint.h>
#include "Esp32IdfDigitalService.h"
#include "driver/gpio.h"

using namespace EmbeddedIOServices;

namespace Esp32
{
	Esp32IdfDigitalService::Esp32IdfDigitalService()
	{
		gpio_install_isr_service(ESP_INTR_FLAG_LEVEL3 | ESP_INTR_FLAG_IRAM);
	}

	void Esp32IdfDigitalService::InitPin(uint16_t pin, PinDirection direction)
	{		
		if (pin == 0xFFFF)
			return;

		gpio_set_direction(static_cast<gpio_num_t>(pin), direction == In? GPIO_MODE_INPUT : GPIO_MODE_OUTPUT);
	}
	
	bool IRAM_ATTR Esp32IdfDigitalService::ReadPin(uint16_t pin)
	{
		if (pin == 0xFFFF)
			return false;
		
		return gpio_get_level(static_cast<gpio_num_t>(pin)) == 1;
	}
	
	void IRAM_ATTR Esp32IdfDigitalService::WritePin(uint16_t pin, bool value)
	{
		if (pin == 0xFFFF)
			return;
		
        gpio_set_level(static_cast<gpio_num_t>(pin), value);
	}

	EmbeddedIOServices::callback_t ExternalInterruptCallback[GPIO_NUM_MAX];
	void IRAM_ATTR DigitalInterrupt(void *arg) 
	{
        gpio_set_level(static_cast<gpio_num_t>(5), true);
        gpio_set_level(static_cast<gpio_num_t>(5), false);
		// (*reinterpret_cast<callback_t *>(arg))(); 
	}

	void Esp32IdfDigitalService::AttachInterrupt(uint16_t pin, callback_t callBack)
	{
		if (pin == 0xFFFF)
			return;

			pin = 35;
		
		gpio_set_direction(static_cast<gpio_num_t>(pin), GPIO_MODE_INPUT);
		gpio_set_intr_type(static_cast<gpio_num_t>(pin), GPIO_INTR_ANYEDGE);
		gpio_intr_enable(static_cast<gpio_num_t>(pin));
		ExternalInterruptCallback[pin] = callBack;
   		gpio_isr_handler_add(static_cast<gpio_num_t>(pin), DigitalInterrupt, &ExternalInterruptCallback[pin]);
	}

	void Esp32IdfDigitalService::DetachInterrupt(uint16_t pin)
	{
		if (pin == 0xFFFF)
			return;
		
		gpio_intr_disable(static_cast<gpio_num_t>(pin));
    	gpio_isr_handler_remove(static_cast<gpio_num_t>(pin));
	}
}