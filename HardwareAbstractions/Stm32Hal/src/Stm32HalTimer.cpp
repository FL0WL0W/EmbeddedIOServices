#include "Stm32HalTimer.h"
#include "Stm32HalTimerService.h"
#include "Stm32HalPwmService.h"

namespace Stm32
{
#ifdef TIM1
	bool TIM1_Freq_Locked;
    bool TIM1_Input;
#endif
#ifdef TIM2
	bool TIM2_Freq_Locked;
    bool TIM2_Input;
#endif
#ifdef TIM3
	bool TIM3_Freq_Locked;
    bool TIM3_Input;
#endif
#ifdef TIM4
	bool TIM4_Freq_Locked;
    bool TIM4_Input;
#endif
#ifdef TIM5
	bool TIM5_Freq_Locked;
    bool TIM5_Input;
#endif
#ifdef TIM6
	bool TIM6_Freq_Locked;
    bool TIM6_Input;
#endif
#ifdef TIM7
	bool TIM7_Freq_Locked;
    bool TIM7_Input;
#endif
#ifdef TIM8
	bool TIM8_Freq_Locked;
    bool TIM8_Input;
#endif
#ifdef TIM9
	bool TIM9_Freq_Locked;
    bool TIM9_Input;
#endif
#ifdef TIM10
	bool TIM10_Freq_Locked;
    bool TIM10_Input;
#endif
#ifdef TIM11
	bool TIM11_Freq_Locked;
    bool TIM11_Input;
#endif
#ifdef TIM12
	bool TIM12_Freq_Locked;
    bool TIM12_Input;
#endif
#ifdef TIM13
	bool TIM13_Freq_Locked;
    bool TIM13_Input;
#endif
#ifdef TIM14
	bool TIM14_Freq_Locked;
    bool TIM14_Input;
#endif

    void EnableTimerClock(uint8_t timNum)
    {
		switch (timNum)
		{
#ifdef TIM1
		case 1:
			__HAL_RCC_TIM1_CLK_ENABLE();
			break;
#endif
#ifdef TIM2
		case 2:
			__HAL_RCC_TIM2_CLK_ENABLE();
			break;
#endif
#ifdef TIM3
		case 3:
			__HAL_RCC_TIM3_CLK_ENABLE();
			break;
#endif
#ifdef TIM4
		case 4:
			__HAL_RCC_TIM4_CLK_ENABLE();
			break;
#endif
#ifdef TIM5
		case 5:
			__HAL_RCC_TIM5_CLK_ENABLE();
			break;
#endif
#ifdef TIM6
		case 6:
			__HAL_RCC_TIM6_CLK_ENABLE();
			break;
#endif
#ifdef TIM7
		case 7:
			__HAL_RCC_TIM7_CLK_ENABLE();
			break;
#endif
#ifdef TIM8
		case 8:
			__HAL_RCC_TIM8_CLK_ENABLE();
			break;
#endif
#ifdef TIM9
		case 9:
			__HAL_RCC_TIM9_CLK_ENABLE();
			break;
#endif
#ifdef TIM10
		case 10:
			__HAL_RCC_TIM10_CLK_ENABLE();
			break;
#endif
#ifdef TIM11
		case 11:
			__HAL_RCC_TIM11_CLK_ENABLE();
			break;
#endif
#ifdef TIM12
		case 12:
			__HAL_RCC_TIM12_CLK_ENABLE();
			break;
#endif
#ifdef TIM13
		case 13:
			__HAL_RCC_TIM13_CLK_ENABLE();
			break;
#endif
#ifdef TIM14
		case 14:
			__HAL_RCC_TIM14_CLK_ENABLE();
			break;
#endif
		}
    }

