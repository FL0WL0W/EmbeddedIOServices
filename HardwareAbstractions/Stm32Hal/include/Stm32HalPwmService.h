#include "HardwareAbstraction/PinDirection.h"
#include "HardwareAbstraction/IPwmService.h"

using namespace HardwareAbstraction;

namespace Stm32
{
	class Stm32HalPwmService : public HardwareAbstraction::IPwmService
	{
	protected:
#if defined(TIM14)
		unsigned short _prevCC[14][4];
		unsigned short _currCC[14][4];
#elif defined(TIM13)
		unsigned short _prevCC[13][4];
		unsigned short _currCC[13][4];
#elif defined(TIM12)
		unsigned short _prevCC[12][4];
		unsigned short _currCC[12][4];
#elif defined(TIM11)
		unsigned short _prevCC[11][4];
		unsigned short _currCC[11][4];
#elif defined(TIM10)
		unsigned short _prevCC[10][4];
		unsigned short _currCC[10][4];
#elif defined(TIM9)
		unsigned short _prevCC[9][4];
		unsigned short _currCC[9][4];
#elif defined(TIM8)
		unsigned short _prevCC[8][4];
		unsigned short _currCC[8][4];
#elif defined(TIM7)
		unsigned short _prevCC[7][4];
		unsigned short _currCC[7][4];
#elif defined(TIM6)
		unsigned short _prevCC[6][4];
		unsigned short _currCC[6][4];
#elif defined(TIM5)
		unsigned short _prevCC[5][4];
		unsigned short _currCC[5][4];
#elif defined(TIM4)
		unsigned short _prevCC[4][4];
		unsigned short _currCC[4][4];
#elif defined(TIM3)
		unsigned short _prevCC[3][4];
		unsigned short _currCC[3][4];
#elif defined(TIM2)
		unsigned short _prevCC[2][4];
		unsigned short _currCC[2][4];
#elif defined(TIM1)
		unsigned short _prevCC[1][4];
		unsigned short _currCC[1][4];
#endif
	public:
		Stm32HalPwmService();
		void InitPin(unsigned short pin, PinDirection direction, unsigned short minFrequency);
		PwmValue ReadPin(unsigned short pin);
		void WritePin(unsigned short pin, PwmValue value);
#ifdef TIM1
		void InterruptTim1(void);
#endif
#ifdef TIM2
		void InterruptTim2(void);
#endif
#ifdef TIM3
		void InterruptTim3(void);
#endif
#ifdef TIM4
		void InterruptTim4(void);
#endif
#ifdef TIM5
		void InterruptTim5(void);
#endif
#ifdef TIM6
		void InterruptTim6(void);
#endif
#ifdef TIM7
		void InterruptTim7(void);
#endif
#ifdef TIM8
		void InterruptTim8(void);
#endif
#ifdef TIM9
		void InterruptTim9(void);
#endif
#ifdef TIM10
		void InterruptTim10(void);
#endif
#ifdef TIM11
		void InterruptTim11(void);
#endif
#ifdef TIM12
		void InterruptTim12(void);
#endif
#ifdef TIM13
		void InterruptTim13(void);
#endif
#ifdef TIM14
		void InterruptTim14(void);
#endif
	};

	extern Stm32HalPwmService *PwmService;
}