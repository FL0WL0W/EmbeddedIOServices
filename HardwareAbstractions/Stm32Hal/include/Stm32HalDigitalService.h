#include "HardwareAbstraction/PinDirection.h"
#include "HardwareAbstraction/IDigitalService.h"
#include "Stm32HalConf.h"
#include "Stm32HalGpio.h"

#ifndef STM32HALDIGITALSERVICE_H
#define STM32HALDIGITALSERVICE_H

using namespace HardwareAbstraction;

namespace Stm32
{
	class Stm32HalDigitalService : public HardwareAbstraction::IDigitalService
	{
	public:
		void InitPin(uint16_t pin, HardwareAbstraction::PinDirection direction);
		bool ReadPin(uint16_t pin);
		void WritePin(uint16_t pin, bool value);
		void ScheduleRecurringInterrupt(uint16_t pin, ICallBack *);
		void ScheduleNextInterrupt(uint16_t pin, ICallBack *);
	};
}

#endif