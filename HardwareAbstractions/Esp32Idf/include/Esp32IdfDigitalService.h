#include "PinDirection.h"
#include "IDigitalService.h"
#include "hal/gpio_hal.h"
#include "esp_intr_alloc.h"

#ifndef ESP32IDFDIGITALSERVICE_H
#define ESP32IDFDIGITALSERVICE_H

namespace Esp32
{
	class Esp32IdfDigitalService : public EmbeddedIOServices::IDigitalService
	{
	protected: 
		static EmbeddedIOServices::callback_t ExternalInterruptCallback[GPIO_NUM_MAX];
		static gpio_hal_context_t hal;
	public:
		void InitPin(uint16_t pin, EmbeddedIOServices::PinDirection direction);
		bool ReadPin(uint16_t pin);
		void WritePin(uint16_t pin, bool value);
		void AttachInterrupt(uint16_t pin, EmbeddedIOServices::callback_t);
		void DetachInterrupt(uint16_t pin);

		static void DigitalInterrupt(void *arg);
	};
}

#endif