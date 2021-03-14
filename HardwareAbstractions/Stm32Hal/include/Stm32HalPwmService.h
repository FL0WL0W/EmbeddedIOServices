#include "PinDirection.h"
#include "IPwmService.h"
#include "Stm32HalGpio.h"
#include "stdint.h"

#ifndef STM32HALPWMSERVICE_H
#define STM32HALPWMSERVICE_H

namespace Stm32
{
	class Stm32HalPwmService : public EmbeddedIOServices::IPwmService
	{
	protected:
#if defined(TIM14)
		uint16_t _prevCC[14][4];
		uint16_t _currCC[14][4];
#elif defined(TIM13)
		uint16_t _prevCC[13][4];
		uint16_t _currCC[13][4];
#elif defined(TIM12)
		uint16_t _prevCC[12][4];
		uint16_t _currCC[12][4];
#elif defined(TIM11)
		uint16_t _prevCC[11][4];
		uint16_t _currCC[11][4];
#elif defined(TIM10)
		uint16_t _prevCC[10][4];
		uint16_t _currCC[10][4];
#elif defined(TIM9)
		uint16_t _prevCC[9][4];
		uint16_t _currCC[9][4];
#elif defined(TIM8)
		uint16_t _prevCC[8][4];
		uint16_t _currCC[8][4];
#elif defined(TIM7)
		uint16_t _prevCC[7][4];
		uint16_t _currCC[7][4];
#elif defined(TIM6)
		uint16_t _prevCC[6][4];
		uint16_t _currCC[6][4];
#elif defined(TIM5)
		uint16_t _prevCC[5][4];
		uint16_t _currCC[5][4];
#elif defined(TIM4)
		uint16_t _prevCC[4][4];
		uint16_t _currCC[4][4];
#elif defined(TIM3)
		uint16_t _prevCC[3][4];
		uint16_t _currCC[3][4];
#elif defined(TIM2)
		uint16_t _prevCC[2][4];
		uint16_t _currCC[2][4];
#elif defined(TIM1)
		uint16_t _prevCC[1][4];
		uint16_t _currCC[1][4];
#endif
	public:
		Stm32HalPwmService();
		void InitPin(uint16_t pin, EmbeddedIOServices::PinDirection direction, uint16_t minFrequency);
		EmbeddedIOServices::PwmValue ReadPin(uint16_t pin);
		void WritePin(uint16_t pin, EmbeddedIOServices::PwmValue value);
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

	extern Stm32HalPwmService *HalPwmService;
}

#endif