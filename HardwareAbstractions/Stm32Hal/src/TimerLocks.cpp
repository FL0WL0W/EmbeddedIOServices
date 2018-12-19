#include "TimerLocks.h"
#include "Stm32HalTimerService.h"
#include "Stm32HalPwmService.h"

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

namespace Stm32
{
	extern "C" 
	{
#if defined(TIM1)
		void TIM1_UP_IRQHandler(void)
		{
			if (TimerService1 != 0)
				TimerService1->Interrupt();
		}
#endif
#if defined(TIM1)
		void TIM1_IRQHandler(void)
		{
			if (TimerService1 != 0)
				TimerService1->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim1();
		}
#endif
#if defined(TIM1)
		void TIM1_CC_IRQHandler(void)
		{
			if (TimerService1 != 0)
				TimerService1->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim1();
		}
#endif
#if defined(TIM2)
		void TIM2_UP_IRQHandler(void)
		{
			if (TimerService2 != 0)
				TimerService2->Interrupt();
		}
#endif
#if defined(TIM2)
		void TIM2_IRQHandler(void)
		{
			if (TimerService2 != 0)
				TimerService2->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim2();
		}
#endif
#if defined(TIM2)
		void TIM2_CC_IRQHandler(void)
		{
			if (TimerService2 != 0)
				TimerService2->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim2();
		}
#endif
#if defined(TIM3)
		void TIM3_UP_IRQHandler(void)
		{
			if (TimerService3 != 0)
				TimerService3->Interrupt();
		}
#endif
#if defined(TIM3)
		void TIM3_IRQHandler(void)
		{
			if (TimerService3 != 0)
				TimerService3->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim3();
		}
#endif
#if defined(TIM3)
		void TIM3_CC_IRQHandler(void)
		{
			if (TimerService3 != 0)
				TimerService3->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim3();
		}
#endif
#if defined(TIM4)
		void TIM4_UP_IRQHandler(void)
		{
			if (TimerService4 != 0)
				TimerService4->Interrupt();
		}
#endif
#if defined(TIM4)
		void TIM4_IRQHandler(void)
		{
			if (TimerService4 != 0)
				TimerService4->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim4();
		}
#endif
#if defined(TIM4)
		void TIM4_CC_IRQHandler(void)
		{
			if (TimerService4 != 0)
				TimerService4->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim4();
		}
#endif
#if defined(TIM5)
		void TIM5_UP_IRQHandler(void)
		{
			if (TimerService5 != 0)
				TimerService5->Interrupt();
		}
#endif
#if defined(TIM5)
		void TIM5_IRQHandler(void)
		{
			if (TimerService5 != 0)
				TimerService5->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim5();
		}
#endif
#if defined(TIM5)
		void TIM5_CC_IRQHandler(void)
		{
			if (TimerService5 != 0)
				TimerService5->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim5();
		}
#endif
#if defined(TIM6)
		void TIM6_UP_IRQHandler(void)
		{
			if (TimerService6 != 0)
				TimerService6->Interrupt();
		}
#endif
#if defined(TIM6)
		void TIM6_IRQHandler(void)
		{
			if (TimerService6 != 0)
				TimerService6->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim6();
		}
#endif
#if defined(TIM6)
		void TIM6_CC_IRQHandler(void)
		{
			if (TimerService6 != 0)
				TimerService6->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim6();
		}
#endif
#if defined(TIM7)
		void TIM7_UP_IRQHandler(void)
		{
			if (TimerService7 != 0)
				TimerService7->Interrupt();
		}
#endif
#if defined(TIM7)
		void TIM7_IRQHandler(void)
		{
			if (TimerService7 != 0)
				TimerService7->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim7();
		}
#endif
#if defined(TIM7)
		void TIM7_CC_IRQHandler(void)
		{
			if (TimerService7 != 0)
				TimerService7->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim7();
		}
#endif
#if defined(TIM8)
		void TIM8_UP_IRQHandler(void)
		{
			if (TimerService8 != 0)
				TimerService8->Interrupt();
		}
#endif
#if defined(TIM8)
		void TIM8_IRQHandler(void)
		{
			if (TimerService8 != 0)
				TimerService8->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim8();
		}
#endif
#if defined(TIM8)
		void TIM8_CC_IRQHandler(void)
		{
			if (TimerService8 != 0)
				TimerService8->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim8();
		}
#endif
#if defined(TIM9)
		void TIM9_UP_IRQHandler(void)
		{
			if (TimerService9 != 0)
				TimerService9->Interrupt();
		}
#endif
#if defined(TIM9)
		void TIM9_IRQHandler(void)
		{
			if (TimerService9 != 0)
				TimerService9->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim9();
		}
#endif
#if defined(TIM9)
		void TIM9_CC_IRQHandler(void)
		{
			if (TimerService9 != 0)
				TimerService9->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim9();
		}
#endif
#if defined(TIM10)
		void TIM10_UP_IRQHandler(void)
		{
			if (TimerService10 != 0)
				TimerService10->Interrupt();
		}
#endif
#if defined(TIM10)
		void TIM10_IRQHandler(void)
		{
			if (TimerService10 != 0)
				TimerService10->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim10();
		}
#endif
#if defined(TIM10)
		void TIM10_CC_IRQHandler(void)
		{
			if (TimerService10 != 0)
				TimerService10->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim10();
		}
#endif
#if defined(TIM11)
		void TIM11_UP_IRQHandler(void)
		{
			if (TimerService11 != 0)
				TimerService11->Interrupt();
		}
#endif
#if defined(TIM11)
		void TIM11_IRQHandler(void)
		{
			if (TimerService11 != 0)
				TimerService11->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim11();
		}
#endif
#if defined(TIM11)
		void TIM11_CC_IRQHandler(void)
		{
			if (TimerService11 != 0)
				TimerService11->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim11();
		}
#endif
#if defined(TIM12)
		void TIM12_UP_IRQHandler(void)
		{
			if (TimerService12 != 0)
				TimerService12->Interrupt();
		}
#endif
#if defined(TIM12)
		void TIM12_IRQHandler(void)
		{
			if (TimerService12 != 0)
				TimerService12->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim12();
		}
#endif
#if defined(TIM12)
		void TIM12_CC_IRQHandler(void)
		{
			if (TimerService12 != 0)
				TimerService12->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim12();
		}
#endif
#if defined(TIM13)
		void TIM13_UP_IRQHandler(void)
		{
			if (TimerService13 != 0)
				TimerService13->Interrupt();
		}
#endif
#if defined(TIM13)
		void TIM13_IRQHandler(void)
		{
			if (TimerService13 != 0)
				TimerService13->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim13();
		}
#endif
#if defined(TIM13)
		void TIM13_CC_IRQHandler(void)
		{
			if (TimerService13 != 0)
				TimerService13->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim13();
		}
#endif
#if defined(TIM14)
		void TIM14_UP_IRQHandler(void)
		{
			if (TimerService14 != 0)
				TimerService14->Interrupt();
		}
#endif
#if defined(TIM14)
		void TIM14_IRQHandler(void)
		{
			if (TimerService14 != 0)
				TimerService14->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim14();
		}
#endif
#if defined(TIM14)
		void TIM14_CC_IRQHandler(void)
		{
			if (TimerService14 != 0)
				TimerService14->Interrupt();
			if (PwmService != 0)
				PwmService->InterruptTim14();
		}
#endif
	}
}