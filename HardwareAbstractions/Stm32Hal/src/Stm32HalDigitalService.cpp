#include <stdint.h>
#include "Stm32HalDigitalService.h"

using namespace EmbeddedIOServices;

namespace Stm32
{
	void Stm32HalDigitalService::InitPin(uint16_t pin, PinDirection direction)
	{		
		if (pin == 0xFFFF)
			return;

		EnableGPIOClock(pin);

  		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = PinToGPIO_Pin(pin);
		GPIO_InitStruct.Mode = direction == In ? GPIO_MODE_INPUT : GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(PinToGPIO(pin), &GPIO_InitStruct);
	}
	
	bool Stm32HalDigitalService::ReadPin(uint16_t pin)
	{
		if (pin == 0xFFFF)
			return false;
		
		return HAL_GPIO_ReadPin(PinToGPIO(pin), PinToGPIO_Pin(pin)) == GPIO_PIN_SET;
	}
	
	void Stm32HalDigitalService::WritePin(uint16_t pin, bool value)
	{
		if (pin == 0xFFFF)
			return;
		
  		HAL_GPIO_WritePin(PinToGPIO(pin), PinToGPIO_Pin(pin), value? GPIO_PIN_SET : GPIO_PIN_RESET);
	}

	void Stm32HalDigitalService::AttachInterrupt(uint16_t pin, std::function<void()> callBack)
	{
		if (pin == 0xFFFF)
			return;
		
		attachInterrupt(pin, callBack);
	}

	void Stm32HalDigitalService::DetachInterrupt(uint16_t pin)
	{
		if (pin == 0xFFFF)
			return;
		
		detachInterrupt(pin);
	}
}