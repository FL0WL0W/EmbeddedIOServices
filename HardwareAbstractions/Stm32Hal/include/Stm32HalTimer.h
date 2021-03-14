#include "Stm32HalConf.h"
#include "stdint.h"

#ifndef STM32HALTIMER_H
#define STM32HALTIMER_H

namespace Stm32
{
    void EnableTimerClock(uint8_t timNum);
    void EnableTimerInterrupts(uint8_t timNum);

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