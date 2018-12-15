#include "HardwareAbstraction/PinDirection.h"
#include "HardwareAbstraction/IDigitalService.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"

#ifndef STM32HALDIGITALSERVICE_H
#define STM32HALDIGITALSERVICE_H

using namespace HardwareAbstraction;

namespace Stm32
{
	class Stm32HalDigitalService : public HardwareAbstraction::IDigitalService
	{
	public:
		void InitPin(unsigned short pin, HardwareAbstraction::PinDirection direction);
		bool ReadPin(unsigned short pin);
		void WritePin(unsigned short pin, bool value);
		void ScheduleRecurringInterrupt(unsigned short pin, ICallBack *);
		void ScheduleNextInterrupt(unsigned short pin, ICallBack *);
	};
}

#endif