#include "Stm32HalConf.h"
#include "stdint.h"

#ifndef STM32HALTIMER_H
#define STM32HALTIMER_H

namespace Stm32
{
    void EnableTimerClock(uint8_t timNum);
    void EnableTimerInterrupts(uint8_t timNum);
#if defined(TIM14)
	extern "C" uint16_t _prevCC[14][4];
	extern "C" uint16_t _currCC[14][4];
#elif defined(TIM13)
	extern "C" uint16_t _prevCC[13][4];
	extern "C" uint16_t _currCC[13][4];
#elif defined(TIM12)
	extern "C" uint16_t _prevCC[12][4];
	extern "C" uint16_t _currCC[12][4];
#elif defined(TIM11)
	extern "C" uint16_t _prevCC[11][4];
	extern "C" uint16_t _currCC[11][4];
#elif defined(TIM10)
	extern "C" uint16_t _prevCC[10][4];
	extern "C" uint16_t _currCC[10][4];
#elif defined(TIM9)
	extern "C" uint16_t _prevCC[9][4];
	extern "C" uint16_t _currCC[9][4];
#elif defined(TIM8)
	extern "C" uint16_t _prevCC[8][4];
	extern "C" uint16_t _currCC[8][4];
#elif defined(TIM7)
	extern "C" uint16_t _prevCC[7][4];
	extern "C" uint16_t _currCC[7][4];
#elif defined(TIM6)
	extern "C" uint16_t _prevCC[6][4];
	extern "C" uint16_t _currCC[6][4];
#elif defined(TIM5)
	extern "C" uint16_t _prevCC[5][4];
	extern "C" uint16_t _currCC[5][4];
#elif defined(TIM4)
	extern "C" uint16_t _prevCC[4][4];
	extern "C" uint16_t _currCC[4][4];
#elif defined(TIM3)
	extern "C" uint16_t _prevCC[3][4];
	extern "C" uint16_t _currCC[3][4];
#elif defined(TIM2)
	extern "C" uint16_t _prevCC[2][4];
	extern "C" uint16_t _currCC[2][4];
#elif defined(TIM1)
	extern "C" uint16_t _prevCC[1][4];
	extern "C" uint16_t _currCC[1][4];
#endif
#ifdef TIM1
	extern bool TIM1_Freq_Locked;
    extern bool TIM1_Input;
#endif
#ifdef TIM2
	extern bool TIM2_Freq_Locked;
    extern bool TIM2_Input;
#endif
#ifdef TIM3
	extern bool TIM3_Freq_Locked;
    extern bool TIM3_Input;
#endif
#ifdef TIM4
	extern bool TIM4_Freq_Locked;
    extern bool TIM4_Input;
#endif
#ifdef TIM5
	extern bool TIM5_Freq_Locked;
    extern bool TIM5_Input;
#endif
#ifdef TIM6
	extern bool TIM6_Freq_Locked;
    extern bool TIM6_Input;
#endif
#ifdef TIM7
	extern bool TIM7_Freq_Locked;
    extern bool TIM7_Input;
#endif
#ifdef TIM8
	extern bool TIM8_Freq_Locked;
    extern bool TIM8_Input;
#endif
#ifdef TIM9
	extern bool TIM9_Freq_Locked;
    extern bool TIM9_Input;
#endif
#ifdef TIM10
	extern bool TIM10_Freq_Locked;
    extern bool TIM10_Input;
#endif
#ifdef TIM11
	extern bool TIM11_Freq_Locked;
    extern bool TIM11_Input;
#endif
#ifdef TIM12
	extern bool TIM12_Freq_Locked;
    extern bool TIM12_Input;
#endif
#ifdef TIM13
	extern bool TIM13_Freq_Locked;
    extern bool TIM13_Input;
#endif
#ifdef TIM14
	extern bool TIM14_Freq_Locked;
    extern bool TIM14_Input;
#endif
}

#endif