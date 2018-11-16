#include "Stm32F103CxDefines.h"
#include "HardwareAbstraction/PinDirection.h"
#include "HardwareAbstraction/IDigitalService.h"

using namespace HardwareAbstraction;

namespace Stm32
{
	class Stm32F103CxDigitalService : public HardwareAbstraction::IDigitalService
	{
	public:
		void InitPin(uint8_t pin, HardwareAbstraction::PinDirection direction);
		bool ReadPin(uint8_t pin);
		void WritePin(uint8_t pin, bool value);
		void ScheduleRecurringInterrupt(unsigned char pin, ICallBack *);
		void ScheduleNextInterrupt(unsigned char pin, ICallBack *);
	};
}