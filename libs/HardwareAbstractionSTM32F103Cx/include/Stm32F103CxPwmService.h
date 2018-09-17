#include "Stm32F103CxDefines.h"
#include "HardwareAbstraction/PinDirection.h"
#include "HardwareAbstraction/IPwmService.h"

namespace Stm32
{
	class Stm32F103CxPwmService : public HardwareAbstraction::IPwmService
	{
	protected:
		unsigned short _prevCC[4][4];
		unsigned short _currCC[4][4];
	public:
		Stm32F103CxPwmService();
		void InitPin(unsigned char pin, HardwareAbstraction::PinDirection direction, unsigned short minFrequency);
		HardwareAbstraction::PwmValue ReadPin(unsigned char pin);
		void WritePin(unsigned char pin, HardwareAbstraction::PwmValue value);
		void InterruptTim1(void);
		void InterruptTim2(void);
		void InterruptTim3(void);
		void InterruptTim4(void);
	};

	extern Stm32F103CxPwmService *PwmService;
}