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
		static intr_handle_t gpio_isr_handle;
	public:
		Esp32IdfDigitalService();
		void InitPin(uint16_t pin, EmbeddedIOServices::PinDirection direction);
		bool ReadPin(uint16_t pin);
		void WritePin(uint16_t pin, bool value);
		void AttachInterrupt(uint16_t pin, EmbeddedIOServices::callback_t);
		void DetachInterrupt(uint16_t pin);

		static void DigitalInterrupt(void *arg);
		static inline void DigitalInterruptLoop(uint32_t status, const uint32_t gpio_num_start)
		{
			while (status) 
			{
				const int nbit = __builtin_ffs(status) - 1;
				status &= ~(1 << nbit);
				const uint32_t gpio_num = gpio_num_start + nbit;

				if (ExternalInterruptCallback[gpio_num] != NULL) 
				{
					ExternalInterruptCallback[gpio_num]();
				}
			}
		}
	};
}

#endif