    void EnableTimerInterrupts(uint8_t timNum)
	{
		switch (timNum)
		{
#ifdef TIM1
		case 1:
#ifdef TIM1_SEPERATE_UP_CC_IT
			HAL_NVIC_SetPriority(TIM1_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
#else
			HAL_NVIC_SetPriority(TIM1_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM1_IRQn);
#endif
			break;
#endif
#ifdef TIM2
		case 2:
#ifdef TIM2_SEPERATE_UP_CC_IT
			HAL_NVIC_SetPriority(TIM2_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM2_CC_IRQn);
#else
			HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM2_IRQn);
#endif
			break;
#endif
#ifdef TIM3
		case 3:
#ifdef TIM3_SEPERATE_UP_CC_IT
			HAL_NVIC_SetPriority(TIM3_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM3_CC_IRQn);
#else
			HAL_NVIC_SetPriority(TIM3_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM3_IRQn);
#endif
			break;
#endif
#ifdef TIM4
		case 4:
#ifdef TIM4_SEPERATE_UP_CC_IT
			HAL_NVIC_SetPriority(TIM4_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM4_CC_IRQn);
#else
			HAL_NVIC_SetPriority(TIM4_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM4_IRQn);
#endif
			break;
#endif
#ifdef TIM5
		case 5:
#ifdef TIM5_SEPERATE_UP_CC_IT
			HAL_NVIC_SetPriority(TIM5_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM5_CC_IRQn);
#else
			HAL_NVIC_SetPriority(TIM5_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM5_IRQn);
#endif
			break;
#endif
#ifdef TIM6
		case 6:
#ifdef TIM6_SEPERATE_UP_CC_IT
			HAL_NVIC_SetPriority(TIM6_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM6_CC_IRQn);
#else
			HAL_NVIC_SetPriority(TIM6_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM6_IRQn);
#endif
			break;
#endif
#ifdef TIM7
		case 7:
#ifdef TIM7_SEPERATE_UP_CC_IT
			HAL_NVIC_SetPriority(TIM7_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM7_CC_IRQn);
#else
			HAL_NVIC_SetPriority(TIM7_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM7_IRQn);
#endif
			break;
#endif
#ifdef TIM8
		case 8:
#ifdef TIM8_SEPERATE_UP_CC_IT
			HAL_NVIC_SetPriority(TIM8_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM8_CC_IRQn);
#else
			HAL_NVIC_SetPriority(TIM8_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM8_IRQn);
#endif
			break;
#endif
#ifdef TIM9
		case 9:
#ifdef TIM9_SEPERATE_UP_CC_IT
			HAL_NVIC_SetPriority(TIM9_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM9_CC_IRQn);
#else
			HAL_NVIC_SetPriority(TIM9_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM9_IRQn);
#endif
			break;
#endif
#ifdef TIM10
		case 10:
#ifdef TIM10_SEPERATE_UP_CC_IT
			HAL_NVIC_SetPriority(TIM10_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM10_CC_IRQn);
#else
			HAL_NVIC_SetPriority(TIM10_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM10_IRQn);
#endif
			break;
#endif
#ifdef TIM11
		case 11:
#ifdef TIM11_SEPERATE_UP_CC_IT
			HAL_NVIC_SetPriority(TIM11_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM11_CC_IRQn);
#else
			HAL_NVIC_SetPriority(TIM11_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM11_IRQn);
#endif
			break;
#endif
#ifdef TIM12
		case 12:
#ifdef TIM12_SEPERATE_UP_CC_IT
			HAL_NVIC_SetPriority(TIM12_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM12_CC_IRQn);
#else
			HAL_NVIC_SetPriority(TIM12_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM12_IRQn);
#endif
			break;
#endif
#ifdef TIM13
		case 13:
#ifdef TIM13_SEPERATE_UP_CC_IT
			HAL_NVIC_SetPriority(TIM13_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM13_CC_IRQn);
#else
			HAL_NVIC_SetPriority(TIM13_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM13_IRQn);
#endif
			break;
#endif
#ifdef TIM14
		case 14:
#ifdef TIM14_SEPERATE_UP_CC_IT
			HAL_NVIC_SetPriority(TIM14_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM14_CC_IRQn);
#else
			HAL_NVIC_SetPriority(TIM14_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM14_IRQn);
#endif
			break;
#endif
		}
	}

