#include <stdint.h>
#include "Stm32HalDigitalService.h"

namespace Stm32
{
	void Stm32HalDigitalService::InitPin(unsigned short pin, HardwareAbstraction::PinDirection direction)
	{		
		if (pin == 0)
			return;

		EnableGPIOClock(pin);

  		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = PinToGPIO_Pin(pin);
		GPIO_InitStruct.Mode = HardwareAbstraction::In ? GPIO_MODE_INPUT : GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(PinToGPIO(pin), &GPIO_InitStruct);
	}
	
	bool Stm32HalDigitalService::ReadPin(unsigned short pin)
	{
		if (pin == 0)
			return false;
		
		return HAL_GPIO_ReadPin(PinToGPIO(pin), PinToGPIO_Pin(pin)) == GPIO_PIN_SET;
	}
	
	void Stm32HalDigitalService::WritePin(unsigned short pin, bool value)
	{
		if (pin == 0)
			return;
		
  		HAL_GPIO_WritePin(PinToGPIO(pin), PinToGPIO_Pin(pin), value? GPIO_PIN_SET : GPIO_PIN_RESET);
	}

	void Stm32HalDigitalService::ScheduleRecurringInterrupt(unsigned short pin, ICallBack *callBack)
	{
		//TODO
		//Setup Interrupt
	}

	void Stm32HalDigitalService::ScheduleNextInterrupt(unsigned short pin, ICallBack *callBack)
	{
		//TODO
	}
}