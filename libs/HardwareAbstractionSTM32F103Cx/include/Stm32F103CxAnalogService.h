#include "Stm32F103CxDefines.h"
#include "HardwareAbstraction/IAnalogService.h"

namespace Stm32
{
	class Stm32F103CxAnalogService : public HardwareAbstraction::IAnalogService
	{
	public:
		Stm32F103CxAnalogService();
		void InitPin(unsigned char pin);
		float ReadPin(unsigned char pin);
	};
}