	extern "C" 
	{
#if defined(TIM1)
		void TIM1_IRQHandler(void)
		{
			if (TimerService1 != 0)
				TimerService1->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim1();
		}
#endif
#if defined(TIM1)
		void TIM1_CC_IRQHandler(void)
		{
			if (TimerService1 != 0)
				TimerService1->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim1();
		}
#endif
#if defined(TIM2)
		void TIM2_IRQHandler(void)
		{
			if (TimerService2 != 0)
				TimerService2->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim2();
		}
#endif
#if defined(TIM2)
		void TIM2_CC_IRQHandler(void)
		{
			if (TimerService2 != 0)
				TimerService2->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim2();
		}
#endif
#if defined(TIM3)
		void TIM3_IRQHandler(void)
		{
			if (TimerService3 != 0)
				TimerService3->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim3();
		}
#endif
#if defined(TIM3)
		void TIM3_CC_IRQHandler(void)
		{
			if (TimerService3 != 0)
				TimerService3->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim3();
		}
#endif
#if defined(TIM4)
		void TIM4_IRQHandler(void)
		{
			if (TimerService4 != 0)
				TimerService4->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim4();
		}
#endif
#if defined(TIM4)
		void TIM4_CC_IRQHandler(void)
		{
			if (TimerService4 != 0)
				TimerService4->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim4();
		}
#endif
#if defined(TIM5)
		void TIM5_IRQHandler(void)
		{
			if (TimerService5 != 0)
				TimerService5->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim5();
		}
#endif
#if defined(TIM5)
		void TIM5_CC_IRQHandler(void)
		{
			if (TimerService5 != 0)
				TimerService5->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim5();
		}
#endif
#if defined(TIM6)
		void TIM6_IRQHandler(void)
		{
			if (TimerService6 != 0)
				TimerService6->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim6();
		}
#endif
#if defined(TIM6)
		void TIM6_CC_IRQHandler(void)
		{
			if (TimerService6 != 0)
				TimerService6->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim6();
		}
#endif
#if defined(TIM7)
		void TIM7_IRQHandler(void)
		{
			if (TimerService7 != 0)
				TimerService7->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim7();
		}
#endif
#if defined(TIM7)
		void TIM7_CC_IRQHandler(void)
		{
			if (TimerService7 != 0)
				TimerService7->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim7();
		}
#endif
#if defined(TIM8)
		void TIM8_IRQHandler(void)
		{
			if (TimerService8 != 0)
				TimerService8->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim8();
		}
#endif
#if defined(TIM8)
		void TIM8_CC_IRQHandler(void)
		{
			if (TimerService8 != 0)
				TimerService8->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim8();
		}
#endif
#if defined(TIM9)
		void TIM9_IRQHandler(void)
		{
			if (TimerService9 != 0)
				TimerService9->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim9();
		}
#endif
#if defined(TIM9)
		void TIM9_CC_IRQHandler(void)
		{
			if (TimerService9 != 0)
				TimerService9->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim9();
		}
#endif
#if defined(TIM10)
		void TIM10_IRQHandler(void)
		{
			if (TimerService10 != 0)
				TimerService10->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim10();
		}
#endif
#if defined(TIM10)
		void TIM10_CC_IRQHandler(void)
		{
			if (TimerService10 != 0)
				TimerService10->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim10();
		}
#endif
#if defined(TIM11)
		void TIM11_IRQHandler(void)
		{
			if (TimerService11 != 0)
				TimerService11->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim11();
		}
#endif
#if defined(TIM11)
		void TIM11_CC_IRQHandler(void)
		{
			if (TimerService11 != 0)
				TimerService11->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim11();
		}
#endif
#if defined(TIM12)
		void TIM12_IRQHandler(void)
		{
			if (TimerService12 != 0)
				TimerService12->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim12();
		}
#endif
#if defined(TIM12)
		void TIM12_CC_IRQHandler(void)
		{
			if (TimerService12 != 0)
				TimerService12->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim12();
		}
#endif
#if defined(TIM13)
		void TIM13_IRQHandler(void)
		{
			if (TimerService13 != 0)
				TimerService13->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim13();
		}
#endif
#if defined(TIM13)
		void TIM13_CC_IRQHandler(void)
		{
			if (TimerService13 != 0)
				TimerService13->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim13();
		}
#endif
#if defined(TIM14)
		void TIM14_IRQHandler(void)
		{
			if (TimerService14 != 0)
				TimerService14->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim14();
		}
#endif
#if defined(TIM14)
		void TIM14_CC_IRQHandler(void)
		{
			if (TimerService14 != 0)
				TimerService14->Interrupt();
			if (HalPwmService != 0)
				HalPwmService->InterruptTim14();
		}
#endif
	}
}