#include "PinDirection.h"
#include "IDigitalService.h"
#include "Stm32duinoConf.h"
#include "stdint.h"

#ifndef STM32DUINDIGITALSERVICE_H
#define STM32DUINDIGITALSERVICE_H

namespace Stm32
{
	class Stm32duinoDigitalService : public EmbeddedIOServices::IDigitalService
	{
	public:
		void InitPin(uint16_t pin, EmbeddedIOServices::PinDirection direction);
		bool ReadPin(uint16_t pin);
		void WritePin(uint16_t pin, bool value);
		void ScheduleRecurringInterrupt(uint16_t pin, std::function<void()> *);
		void ScheduleNextInterrupt(uint16_t pin, std::function<void()> *);
	};
}

#endif
