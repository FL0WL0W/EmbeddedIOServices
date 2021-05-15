#include "Stm32HalTimer.h"
#include "Stm32HalTimerService.h"

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
#if defined(TIM1)
		void TIM1_IRQHandler(void)
		{
			if (timerCallBack1 != 0 && TIM1->SR & TIM_IT_CC1)
				timerCallBack1->Execute();;
			if (TIM1->SR & TIM_IT_CC1)
			{
				_prevCC[0][0] = _currCC[0][0];
				_currCC[0][0] = TIM1->CCR1;
				TIM1->SR = ~TIM_IT_CC1;
			}
			if (TIM1->SR & TIM_IT_CC2)
			{
				_prevCC[0][1] = _currCC[0][1];
				_currCC[0][1] = TIM1->CCR2;
				TIM1->SR = ~TIM_IT_CC2;
			}
			if (TIM1->SR & TIM_IT_CC3)
			{
				_prevCC[0][2] = _currCC[0][2];
				_currCC[0][2] = TIM1->CCR3;
				TIM1->SR = ~TIM_IT_CC3;
			}
			if (TIM1->SR & TIM_IT_CC4)
			{
				_prevCC[0][3] = _currCC[0][3];
				_currCC[0][3] = TIM1->CCR4;
				TIM1->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM1)
		void TIM1_CC_IRQHandler(void)
		{
			if (timerCallBack1 != 0 && TIM1->SR & TIM_IT_CC1)
				timerCallBack1->Execute();;
			if (TIM1->SR & TIM_IT_CC1)
			{
				_prevCC[0][0] = _currCC[0][0];
				_currCC[0][0] = TIM1->CCR1;
				TIM1->SR = ~TIM_IT_CC1;
			}
			if (TIM1->SR & TIM_IT_CC2)
			{
				_prevCC[0][1] = _currCC[0][1];
				_currCC[0][1] = TIM1->CCR2;
				TIM1->SR = ~TIM_IT_CC2;
			}
			if (TIM1->SR & TIM_IT_CC3)
			{
				_prevCC[0][2] = _currCC[0][2];
				_currCC[0][2] = TIM1->CCR3;
				TIM1->SR = ~TIM_IT_CC3;
			}
			if (TIM1->SR & TIM_IT_CC4)
			{
				_prevCC[0][3] = _currCC[0][3];
				_currCC[0][3] = TIM1->CCR4;
				TIM1->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM2)
		void TIM2_IRQHandler(void)
		{
			if (timerCallBack2 != 0 && TIM2->SR & TIM_IT_CC1)
				timerCallBack2->Execute();
			if (TIM2->SR & TIM_IT_CC1)
			{
				_prevCC[1][0] = _currCC[1][0];
				_currCC[1][0] = TIM2->CCR1;
				TIM2->SR = ~TIM_IT_CC1;
			}
			if (TIM2->SR & TIM_IT_CC2)
			{
				_prevCC[1][1] = _currCC[1][1];
				_currCC[1][1] = TIM2->CCR2;
				TIM2->SR = ~TIM_IT_CC2;
			}
			if (TIM2->SR & TIM_IT_CC3)
			{
				_prevCC[1][2] = _currCC[1][2];
				_currCC[1][2] = TIM2->CCR3;
				TIM2->SR = ~TIM_IT_CC3;
			}
			if (TIM2->SR & TIM_IT_CC4)
			{
				_prevCC[1][3] = _currCC[1][3];
				_currCC[1][3] = TIM2->CCR4;
				TIM2->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM2)
		void TIM2_CC_IRQHandler(void)
		{
			if (timerCallBack2 != 0 && TIM2->SR & TIM_IT_CC1)
				timerCallBack2->Execute();
			if (TIM2->SR & TIM_IT_CC1)
			{
				_prevCC[1][0] = _currCC[1][0];
				_currCC[1][0] = TIM2->CCR1;
				TIM2->SR = ~TIM_IT_CC1;
			}
			if (TIM2->SR & TIM_IT_CC2)
			{
				_prevCC[1][1] = _currCC[1][1];
				_currCC[1][1] = TIM2->CCR2;
				TIM2->SR = ~TIM_IT_CC2;
			}
			if (TIM2->SR & TIM_IT_CC3)
			{
				_prevCC[1][2] = _currCC[1][2];
				_currCC[1][2] = TIM2->CCR3;
				TIM2->SR = ~TIM_IT_CC3;
			}
			if (TIM2->SR & TIM_IT_CC4)
			{
				_prevCC[1][3] = _currCC[1][3];
				_currCC[1][3] = TIM2->CCR4;
				TIM2->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM3)
		void TIM3_IRQHandler(void)
		{
			if (timerCallBack3 != 0 && TIM3->SR & TIM_IT_CC1)
				timerCallBack3->Execute();
			if (TIM3->SR & TIM_IT_CC1)
			{
				_prevCC[2][0] = _currCC[2][0];
				_currCC[2][0] = TIM3->CCR1;
				TIM3->SR = ~TIM_IT_CC1;
			}
			if (TIM3->SR & TIM_IT_CC2)
			{
				_prevCC[2][1] = _currCC[2][1];
				_currCC[2][1] = TIM3->CCR2;
				TIM3->SR = ~TIM_IT_CC2;
			}
			if (TIM3->SR & TIM_IT_CC3)
			{
				_prevCC[2][2] = _currCC[2][2];
				_currCC[2][2] = TIM3->CCR3;
				TIM3->SR = ~TIM_IT_CC3;
			}
			if (TIM3->SR & TIM_IT_CC4)
			{
				_prevCC[2][3] = _currCC[2][3];
				_currCC[2][3] = TIM3->CCR4;
				TIM3->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM3)
		void TIM3_CC_IRQHandler(void)
		{
			if (timerCallBack3 != 0 && TIM3->SR & TIM_IT_CC1)
				timerCallBack3->Execute();
			if (TIM3->SR & TIM_IT_CC1)
			{
				_prevCC[2][0] = _currCC[2][0];
				_currCC[2][0] = TIM3->CCR1;
				TIM3->SR = ~TIM_IT_CC1;
			}
			if (TIM3->SR & TIM_IT_CC2)
			{
				_prevCC[2][1] = _currCC[2][1];
				_currCC[2][1] = TIM3->CCR2;
				TIM3->SR = ~TIM_IT_CC2;
			}
			if (TIM3->SR & TIM_IT_CC3)
			{
				_prevCC[2][2] = _currCC[2][2];
				_currCC[2][2] = TIM3->CCR3;
				TIM3->SR = ~TIM_IT_CC3;
			}
			if (TIM3->SR & TIM_IT_CC4)
			{
				_prevCC[2][3] = _currCC[2][3];
				_currCC[2][3] = TIM3->CCR4;
				TIM3->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM4)
		void TIM4_IRQHandler(void)
		{
			if (timerCallBack4 != 0 && TIM4->SR & TIM_IT_CC1)
				timerCallBack4->Execute();
			if (TIM4->SR & TIM_IT_CC1)
			{
				_prevCC[3][0] = _currCC[3][0];
				_currCC[3][0] = TIM4->CCR1;
				TIM4->SR = ~TIM_IT_CC1;
			}
			if (TIM4->SR & TIM_IT_CC2)
			{
				_prevCC[3][1] = _currCC[3][1];
				_currCC[3][1] = TIM4->CCR2;
				TIM4->SR = ~TIM_IT_CC2;
			}
			if (TIM4->SR & TIM_IT_CC3)
			{
				_prevCC[3][2] = _currCC[3][2];
				_currCC[3][2] = TIM4->CCR3;
				TIM4->SR = ~TIM_IT_CC3;
			}
			if (TIM4->SR & TIM_IT_CC4)
			{
				_prevCC[3][3] = _currCC[3][3];
				_currCC[3][3] = TIM4->CCR4;
				TIM4->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM4)
		void TIM4_CC_IRQHandler(void)
		{
			if (timerCallBack4 != 0 && TIM4->SR & TIM_IT_CC1)
				timerCallBack4->Execute();
			if (TIM4->SR & TIM_IT_CC1)
			{
				_prevCC[3][0] = _currCC[3][0];
				_currCC[3][0] = TIM4->CCR1;
				TIM4->SR = ~TIM_IT_CC1;
			}
			if (TIM4->SR & TIM_IT_CC2)
			{
				_prevCC[3][1] = _currCC[3][1];
				_currCC[3][1] = TIM4->CCR2;
				TIM4->SR = ~TIM_IT_CC2;
			}
			if (TIM4->SR & TIM_IT_CC3)
			{
				_prevCC[3][2] = _currCC[3][2];
				_currCC[3][2] = TIM4->CCR3;
				TIM4->SR = ~TIM_IT_CC3;
			}
			if (TIM4->SR & TIM_IT_CC4)
			{
				_prevCC[3][3] = _currCC[3][3];
				_currCC[3][3] = TIM4->CCR4;
				TIM4->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM5)
		void TIM5_IRQHandler(void)
		{
			if (timerCallBack5 != 0 && TIM5->SR & TIM_IT_CC1)
				timerCallBack5->Execute();
			if (TIM5->SR & TIM_IT_CC1)
			{
				_prevCC[4][0] = _currCC[4][0];
				_currCC[4][0] = TIM5->CCR1;
				TIM5->SR = ~TIM_IT_CC1;
			}
			if (TIM5->SR & TIM_IT_CC2)
			{
				_prevCC[4][1] = _currCC[4][1];
				_currCC[4][1] = TIM5->CCR2;
				TIM5->SR = ~TIM_IT_CC2;
			}
			if (TIM5->SR & TIM_IT_CC3)
			{
				_prevCC[4][2] = _currCC[4][2];
				_currCC[4][2] = TIM5->CCR3;
				TIM5->SR = ~TIM_IT_CC3;
			}
			if (TIM5->SR & TIM_IT_CC4)
			{
				_prevCC[4][3] = _currCC[4][3];
				_currCC[4][3] = TIM5->CCR4;
				TIM5->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM5)
		void TIM5_CC_IRQHandler(void)
		{
			if (timerCallBack5 != 0 && TIM5->SR & TIM_IT_CC1)
				timerCallBack5->Execute();
			if (TIM5->SR & TIM_IT_CC1)
			{
				_prevCC[4][0] = _currCC[4][0];
				_currCC[4][0] = TIM5->CCR1;
				TIM5->SR = ~TIM_IT_CC1;
			}
			if (TIM5->SR & TIM_IT_CC2)
			{
				_prevCC[4][1] = _currCC[4][1];
				_currCC[4][1] = TIM5->CCR2;
				TIM5->SR = ~TIM_IT_CC2;
			}
			if (TIM5->SR & TIM_IT_CC3)
			{
				_prevCC[4][2] = _currCC[4][2];
				_currCC[4][2] = TIM5->CCR3;
				TIM5->SR = ~TIM_IT_CC3;
			}
			if (TIM5->SR & TIM_IT_CC4)
			{
				_prevCC[4][3] = _currCC[4][3];
				_currCC[4][3] = TIM5->CCR4;
				TIM5->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM6)
		void TIM6_IRQHandler(void)
		{
			if (timerCallBack6 != 0 && TIM6->SR & TIM_IT_CC1)
				timerCallBack6->Execute();
			if (TIM6->SR & TIM_IT_CC1)
			{
				_prevCC[5][0] = _currCC[5][0];
				_currCC[5][0] = TIM6->CCR1;
				TIM6->SR = ~TIM_IT_CC1;
			}
			if (TIM6->SR & TIM_IT_CC2)
			{
				_prevCC[5][1] = _currCC[5][1];
				_currCC[5][1] = TIM6->CCR2;
				TIM6->SR = ~TIM_IT_CC2;
			}
			if (TIM6->SR & TIM_IT_CC3)
			{
				_prevCC[5][2] = _currCC[5][2];
				_currCC[5][2] = TIM6->CCR3;
				TIM6->SR = ~TIM_IT_CC3;
			}
			if (TIM6->SR & TIM_IT_CC4)
			{
				_prevCC[5][3] = _currCC[5][3];
				_currCC[5][3] = TIM6->CCR4;
				TIM6->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM6)
		void TIM6_CC_IRQHandler(void)
		{
			if (timerCallBack6 != 0 && TIM6->SR & TIM_IT_CC1)
				timerCallBack6->Execute();
			if (TIM6->SR & TIM_IT_CC1)
			{
				_prevCC[5][0] = _currCC[5][0];
				_currCC[5][0] = TIM6->CCR1;
				TIM6->SR = ~TIM_IT_CC1;
			}
			if (TIM6->SR & TIM_IT_CC2)
			{
				_prevCC[5][1] = _currCC[5][1];
				_currCC[5][1] = TIM6->CCR2;
				TIM6->SR = ~TIM_IT_CC2;
			}
			if (TIM6->SR & TIM_IT_CC3)
			{
				_prevCC[5][2] = _currCC[5][2];
				_currCC[5][2] = TIM6->CCR3;
				TIM6->SR = ~TIM_IT_CC3;
			}
			if (TIM6->SR & TIM_IT_CC4)
			{
				_prevCC[5][3] = _currCC[5][3];
				_currCC[5][3] = TIM6->CCR4;
				TIM6->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM7)
		void TIM7_IRQHandler(void)
		{
			if (timerCallBack7 != 0 && TIM7->SR & TIM_IT_CC1)
				timerCallBack7->Execute();
			if (TIM7->SR & TIM_IT_CC1)
			{
				_prevCC[6][0] = _currCC[6][0];
				_currCC[6][0] = TIM7->CCR1;
				TIM7->SR = ~TIM_IT_CC1;
			}
			if (TIM7->SR & TIM_IT_CC2)
			{
				_prevCC[6][1] = _currCC[6][1];
				_currCC[6][1] = TIM7->CCR2;
				TIM7->SR = ~TIM_IT_CC2;
			}
			if (TIM7->SR & TIM_IT_CC3)
			{
				_prevCC[6][2] = _currCC[6][2];
				_currCC[6][2] = TIM7->CCR3;
				TIM7->SR = ~TIM_IT_CC3;
			}
			if (TIM7->SR & TIM_IT_CC4)
			{
				_prevCC[6][3] = _currCC[6][3];
				_currCC[6][3] = TIM7->CCR4;
				TIM7->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM7)
		void TIM7_CC_IRQHandler(void)
		{
			if (timerCallBack7 != 0 && TIM7->SR & TIM_IT_CC1)
				timerCallBack7->Execute();
			if (TIM7->SR & TIM_IT_CC1)
			{
				_prevCC[6][0] = _currCC[6][0];
				_currCC[6][0] = TIM7->CCR1;
				TIM7->SR = ~TIM_IT_CC1;
			}
			if (TIM7->SR & TIM_IT_CC2)
			{
				_prevCC[6][1] = _currCC[6][1];
				_currCC[6][1] = TIM7->CCR2;
				TIM7->SR = ~TIM_IT_CC2;
			}
			if (TIM7->SR & TIM_IT_CC3)
			{
				_prevCC[6][2] = _currCC[6][2];
				_currCC[6][2] = TIM7->CCR3;
				TIM7->SR = ~TIM_IT_CC3;
			}
			if (TIM7->SR & TIM_IT_CC4)
			{
				_prevCC[6][3] = _currCC[6][3];
				_currCC[6][3] = TIM7->CCR4;
				TIM7->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM8)
		void TIM8_IRQHandler(void)
		{
			if (timerCallBack8 != 0 && TIM8->SR & TIM_IT_CC1)
				timerCallBack8->Execute();
			if (TIM8->SR & TIM_IT_CC1)
			{
				_prevCC[7][0] = _currCC[7][0];
				_currCC[7][0] = TIM8->CCR1;
				TIM8->SR = ~TIM_IT_CC1;
			}
			if (TIM8->SR & TIM_IT_CC2)
			{
				_prevCC[7][1] = _currCC[7][1];
				_currCC[7][1] = TIM8->CCR2;
				TIM8->SR = ~TIM_IT_CC2;
			}
			if (TIM8->SR & TIM_IT_CC3)
			{
				_prevCC[7][2] = _currCC[7][2];
				_currCC[7][2] = TIM8->CCR3;
				TIM8->SR = ~TIM_IT_CC3;
			}
			if (TIM8->SR & TIM_IT_CC4)
			{
				_prevCC[7][3] = _currCC[7][3];
				_currCC[7][3] = TIM8->CCR4;
				TIM8->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM8)
		void TIM8_CC_IRQHandler(void)
		{
			if (timerCallBack8 != 0 && TIM8->SR & TIM_IT_CC1)
				timerCallBack8->Execute();
			if (TIM8->SR & TIM_IT_CC1)
			{
				_prevCC[7][0] = _currCC[7][0];
				_currCC[7][0] = TIM8->CCR1;
				TIM8->SR = ~TIM_IT_CC1;
			}
			if (TIM8->SR & TIM_IT_CC2)
			{
				_prevCC[7][1] = _currCC[7][1];
				_currCC[7][1] = TIM8->CCR2;
				TIM8->SR = ~TIM_IT_CC2;
			}
			if (TIM8->SR & TIM_IT_CC3)
			{
				_prevCC[7][2] = _currCC[7][2];
				_currCC[7][2] = TIM8->CCR3;
				TIM8->SR = ~TIM_IT_CC3;
			}
			if (TIM8->SR & TIM_IT_CC4)
			{
				_prevCC[7][3] = _currCC[7][3];
				_currCC[7][3] = TIM8->CCR4;
				TIM8->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM9)
		void TIM9_IRQHandler(void)
		{
			if (timerCallBack9 != 0 && TIM9->SR & TIM_IT_CC1)
				timerCallBack9->Execute();
			if (TIM9->SR & TIM_IT_CC1)
			{
				_prevCC[8][0] = _currCC[8][0];
				_currCC[8][0] = TIM9->CCR1;
				TIM9->SR = ~TIM_IT_CC1;
			}
			if (TIM9->SR & TIM_IT_CC2)
			{
				_prevCC[8][1] = _currCC[8][1];
				_currCC[8][1] = TIM9->CCR2;
				TIM9->SR = ~TIM_IT_CC2;
			}
			if (TIM9->SR & TIM_IT_CC3)
			{
				_prevCC[8][2] = _currCC[8][2];
				_currCC[8][2] = TIM9->CCR3;
				TIM9->SR = ~TIM_IT_CC3;
			}
			if (TIM9->SR & TIM_IT_CC4)
			{
				_prevCC[8][3] = _currCC[8][3];
				_currCC[8][3] = TIM9->CCR4;
				TIM9->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM9)
		void TIM9_CC_IRQHandler(void)
		{
			if (timerCallBack9 != 0 && TIM9->SR & TIM_IT_CC1)
				timerCallBack9->Execute();
			if (TIM9->SR & TIM_IT_CC1)
			{
				_prevCC[8][0] = _currCC[8][0];
				_currCC[8][0] = TIM9->CCR1;
				TIM9->SR = ~TIM_IT_CC1;
			}
			if (TIM9->SR & TIM_IT_CC2)
			{
				_prevCC[8][1] = _currCC[8][1];
				_currCC[8][1] = TIM9->CCR2;
				TIM9->SR = ~TIM_IT_CC2;
			}
			if (TIM9->SR & TIM_IT_CC3)
			{
				_prevCC[8][2] = _currCC[8][2];
				_currCC[8][2] = TIM9->CCR3;
				TIM9->SR = ~TIM_IT_CC3;
			}
			if (TIM9->SR & TIM_IT_CC4)
			{
				_prevCC[8][3] = _currCC[8][3];
				_currCC[8][3] = TIM9->CCR4;
				TIM9->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM10)
		void TIM10_IRQHandler(void)
		{
			if (timerCallBack10 != 0 && TIM10->SR & TIM_IT_CC1)
				timerCallBack10->Execute();
			if (TIM10->SR & TIM_IT_CC1)
			{
				_prevCC[9][0] = _currCC[9][0];
				_currCC[9][0] = TIM10->CCR1;
				TIM10->SR = ~TIM_IT_CC1;
			}
			if (TIM10->SR & TIM_IT_CC2)
			{
				_prevCC[9][1] = _currCC[9][1];
				_currCC[9][1] = TIM10->CCR2;
				TIM10->SR = ~TIM_IT_CC2;
			}
			if (TIM10->SR & TIM_IT_CC3)
			{
				_prevCC[9][2] = _currCC[9][2];
				_currCC[9][2] = TIM10->CCR3;
				TIM10->SR = ~TIM_IT_CC3;
			}
			if (TIM10->SR & TIM_IT_CC4)
			{
				_prevCC[9][3] = _currCC[9][3];
				_currCC[9][3] = TIM10->CCR4;
				TIM10->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM10)
		void TIM10_CC_IRQHandler(void)
		{
			if (timerCallBack10 != 0 && TIM10->SR & TIM_IT_CC1)
				timerCallBack10->Execute();
			if (TIM10->SR & TIM_IT_CC1)
			{
				_prevCC[9][0] = _currCC[9][0];
				_currCC[9][0] = TIM10->CCR1;
				TIM10->SR = ~TIM_IT_CC1;
			}
			if (TIM10->SR & TIM_IT_CC2)
			{
				_prevCC[9][1] = _currCC[9][1];
				_currCC[9][1] = TIM10->CCR2;
				TIM10->SR = ~TIM_IT_CC2;
			}
			if (TIM10->SR & TIM_IT_CC3)
			{
				_prevCC[9][2] = _currCC[9][2];
				_currCC[9][2] = TIM10->CCR3;
				TIM10->SR = ~TIM_IT_CC3;
			}
			if (TIM10->SR & TIM_IT_CC4)
			{
				_prevCC[9][3] = _currCC[9][3];
				_currCC[9][3] = TIM10->CCR4;
				TIM10->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM11)
		void TIM11_IRQHandler(void)
		{
			if (timerCallBack11 != 0 && TIM11->SR & TIM_IT_CC1)
				timerCallBack11->Execute();
			if (TIM11->SR & TIM_IT_CC1)
			{
				_prevCC[10][0] = _currCC[10][0];
				_currCC[10][0] = TIM11->CCR1;
				TIM11->SR = ~TIM_IT_CC1;
			}
			if (TIM11->SR & TIM_IT_CC2)
			{
				_prevCC[10][1] = _currCC[10][1];
				_currCC[10][1] = TIM11->CCR2;
				TIM11->SR = ~TIM_IT_CC2;
			}
			if (TIM11->SR & TIM_IT_CC3)
			{
				_prevCC[10][2] = _currCC[10][2];
				_currCC[10][2] = TIM11->CCR3;
				TIM11->SR = ~TIM_IT_CC3;
			}
			if (TIM11->SR & TIM_IT_CC4)
			{
				_prevCC[10][3] = _currCC[10][3];
				_currCC[10][3] = TIM11->CCR4;
				TIM11->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM11)
		void TIM11_CC_IRQHandler(void)
		{
			if (timerCallBack11 != 0 && TIM11->SR & TIM_IT_CC1)
				timerCallBack11->Execute();
			if (TIM11->SR & TIM_IT_CC1)
			{
				_prevCC[10][0] = _currCC[10][0];
				_currCC[10][0] = TIM11->CCR1;
				TIM11->SR = ~TIM_IT_CC1;
			}
			if (TIM11->SR & TIM_IT_CC2)
			{
				_prevCC[10][1] = _currCC[10][1];
				_currCC[10][1] = TIM11->CCR2;
				TIM11->SR = ~TIM_IT_CC2;
			}
			if (TIM11->SR & TIM_IT_CC3)
			{
				_prevCC[10][2] = _currCC[10][2];
				_currCC[10][2] = TIM11->CCR3;
				TIM11->SR = ~TIM_IT_CC3;
			}
			if (TIM11->SR & TIM_IT_CC4)
			{
				_prevCC[10][3] = _currCC[10][3];
				_currCC[10][3] = TIM11->CCR4;
				TIM11->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM12)
		void TIM12_IRQHandler(void)
		{
			if (timerCallBack12 != 0 && TIM12->SR & TIM_IT_CC1)
				timerCallBack12->Execute();
			if (TIM12->SR & TIM_IT_CC1)
			{
				_prevCC[11][0] = _currCC[11][0];
				_currCC[11][0] = TIM12->CCR1;
				TIM12->SR = ~TIM_IT_CC1;
			}
			if (TIM12->SR & TIM_IT_CC2)
			{
				_prevCC[11][1] = _currCC[11][1];
				_currCC[11][1] = TIM12->CCR2;
				TIM12->SR = ~TIM_IT_CC2;
			}
			if (TIM12->SR & TIM_IT_CC3)
			{
				_prevCC[11][2] = _currCC[11][2];
				_currCC[11][2] = TIM12->CCR3;
				TIM12->SR = ~TIM_IT_CC3;
			}
			if (TIM12->SR & TIM_IT_CC4)
			{
				_prevCC[11][3] = _currCC[11][3];
				_currCC[11][3] = TIM12->CCR4;
				TIM12->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM12)
		void TIM12_CC_IRQHandler(void)
		{
			if (timerCallBack12 != 0 && TIM12->SR & TIM_IT_CC1)
				timerCallBack12->Execute();
			if (TIM12->SR & TIM_IT_CC1)
			{
				_prevCC[11][0] = _currCC[11][0];
				_currCC[11][0] = TIM12->CCR1;
				TIM12->SR = ~TIM_IT_CC1;
			}
			if (TIM12->SR & TIM_IT_CC2)
			{
				_prevCC[11][1] = _currCC[11][1];
				_currCC[11][1] = TIM12->CCR2;
				TIM12->SR = ~TIM_IT_CC2;
			}
			if (TIM12->SR & TIM_IT_CC3)
			{
				_prevCC[11][2] = _currCC[11][2];
				_currCC[11][2] = TIM12->CCR3;
				TIM12->SR = ~TIM_IT_CC3;
			}
			if (TIM12->SR & TIM_IT_CC4)
			{
				_prevCC[11][3] = _currCC[11][3];
				_currCC[11][3] = TIM12->CCR4;
				TIM12->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM13)
		void TIM13_IRQHandler(void)
		{
			if (timerCallBack13 != 0 && TIM13->SR & TIM_IT_CC1)
				timerCallBack13->Execute();
			if (TIM13->SR & TIM_IT_CC1)
			{
				_prevCC[12][0] = _currCC[12][0];
				_currCC[12][0] = TIM13->CCR1;
				TIM13->SR = ~TIM_IT_CC1;
			}
			if (TIM13->SR & TIM_IT_CC2)
			{
				_prevCC[12][1] = _currCC[12][1];
				_currCC[12][1] = TIM13->CCR2;
				TIM13->SR = ~TIM_IT_CC2;
			}
			if (TIM13->SR & TIM_IT_CC3)
			{
				_prevCC[12][2] = _currCC[12][2];
				_currCC[12][2] = TIM13->CCR3;
				TIM13->SR = ~TIM_IT_CC3;
			}
			if (TIM13->SR & TIM_IT_CC4)
			{
				_prevCC[12][3] = _currCC[12][3];
				_currCC[12][3] = TIM13->CCR4;
				TIM13->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM13)
		void TIM13_CC_IRQHandler(void)
		{
			if (timerCallBack13 != 0 && TIM13->SR & TIM_IT_CC1)
				timerCallBack13->Execute();
			if (TIM13->SR & TIM_IT_CC1)
			{
				_prevCC[12][0] = _currCC[12][0];
				_currCC[12][0] = TIM13->CCR1;
				TIM13->SR = ~TIM_IT_CC1;
			}
			if (TIM13->SR & TIM_IT_CC2)
			{
				_prevCC[12][1] = _currCC[12][1];
				_currCC[12][1] = TIM13->CCR2;
				TIM13->SR = ~TIM_IT_CC2;
			}
			if (TIM13->SR & TIM_IT_CC3)
			{
				_prevCC[12][2] = _currCC[12][2];
				_currCC[12][2] = TIM13->CCR3;
				TIM13->SR = ~TIM_IT_CC3;
			}
			if (TIM13->SR & TIM_IT_CC4)
			{
				_prevCC[12][3] = _currCC[12][3];
				_currCC[12][3] = TIM13->CCR4;
				TIM13->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM14)
		void TIM14_IRQHandler(void)
		{
			if (timerCallBack14 != 0 && TIM14->SR & TIM_IT_CC1)
				timerCallBack14->Execute();
			if (TIM14->SR & TIM_IT_CC1)
			{
				_prevCC[13][0] = _currCC[13][0];
				_currCC[13][0] = TIM14->CCR1;
				TIM14->SR = ~TIM_IT_CC1;
			}
			if (TIM14->SR & TIM_IT_CC2)
			{
				_prevCC[13][1] = _currCC[13][1];
				_currCC[13][1] = TIM14->CCR2;
				TIM14->SR = ~TIM_IT_CC2;
			}
			if (TIM14->SR & TIM_IT_CC3)
			{
				_prevCC[13][2] = _currCC[13][2];
				_currCC[13][2] = TIM14->CCR3;
				TIM14->SR = ~TIM_IT_CC3;
			}
			if (TIM14->SR & TIM_IT_CC4)
			{
				_prevCC[13][3] = _currCC[13][3];
				_currCC[13][3] = TIM14->CCR4;
				TIM14->SR = ~TIM_IT_CC4;
			}
		}
#endif
#if defined(TIM14)
		void TIM14_CC_IRQHandler(void)
		{
			if (timerCallBack14 != 0 && TIM14->SR & TIM_IT_CC1)
				timerCallBack14->Execute();
			if (TIM14->SR & TIM_IT_CC1)
			{
				_prevCC[13][0] = _currCC[13][0];
				_currCC[13][0] = TIM14->CCR1;
				TIM14->SR = ~TIM_IT_CC1;
			}
			if (TIM14->SR & TIM_IT_CC2)
			{
				_prevCC[13][1] = _currCC[13][1];
				_currCC[13][1] = TIM14->CCR2;
				TIM14->SR = ~TIM_IT_CC2;
			}
			if (TIM14->SR & TIM_IT_CC3)
			{
				_prevCC[13][2] = _currCC[13][2];
				_currCC[13][2] = TIM14->CCR3;
				TIM14->SR = ~TIM_IT_CC3;
			}
			if (TIM14->SR & TIM_IT_CC4)
			{
				_prevCC[13][3] = _currCC[13][3];
				_currCC[13][3] = TIM14->CCR4;
				TIM14->SR = ~TIM_IT_CC4;
			}
		}
#endif
	}
}