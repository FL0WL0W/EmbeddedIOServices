#include "Stm32HalConf.h"
#include "stdint.h"
#include <functional>

#ifndef STM32HALTIMER_H
#define STM32HALTIMER_H

namespace Stm32
{		
	enum TimerIndex : uint8_t
	{
#ifdef TIM1_BASE	
		Index1,
#endif
#ifdef TIM2_BASE	
		Index2,
#endif
#ifdef TIM3_BASE	
		Index3,
#endif
#ifdef TIM4_BASE	
		Index4,
#endif
#ifdef TIM5_BASE	
		Index5,
#endif
#ifdef TIM6_BASE	
		Index6,
#endif
#ifdef TIM7_BASE	
		Index7,
#endif
#ifdef TIM8_BASE	
		Index8,
#endif
#ifdef TIM9_BASE	
		Index9,
#endif
#ifdef TIM10_BASE	
		Index10,
#endif
#ifdef TIM11_BASE	
		Index11,
#endif
#ifdef TIM12_BASE	
		Index12,
#endif
#ifdef TIM13_BASE	
		Index13,
#endif
#ifdef TIM14_BASE	
		Index14,
#endif
#ifdef TIM15_BASE	
		Index15,
#endif
#ifdef TIM16_BASE	
		Index16,
#endif
#ifdef TIM17_BASE	
		Index17,
#endif
#ifdef TIM18_BASE	
		Index18,
#endif
#ifdef TIM19_BASE	
		Index19,
#endif
#ifdef TIM20_BASE	
		Index20,
#endif
#ifdef TIM21_BASE	
		Index21,
#endif
#ifdef TIM22_BASE	
		Index22,
#endif
		Num
	};

	extern bool _timFrequencyLocked[TimerIndex::Num];
	extern std::function<void()> _timCallBack[TimerIndex::Num];
	extern "C" uint16_t _timPreviousCC[TimerIndex::Num][4];
	extern "C" uint16_t _timCurrentCC[TimerIndex::Num][4];

	TIM_HandleTypeDef TimInit(TimerIndex index, uint32_t prescaler, uint32_t period);
	TIM_TypeDef* TimIndexToTIM(TimerIndex index);
	void EnableTimerClock(TimerIndex index);
	void EnableTimerInterrupts(TimerIndex index);
}

#endif