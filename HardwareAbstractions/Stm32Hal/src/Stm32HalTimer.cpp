#include "Stm32HalTimer.h"

namespace Stm32
{
	bool _timFrequencyLocked[TimerIndex::Num];
	std::function<void()> _timCallBack[TimerIndex::Num];

	TIM_HandleTypeDef TimInit(TimerIndex index, uint32_t prescaler, uint32_t period)
	{
		EnableTimerInterrupts(index);
		EnableTimerClock(index);
		TIM_TypeDef *TIM = TimIndexToTIM(index);
		
		TIM_HandleTypeDef TIM_HandleStruct;

		//set mode
		TIM_HandleStruct.Instance = TIM;
		TIM_HandleStruct.Init.CounterMode = TIM_COUNTERMODE_UP;
		TIM_HandleStruct.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  		TIM_HandleStruct.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
		TIM_HandleStruct.Init.Prescaler = prescaler;
  		TIM_HandleStruct.Init.Period = period; // autoreload at max
		
		//init
		HAL_TIM_Base_Init(&TIM_HandleStruct);

		//configure clock
		TIM_ClockConfigTypeDef TIM_ClockConfigStruct = {0};
		TIM_ClockConfigStruct.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		HAL_TIM_ConfigClockSource(&TIM_HandleStruct, &TIM_ClockConfigStruct);

		//start timer
		HAL_TIM_Base_Start(&TIM_HandleStruct);

		return TIM_HandleStruct;
	}

	TIM_TypeDef *TimIndexToTIM(TimerIndex index)
	{
		switch (index)
		{
#ifdef TIM1_BASE
		case TimerIndex::Index1:
			return TIM1;
#endif
#ifdef TIM2_BASE
		case TimerIndex::Index2:
			return TIM2;
#endif
#ifdef TIM3_BASE
		case TimerIndex::Index3:
			return TIM3;
#endif
#ifdef TIM4_BASE
		case TimerIndex::Index4:
			return TIM4;
#endif
#ifdef TIM5_BASE
		case TimerIndex::Index5:
			return TIM5;
#endif
#ifdef TIM6_BASE
		case TimerIndex::Index6:
			return TIM6;
#endif
#ifdef TIM7_BASE
		case TimerIndex::Index7:
			return TIM7;
#endif
#ifdef TIM8_BASE
		case TimerIndex::Index8:
			return TIM8;
#endif
#ifdef TIM9_BASE
		case TimerIndex::Index9:
			return TIM9;
#endif
#ifdef TIM10_BASE
		case TimerIndex::Index10:
			return TIM10;
#endif
#ifdef TIM11_BASE
		case TimerIndex::Index11:
			return TIM11;
#endif
#ifdef TIM12_BASE
		case TimerIndex::Index12:
			return TIM12;
#endif
#ifdef TIM13_BASE
		case TimerIndex::Index13:
			return TIM13;
#endif
#ifdef TIM14_BASE
		case TimerIndex::Index14:
			return TIM14;
#endif
#ifdef TIM15_BASE
		case TimerIndex::Index15:
			return TIM15;
#endif
#ifdef TIM16_BASE
		case TimerIndex::Index16:
			return TIM16;
#endif
#ifdef TIM17_BASE
		case TimerIndex::Index17:
			return TIM17;
#endif
#ifdef TIM18_BASE
		case TimerIndex::Index18:
			return TIM18;
#endif
#ifdef TIM19_BASE
		case TimerIndex::Index19:
			return TIM19;
#endif
#ifdef TIM20_BASE
		case TimerIndex::Index20:
			return TIM20;
#endif
#ifdef TIM21_BASE
		case TimerIndex::Index21:
			return TIM21;
#endif
#ifdef TIM22_BASE
		case TimerIndex::Index22:
			return TIM22;
#endif
		}
	}
    void EnableTimerClock(TimerIndex index)
    {
		switch (index)
		{
#ifdef TIM1_BASE
		case TimerIndex::Index1:
			__HAL_RCC_TIM1_CLK_ENABLE();
			break;
#endif
#ifdef TIM2_BASE
		case TimerIndex::Index2:
			__HAL_RCC_TIM2_CLK_ENABLE();
			break;
#endif
#ifdef TIM3_BASE
		case TimerIndex::Index3:
			__HAL_RCC_TIM3_CLK_ENABLE();
			break;
#endif
#ifdef TIM4_BASE
		case TimerIndex::Index4:
			__HAL_RCC_TIM4_CLK_ENABLE();
			break;
#endif
#ifdef TIM5_BASE
		case TimerIndex::Index5:
			__HAL_RCC_TIM5_CLK_ENABLE();
			break;
#endif
#ifdef TIM6_BASE
		case TimerIndex::Index6:
			__HAL_RCC_TIM6_CLK_ENABLE();
			break;
#endif
#ifdef TIM7_BASE
		case TimerIndex::Index7:
			__HAL_RCC_TIM7_CLK_ENABLE();
			break;
#endif
#ifdef TIM8_BASE
		case TimerIndex::Index8:
			__HAL_RCC_TIM8_CLK_ENABLE();
			break;
#endif
#ifdef TIM9_BASE
		case TimerIndex::Index9:
			__HAL_RCC_TIM9_CLK_ENABLE();
			break;
#endif
#ifdef TIM10_BASE
		case TimerIndex::Index10:
			__HAL_RCC_TIM10_CLK_ENABLE();
			break;
#endif
#ifdef TIM11_BASE
		case TimerIndex::Index11:
			__HAL_RCC_TIM11_CLK_ENABLE();
			break;
#endif
#ifdef TIM12_BASE
		case TimerIndex::Index12:
			__HAL_RCC_TIM12_CLK_ENABLE();
			break;
#endif
#ifdef TIM13_BASE
		case TimerIndex::Index13:
			__HAL_RCC_TIM13_CLK_ENABLE();
			break;
#endif
#ifdef TIM15_BASE
		case TimerIndex::Index15:
			__HAL_RCC_TIM15_CLK_ENABLE();
			break;
#endif
#ifdef TIM16_BASE
		case TimerIndex::Index16:
			__HAL_RCC_TIM16_CLK_ENABLE();
			break;
#endif
#ifdef TIM17_BASE
		case TimerIndex::Index17:
			__HAL_RCC_TIM17_CLK_ENABLE();
			break;
#endif
#ifdef TIM18_BASE
		case TimerIndex::Index18:
			__HAL_RCC_TIM18_CLK_ENABLE();
			break;
#endif
#ifdef TIM19_BASE
		case TimerIndex::Index19:
			__HAL_RCC_TIM19_CLK_ENABLE();
			break;
#endif
#ifdef TIM20_BASE
		case TimerIndex::Index20:
			__HAL_RCC_TIM20_CLK_ENABLE();
			break;
#endif
#ifdef TIM21_BASE
		case TimerIndex::Index21:
			__HAL_RCC_TIM21_CLK_ENABLE();
			break;
#endif
#ifdef TIM22_BASE
		case TimerIndex::Index22:
			__HAL_RCC_TIM22_CLK_ENABLE();
			break;
#endif
		}
    }

    void EnableTimerInterrupts(TimerIndex index)
	{
		switch (index)
		{
#ifdef TIM1_BASE
		case TimerIndex::Index1:
			HAL_NVIC_SetPriority(TIM1_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
			break;
#endif
#ifdef TIM2_BASE
		case TimerIndex::Index2:
			HAL_NVIC_SetPriority(TIM2_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM2_CC_IRQn);
			break;
#endif
#ifdef TIM3_BASE
		case TimerIndex::Index3:
			HAL_NVIC_SetPriority(TIM3_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM3_CC_IRQn);
			break;
#endif
#ifdef TIM4_BASE
		case TimerIndex::Index4:
			HAL_NVIC_SetPriority(TIM4_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM4_CC_IRQn);
			break;
#endif
#ifdef TIM5_BASE
		case TimerIndex::Index5:
			HAL_NVIC_SetPriority(TIM5_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM5_CC_IRQn);
			break;
#endif
#ifdef TIM6_BASE
		case TimerIndex::Index6:
			HAL_NVIC_SetPriority(TIM6_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM6_CC_IRQn);
			break;
#endif
#ifdef TIM7_BASE
		case TimerIndex::Index7:
			HAL_NVIC_SetPriority(TIM7_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM7_CC_IRQn);
			break;
#endif
#ifdef TIM8_BASE
		case TimerIndex::Index8:
			HAL_NVIC_SetPriority(TIM8_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM8_CC_IRQn);
			break;
#endif
#ifdef TIM9_BASE
		case TimerIndex::Index9:
			HAL_NVIC_SetPriority(TIM9_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM9_CC_IRQn);
			break;
#endif
#ifdef TIM10_BASE
		case TimerIndex::Index10:
			HAL_NVIC_SetPriority(TIM10_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM10_CC_IRQn);
			break;
#endif
#ifdef TIM11_BASE
		case TimerIndex::Index11:
			HAL_NVIC_SetPriority(TIM11_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM11_CC_IRQn);
			break;
#endif
#ifdef TIM12_BASE
		case TimerIndex::Index12:
			HAL_NVIC_SetPriority(TIM12_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM12_CC_IRQn);
			break;
#endif
#ifdef TIM13_BASE
		case TimerIndex::Index13:
			HAL_NVIC_SetPriority(TIM13_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM13_CC_IRQn);
			break;
#endif
#ifdef TIM14_BASE
		case TimerIndex::Index14:
			HAL_NVIC_SetPriority(TIM14_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM14_CC_IRQn);
			break;
#endif
#ifdef TIM15_BASE
		case TimerIndex::Index15:
			HAL_NVIC_SetPriority(TIM15_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM15_CC_IRQn);
			break;
#endif
#ifdef TIM16_BASE
		case TimerIndex::Index16:
			HAL_NVIC_SetPriority(TIM16_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM16_CC_IRQn);
			break;
#endif
#ifdef TIM17_BASE
		case TimerIndex::Index17:
			HAL_NVIC_SetPriority(TIM17_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM17_CC_IRQn);
			break;
#endif
#ifdef TIM18_BASE
		case TimerIndex::Index18:
			HAL_NVIC_SetPriority(TIM18_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM18_CC_IRQn);
			break;
#endif
#ifdef TIM19_BASE
		case TimerIndex::Index19:
			HAL_NVIC_SetPriority(TIM19_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM19_CC_IRQn);
			break;
#endif
#ifdef TIM20_BASE
		case TimerIndex::Index20:
			HAL_NVIC_SetPriority(TIM20_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM20_CC_IRQn);
			break;
#endif
#ifdef TIM21_BASE
		case TimerIndex::Index21:
			HAL_NVIC_SetPriority(TIM21_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM21_CC_IRQn);
			break;
#endif
#ifdef TIM22_BASE
		case TimerIndex::Index22:
			HAL_NVIC_SetPriority(TIM22_CC_IRQn, 1, 0);
    		HAL_NVIC_EnableIRQ(TIM22_CC_IRQn);
			break;
#endif
		}
	}

	extern "C" 
	{
		uint16_t _timPreviousCC[TimerIndex::Num][4];
		uint16_t _timCurrentCC[TimerIndex::Num][4];
#ifdef TIM1_BASE
		void TIM1_CC_IRQHandler(void)
		{
			if (_timCallBack[TimerIndex::Index1] != 0 && TIM1->SR & TIM_IT_CC1)
				_timCallBack[TimerIndex::Index1]();;
			if (TIM1->SR & TIM_IT_CC1)
			{
				_timPreviousCC[TimerIndex::Index1][0] = _timCurrentCC[TimerIndex::Index1][0];
				_timCurrentCC[TimerIndex::Index1][0] = TIM1->CCR1;
				TIM1->SR = ~TIM_IT_CC1;
			}
			if (TIM1->SR & TIM_IT_CC2)
			{
				_timPreviousCC[TimerIndex::Index1][1] = _timCurrentCC[TimerIndex::Index1][1];
				_timCurrentCC[TimerIndex::Index1][1] = TIM1->CCR2;
				TIM1->SR = ~TIM_IT_CC2;
			}
			if (TIM1->SR & TIM_IT_CC3)
			{
				_timPreviousCC[TimerIndex::Index1][2] = _timCurrentCC[TimerIndex::Index1][2];
				_timCurrentCC[TimerIndex::Index1][2] = TIM1->CCR3;
				TIM1->SR = ~TIM_IT_CC3;
			}
			if (TIM1->SR & TIM_IT_CC4)
			{
				_timPreviousCC[TimerIndex::Index1][3] = _timCurrentCC[TimerIndex::Index1][3];
				_timCurrentCC[TimerIndex::Index1][3] = TIM1->CCR4;
				TIM1->SR = ~TIM_IT_CC4;
			}
		}
#endif
#ifdef TIM2_BASE
		void TIM2_CC_IRQHandler(void)
		{
			if (_timCallBack[TimerIndex::Index2] != 0 && TIM2->SR & TIM_IT_CC1)
				_timCallBack[TimerIndex::Index2]();
			if (TIM2->SR & TIM_IT_CC1)
			{
				_timPreviousCC[TimerIndex::Index2][0] = _timCurrentCC[TimerIndex::Index2][0];
				_timCurrentCC[TimerIndex::Index2][0] = TIM2->CCR1;
				TIM2->SR = ~TIM_IT_CC1;
			}
			if (TIM2->SR & TIM_IT_CC2)
			{
				_timPreviousCC[TimerIndex::Index2][1] = _timCurrentCC[TimerIndex::Index2][1];
				_timCurrentCC[TimerIndex::Index2][1] = TIM2->CCR2;
				TIM2->SR = ~TIM_IT_CC2;
			}
			if (TIM2->SR & TIM_IT_CC3)
			{
				_timPreviousCC[TimerIndex::Index2][2] = _timCurrentCC[TimerIndex::Index2][2];
				_timCurrentCC[TimerIndex::Index2][2] = TIM2->CCR3;
				TIM2->SR = ~TIM_IT_CC3;
			}
			if (TIM2->SR & TIM_IT_CC4)
			{
				_timPreviousCC[TimerIndex::Index2][3] = _timCurrentCC[TimerIndex::Index2][3];
				_timCurrentCC[TimerIndex::Index2][3] = TIM2->CCR4;
				TIM2->SR = ~TIM_IT_CC4;
			}
		}
#endif
#ifdef TIM3_BASE
		void TIM3_CC_IRQHandler(void)
		{
			if (_timCallBack[TimerIndex::Index3] != 0 && TIM3->SR & TIM_IT_CC1)
				_timCallBack[TimerIndex::Index3]();
			if (TIM3->SR & TIM_IT_CC1)
			{
				_timPreviousCC[TimerIndex::Index3][0] = _timCurrentCC[TimerIndex::Index3][0];
				_timCurrentCC[TimerIndex::Index3][0] = TIM3->CCR1;
				TIM3->SR = ~TIM_IT_CC1;
			}
			if (TIM3->SR & TIM_IT_CC2)
			{
				_timPreviousCC[TimerIndex::Index3][1] = _timCurrentCC[TimerIndex::Index3][1];
				_timCurrentCC[TimerIndex::Index3][1] = TIM3->CCR2;
				TIM3->SR = ~TIM_IT_CC2;
			}
			if (TIM3->SR & TIM_IT_CC3)
			{
				_timPreviousCC[TimerIndex::Index3][2] = _timCurrentCC[TimerIndex::Index3][2];
				_timCurrentCC[TimerIndex::Index3][2] = TIM3->CCR3;
				TIM3->SR = ~TIM_IT_CC3;
			}
			if (TIM3->SR & TIM_IT_CC4)
			{
				_timPreviousCC[TimerIndex::Index3][3] = _timCurrentCC[TimerIndex::Index3][3];
				_timCurrentCC[TimerIndex::Index3][3] = TIM3->CCR4;
				TIM3->SR = ~TIM_IT_CC4;
			}
		}
#endif
#ifdef TIM4_BASE
		void TIM4_CC_IRQHandler(void)
		{
			if (_timCallBack[TimerIndex::Index4] != 0 && TIM4->SR & TIM_IT_CC1)
				_timCallBack[TimerIndex::Index4]();
			if (TIM4->SR & TIM_IT_CC1)
			{
				_timPreviousCC[TimerIndex::Index4][0] = _timCurrentCC[TimerIndex::Index4][0];
				_timCurrentCC[TimerIndex::Index4][0] = TIM4->CCR1;
				TIM4->SR = ~TIM_IT_CC1;
			}
			if (TIM4->SR & TIM_IT_CC2)
			{
				_timPreviousCC[TimerIndex::Index4][1] = _timCurrentCC[TimerIndex::Index4][1];
				_timCurrentCC[TimerIndex::Index4][1] = TIM4->CCR2;
				TIM4->SR = ~TIM_IT_CC2;
			}
			if (TIM4->SR & TIM_IT_CC3)
			{
				_timPreviousCC[TimerIndex::Index4][2] = _timCurrentCC[TimerIndex::Index4][2];
				_timCurrentCC[TimerIndex::Index4][2] = TIM4->CCR3;
				TIM4->SR = ~TIM_IT_CC3;
			}
			if (TIM4->SR & TIM_IT_CC4)
			{
				_timPreviousCC[TimerIndex::Index4][3] = _timCurrentCC[TimerIndex::Index4][3];
				_timCurrentCC[TimerIndex::Index4][3] = TIM4->CCR4;
				TIM4->SR = ~TIM_IT_CC4;
			}
		}
#endif
#ifdef TIM5_BASE
		void TIM5_CC_IRQHandler(void)
		{
			if (_timCallBack[TimerIndex::Index5] != 0 && TIM5->SR & TIM_IT_CC1)
				_timCallBack[TimerIndex::Index5]();
			if (TIM5->SR & TIM_IT_CC1)
			{
				_timPreviousCC[TimerIndex::Index5][0] = _timCurrentCC[TimerIndex::Index5][0];
				_timCurrentCC[TimerIndex::Index5][0] = TIM5->CCR1;
				TIM5->SR = ~TIM_IT_CC1;
			}
			if (TIM5->SR & TIM_IT_CC2)
			{
				_timPreviousCC[TimerIndex::Index5][1] = _timCurrentCC[TimerIndex::Index5][1];
				_timCurrentCC[TimerIndex::Index5][1] = TIM5->CCR2;
				TIM5->SR = ~TIM_IT_CC2;
			}
			if (TIM5->SR & TIM_IT_CC3)
			{
				_timPreviousCC[TimerIndex::Index5][2] = _timCurrentCC[TimerIndex::Index5][2];
				_timCurrentCC[TimerIndex::Index5][2] = TIM5->CCR3;
				TIM5->SR = ~TIM_IT_CC3;
			}
			if (TIM5->SR & TIM_IT_CC4)
			{
				_timPreviousCC[TimerIndex::Index5][3] = _timCurrentCC[TimerIndex::Index5][3];
				_timCurrentCC[TimerIndex::Index5][3] = TIM5->CCR4;
				TIM5->SR = ~TIM_IT_CC4;
			}
		}
#endif
#ifdef TIM6_BASE
		void TIM6_CC_IRQHandler(void)
		{
			if (_timCallBack[TimerIndex::Index6] != 0 && TIM6->SR & TIM_IT_CC1)
				_timCallBack[TimerIndex::Index6]();
			if (TIM6->SR & TIM_IT_CC1)
			{
				_timPreviousCC[TimerIndex::Index6][0] = _timCurrentCC[TimerIndex::Index6][0];
				_timCurrentCC[TimerIndex::Index6][0] = TIM6->CCR1;
				TIM6->SR = ~TIM_IT_CC1;
			}
			if (TIM6->SR & TIM_IT_CC2)
			{
				_timPreviousCC[TimerIndex::Index6][1] = _timCurrentCC[TimerIndex::Index6][1];
				_timCurrentCC[TimerIndex::Index6][1] = TIM6->CCR2;
				TIM6->SR = ~TIM_IT_CC2;
			}
			if (TIM6->SR & TIM_IT_CC3)
			{
				_timPreviousCC[TimerIndex::Index6][2] = _timCurrentCC[TimerIndex::Index6][2];
				_timCurrentCC[TimerIndex::Index6][2] = TIM6->CCR3;
				TIM6->SR = ~TIM_IT_CC3;
			}
			if (TIM6->SR & TIM_IT_CC4)
			{
				_timPreviousCC[TimerIndex::Index6][3] = _timCurrentCC[TimerIndex::Index6][3];
				_timCurrentCC[TimerIndex::Index6][3] = TIM6->CCR4;
				TIM6->SR = ~TIM_IT_CC4;
			}
		}
#endif
#ifdef TIM7_BASE
		void TIM7_CC_IRQHandler(void)
		{
			if (_timCallBack[TimerIndex::Index7] != 0 && TIM7->SR & TIM_IT_CC1)
				_timCallBack[TimerIndex::Index7]();
			if (TIM7->SR & TIM_IT_CC1)
			{
				_timPreviousCC[TimerIndex::Index7][0] = _timCurrentCC[TimerIndex::Index7][0];
				_timCurrentCC[TimerIndex::Index7][0] = TIM7->CCR1;
				TIM7->SR = ~TIM_IT_CC1;
			}
			if (TIM7->SR & TIM_IT_CC2)
			{
				_timPreviousCC[TimerIndex::Index7][1] = _timCurrentCC[TimerIndex::Index7][1];
				_timCurrentCC[TimerIndex::Index7][1] = TIM7->CCR2;
				TIM7->SR = ~TIM_IT_CC2;
			}
			if (TIM7->SR & TIM_IT_CC3)
			{
				_timPreviousCC[TimerIndex::Index7][2] = _timCurrentCC[TimerIndex::Index7][2];
				_timCurrentCC[TimerIndex::Index7][2] = TIM7->CCR3;
				TIM7->SR = ~TIM_IT_CC3;
			}
			if (TIM7->SR & TIM_IT_CC4)
			{
				_timPreviousCC[TimerIndex::Index7][3] = _timCurrentCC[TimerIndex::Index7][3];
				_timCurrentCC[TimerIndex::Index7][3] = TIM7->CCR4;
				TIM7->SR = ~TIM_IT_CC4;
			}
		}
#endif
#ifdef TIM8_BASE
		void TIM8_CC_IRQHandler(void)
		{
			if (_timCallBack[TimerIndex::Index8] != 0 && TIM8->SR & TIM_IT_CC1)
				_timCallBack[TimerIndex::Index8]();
			if (TIM8->SR & TIM_IT_CC1)
			{
				_timPreviousCC[TimerIndex::Index8][0] = _timCurrentCC[TimerIndex::Index8][0];
				_timCurrentCC[TimerIndex::Index8][0] = TIM8->CCR1;
				TIM8->SR = ~TIM_IT_CC1;
			}
			if (TIM8->SR & TIM_IT_CC2)
			{
				_timPreviousCC[TimerIndex::Index8][1] = _timCurrentCC[TimerIndex::Index8][1];
				_timCurrentCC[TimerIndex::Index8][1] = TIM8->CCR2;
				TIM8->SR = ~TIM_IT_CC2;
			}
			if (TIM8->SR & TIM_IT_CC3)
			{
				_timPreviousCC[TimerIndex::Index8][2] = _timCurrentCC[TimerIndex::Index8][2];
				_timCurrentCC[TimerIndex::Index8][2] = TIM8->CCR3;
				TIM8->SR = ~TIM_IT_CC3;
			}
			if (TIM8->SR & TIM_IT_CC4)
			{
				_timPreviousCC[TimerIndex::Index8][3] = _timCurrentCC[TimerIndex::Index8][3];
				_timCurrentCC[TimerIndex::Index8][3] = TIM8->CCR4;
				TIM8->SR = ~TIM_IT_CC4;
			}
		}
#endif
#ifdef TIM9_BASE
		void TIM9_CC_IRQHandler(void)
		{
			if (_timCallBack[TimerIndex::Index9] != 0 && TIM9->SR & TIM_IT_CC1)
				_timCallBack[TimerIndex::Index9]();
			if (TIM9->SR & TIM_IT_CC1)
			{
				_timPreviousCC[TimerIndex::Index9][0] = _timCurrentCC[TimerIndex::Index9][0];
				_timCurrentCC[TimerIndex::Index9][0] = TIM9->CCR1;
				TIM9->SR = ~TIM_IT_CC1;
			}
			if (TIM9->SR & TIM_IT_CC2)
			{
				_timPreviousCC[TimerIndex::Index9][1] = _timCurrentCC[TimerIndex::Index9][1];
				_timCurrentCC[TimerIndex::Index9][1] = TIM9->CCR2;
				TIM9->SR = ~TIM_IT_CC2;
			}
			if (TIM9->SR & TIM_IT_CC3)
			{
				_timPreviousCC[TimerIndex::Index9][2] = _timCurrentCC[TimerIndex::Index9][2];
				_timCurrentCC[TimerIndex::Index9][2] = TIM9->CCR3;
				TIM9->SR = ~TIM_IT_CC3;
			}
			if (TIM9->SR & TIM_IT_CC4)
			{
				_timPreviousCC[TimerIndex::Index9][3] = _timCurrentCC[TimerIndex::Index9][3];
				_timCurrentCC[TimerIndex::Index9][3] = TIM9->CCR4;
				TIM9->SR = ~TIM_IT_CC4;
			}
		}
#endif
#ifdef TIM10_BASE
		void TIM10_CC_IRQHandler(void)
		{
			if (_timCallBack[TimerIndex::Index10] != 0 && TIM10->SR & TIM_IT_CC1)
				_timCallBack[TimerIndex::Index10]();
			if (TIM10->SR & TIM_IT_CC1)
			{
				_timPreviousCC[TimerIndex::Index10][0] = _timCurrentCC[TimerIndex::Index10][0];
				_timCurrentCC[TimerIndex::Index10][0] = TIM10->CCR1;
				TIM10->SR = ~TIM_IT_CC1;
			}
			if (TIM10->SR & TIM_IT_CC2)
			{
				_timPreviousCC[TimerIndex::Index10][1] = _timCurrentCC[TimerIndex::Index10][1];
				_timCurrentCC[TimerIndex::Index10][1] = TIM10->CCR2;
				TIM10->SR = ~TIM_IT_CC2;
			}
			if (TIM10->SR & TIM_IT_CC3)
			{
				_timPreviousCC[TimerIndex::Index10][2] = _timCurrentCC[TimerIndex::Index10][2];
				_timCurrentCC[TimerIndex::Index10][2] = TIM10->CCR3;
				TIM10->SR = ~TIM_IT_CC3;
			}
			if (TIM10->SR & TIM_IT_CC4)
			{
				_timPreviousCC[TimerIndex::Index10][3] = _timCurrentCC[TimerIndex::Index10][3];
				_timCurrentCC[TimerIndex::Index10][3] = TIM10->CCR4;
				TIM10->SR = ~TIM_IT_CC4;
			}
		}
#endif
#ifdef TIM11_BASE
		void TIM11_CC_IRQHandler(void)
		{
			if (_timCallBack[TimerIndex::Index11] != 0 && TIM11->SR & TIM_IT_CC1)
				_timCallBack[TimerIndex::Index11]();
			if (TIM11->SR & TIM_IT_CC1)
			{
				_timPreviousCC[TimerIndex::Index11][0] = _timCurrentCC[TimerIndex::Index11][0];
				_timCurrentCC[TimerIndex::Index11][0] = TIM11->CCR1;
				TIM11->SR = ~TIM_IT_CC1;
			}
			if (TIM11->SR & TIM_IT_CC2)
			{
				_timPreviousCC[TimerIndex::Index11][1] = _timCurrentCC[TimerIndex::Index11][1];
				_timCurrentCC[TimerIndex::Index11][1] = TIM11->CCR2;
				TIM11->SR = ~TIM_IT_CC2;
			}
			if (TIM11->SR & TIM_IT_CC3)
			{
				_timPreviousCC[TimerIndex::Index11][2] = _timCurrentCC[TimerIndex::Index11][2];
				_timCurrentCC[TimerIndex::Index11][2] = TIM11->CCR3;
				TIM11->SR = ~TIM_IT_CC3;
			}
			if (TIM11->SR & TIM_IT_CC4)
			{
				_timPreviousCC[TimerIndex::Index11][3] = _timCurrentCC[TimerIndex::Index11][3];
				_timCurrentCC[TimerIndex::Index11][3] = TIM11->CCR4;
				TIM11->SR = ~TIM_IT_CC4;
			}
		}
#endif
#ifdef TIM12_BASE
		void TIM12_CC_IRQHandler(void)
		{
			if (_timCallBack[TimerIndex::Index12] != 0 && TIM12->SR & TIM_IT_CC1)
				_timCallBack[TimerIndex::Index12]();
			if (TIM12->SR & TIM_IT_CC1)
			{
				_timPreviousCC[TimerIndex::Index12][0] = _timCurrentCC[TimerIndex::Index12][0];
				_timCurrentCC[TimerIndex::Index12][0] = TIM12->CCR1;
				TIM12->SR = ~TIM_IT_CC1;
			}
			if (TIM12->SR & TIM_IT_CC2)
			{
				_timPreviousCC[TimerIndex::Index12][1] = _timCurrentCC[TimerIndex::Index12][1];
				_timCurrentCC[TimerIndex::Index12][1] = TIM12->CCR2;
				TIM12->SR = ~TIM_IT_CC2;
			}
			if (TIM12->SR & TIM_IT_CC3)
			{
				_timPreviousCC[TimerIndex::Index12][2] = _timCurrentCC[TimerIndex::Index12][2];
				_timCurrentCC[TimerIndex::Index12][2] = TIM12->CCR3;
				TIM12->SR = ~TIM_IT_CC3;
			}
			if (TIM12->SR & TIM_IT_CC4)
			{
				_timPreviousCC[TimerIndex::Index12][3] = _timCurrentCC[TimerIndex::Index12][3];
				_timCurrentCC[TimerIndex::Index12][3] = TIM12->CCR4;
				TIM12->SR = ~TIM_IT_CC4;
			}
		}
#endif
#ifdef TIM13_BASE
		void TIM13_CC_IRQHandler(void)
		{
			if (_timCallBack[TimerIndex::Index13] != 0 && TIM13->SR & TIM_IT_CC1)
				_timCallBack[TimerIndex::Index13]();
			if (TIM13->SR & TIM_IT_CC1)
			{
				_timPreviousCC[TimerIndex::Index13][0] = _timCurrentCC[TimerIndex::Index13][0];
				_timCurrentCC[TimerIndex::Index13][0] = TIM13->CCR1;
				TIM13->SR = ~TIM_IT_CC1;
			}
			if (TIM13->SR & TIM_IT_CC2)
			{
				_timPreviousCC[TimerIndex::Index13][1] = _timCurrentCC[TimerIndex::Index13][1];
				_timCurrentCC[TimerIndex::Index13][1] = TIM13->CCR2;
				TIM13->SR = ~TIM_IT_CC2;
			}
			if (TIM13->SR & TIM_IT_CC3)
			{
				_timPreviousCC[TimerIndex::Index13][2] = _timCurrentCC[TimerIndex::Index13][2];
				_timCurrentCC[TimerIndex::Index13][2] = TIM13->CCR3;
				TIM13->SR = ~TIM_IT_CC3;
			}
			if (TIM13->SR & TIM_IT_CC4)
			{
				_timPreviousCC[TimerIndex::Index13][3] = _timCurrentCC[TimerIndex::Index13][3];
				_timCurrentCC[TimerIndex::Index13][3] = TIM13->CCR4;
				TIM13->SR = ~TIM_IT_CC4;
			}
		}
#endif
#ifdef TIM14_BASE
		void TIM14_CC_IRQHandler(void)
		{
			if (_timCallBack[TimerIndex::Index14] != 0 && TIM14->SR & TIM_IT_CC1)
				_timCallBack[TimerIndex::Index14]();
			if (TIM14->SR & TIM_IT_CC1)
			{
				_timPreviousCC[TimerIndex::Index14][0] = _timCurrentCC[TimerIndex::Index14][0];
				_timCurrentCC[TimerIndex::Index14][0] = TIM14->CCR1;
				TIM14->SR = ~TIM_IT_CC1;
			}
			if (TIM14->SR & TIM_IT_CC2)
			{
				_timPreviousCC[TimerIndex::Index14][1] = _timCurrentCC[TimerIndex::Index14][1];
				_timCurrentCC[TimerIndex::Index14][1] = TIM14->CCR2;
				TIM14->SR = ~TIM_IT_CC2;
			}
			if (TIM14->SR & TIM_IT_CC3)
			{
				_timPreviousCC[TimerIndex::Index14][2] = _timCurrentCC[TimerIndex::Index14][2];
				_timCurrentCC[TimerIndex::Index14][2] = TIM14->CCR3;
				TIM14->SR = ~TIM_IT_CC3;
			}
			if (TIM14->SR & TIM_IT_CC4)
			{
				_timPreviousCC[TimerIndex::Index14][3] = _timCurrentCC[TimerIndex::Index14][3];
				_timCurrentCC[TimerIndex::Index14][3] = TIM14->CCR4;
				TIM14->SR = ~TIM_IT_CC4;
			}
		}
#endif
#ifdef TIM15_BASE
		void TIM15_CC_IRQHandler(void)
		{
			if (_timCallBack[TimerIndex::Index15] != 0 && TIM15->SR & TIM_IT_CC1)
				_timCallBack[TimerIndex::Index15]();
			if (TIM15->SR & TIM_IT_CC1)
			{
				_timPreviousCC[TimerIndex::Index15][0] = _timCurrentCC[TimerIndex::Index15][0];
				_timCurrentCC[TimerIndex::Index15][0] = TIM15->CCR1;
				TIM15->SR = ~TIM_IT_CC1;
			}
			if (TIM15->SR & TIM_IT_CC2)
			{
				_timPreviousCC[TimerIndex::Index15][1] = _timCurrentCC[TimerIndex::Index15][1];
				_timCurrentCC[TimerIndex::Index15][1] = TIM15->CCR2;
				TIM15->SR = ~TIM_IT_CC2;
			}
			if (TIM15->SR & TIM_IT_CC3)
			{
				_timPreviousCC[TimerIndex::Index15][2] = _timCurrentCC[TimerIndex::Index15][2];
				_timCurrentCC[TimerIndex::Index15][2] = TIM15->CCR3;
				TIM15->SR = ~TIM_IT_CC3;
			}
			if (TIM15->SR & TIM_IT_CC4)
			{
				_timPreviousCC[TimerIndex::Index15][3] = _timCurrentCC[TimerIndex::Index15][3];
				_timCurrentCC[TimerIndex::Index15][3] = TIM15->CCR4;
				TIM15->SR = ~TIM_IT_CC4;
			}
		}
#endif
#ifdef TIM16_BASE
		void TIM16_CC_IRQHandler(void)
		{
			if (_timCallBack[TimerIndex::Index16] != 0 && TIM16->SR & TIM_IT_CC1)
				_timCallBack[TimerIndex::Index16]();
			if (TIM16->SR & TIM_IT_CC1)
			{
				_timPreviousCC[TimerIndex::Index16][0] = _timCurrentCC[TimerIndex::Index16][0];
				_timCurrentCC[TimerIndex::Index16][0] = TIM16->CCR1;
				TIM16->SR = ~TIM_IT_CC1;
			}
			if (TIM16->SR & TIM_IT_CC2)
			{
				_timPreviousCC[TimerIndex::Index16][1] = _timCurrentCC[TimerIndex::Index16][1];
				_timCurrentCC[TimerIndex::Index16][1] = TIM16->CCR2;
				TIM16->SR = ~TIM_IT_CC2;
			}
			if (TIM16->SR & TIM_IT_CC3)
			{
				_timPreviousCC[TimerIndex::Index16][2] = _timCurrentCC[TimerIndex::Index16][2];
				_timCurrentCC[TimerIndex::Index16][2] = TIM16->CCR3;
				TIM16->SR = ~TIM_IT_CC3;
			}
			if (TIM16->SR & TIM_IT_CC4)
			{
				_timPreviousCC[TimerIndex::Index16][3] = _timCurrentCC[TimerIndex::Index16][3];
				_timCurrentCC[TimerIndex::Index16][3] = TIM16->CCR4;
				TIM16->SR = ~TIM_IT_CC4;
			}
		}
#endif
#ifdef TIM17_BASE
		void TIM17_CC_IRQHandler(void)
		{
			if (_timCallBack[TimerIndex::Index17] != 0 && TIM17->SR & TIM_IT_CC1)
				_timCallBack[TimerIndex::Index17]();
			if (TIM17->SR & TIM_IT_CC1)
			{
				_timPreviousCC[TimerIndex::Index17][0] = _timCurrentCC[TimerIndex::Index17][0];
				_timCurrentCC[TimerIndex::Index17][0] = TIM17->CCR1;
				TIM17->SR = ~TIM_IT_CC1;
			}
			if (TIM17->SR & TIM_IT_CC2)
			{
				_timPreviousCC[TimerIndex::Index17][1] = _timCurrentCC[TimerIndex::Index17][1];
				_timCurrentCC[TimerIndex::Index17][1] = TIM17->CCR2;
				TIM17->SR = ~TIM_IT_CC2;
			}
			if (TIM17->SR & TIM_IT_CC3)
			{
				_timPreviousCC[TimerIndex::Index17][2] = _timCurrentCC[TimerIndex::Index17][2];
				_timCurrentCC[TimerIndex::Index17][2] = TIM17->CCR3;
				TIM17->SR = ~TIM_IT_CC3;
			}
			if (TIM17->SR & TIM_IT_CC4)
			{
				_timPreviousCC[TimerIndex::Index17][3] = _timCurrentCC[TimerIndex::Index17][3];
				_timCurrentCC[TimerIndex::Index17][3] = TIM17->CCR4;
				TIM17->SR = ~TIM_IT_CC4;
			}
		}
#endif
#ifdef TIM18_BASE
		void TIM18_CC_IRQHandler(void)
		{
			if (_timCallBack[TimerIndex::Index18] != 0 && TIM18->SR & TIM_IT_CC1)
				_timCallBack[TimerIndex::Index18]();
			if (TIM18->SR & TIM_IT_CC1)
			{
				_timPreviousCC[TimerIndex::Index18][0] = _timCurrentCC[TimerIndex::Index18][0];
				_timCurrentCC[TimerIndex::Index18][0] = TIM18->CCR1;
				TIM18->SR = ~TIM_IT_CC1;
			}
			if (TIM18->SR & TIM_IT_CC2)
			{
				_timPreviousCC[TimerIndex::Index18][1] = _timCurrentCC[TimerIndex::Index18][1];
				_timCurrentCC[TimerIndex::Index18][1] = TIM18->CCR2;
				TIM18->SR = ~TIM_IT_CC2;
			}
			if (TIM18->SR & TIM_IT_CC3)
			{
				_timPreviousCC[TimerIndex::Index18][2] = _timCurrentCC[TimerIndex::Index18][2];
				_timCurrentCC[TimerIndex::Index18][2] = TIM18->CCR3;
				TIM18->SR = ~TIM_IT_CC3;
			}
			if (TIM18->SR & TIM_IT_CC4)
			{
				_timPreviousCC[TimerIndex::Index18][3] = _timCurrentCC[TimerIndex::Index18][3];
				_timCurrentCC[TimerIndex::Index18][3] = TIM18->CCR4;
				TIM18->SR = ~TIM_IT_CC4;
			}
		}
#endif
#ifdef TIM19_BASE
		void TIM19_CC_IRQHandler(void)
		{
			if (_timCallBack[TimerIndex::Index19] != 0 && TIM19->SR & TIM_IT_CC1)
				_timCallBack[TimerIndex::Index19]();
			if (TIM19->SR & TIM_IT_CC1)
			{
				_timPreviousCC[TimerIndex::Index19][0] = _timCurrentCC[TimerIndex::Index19][0];
				_timCurrentCC[TimerIndex::Index19][0] = TIM19->CCR1;
				TIM19->SR = ~TIM_IT_CC1;
			}
			if (TIM19->SR & TIM_IT_CC2)
			{
				_timPreviousCC[TimerIndex::Index19][1] = _timCurrentCC[TimerIndex::Index19][1];
				_timCurrentCC[TimerIndex::Index19][1] = TIM19->CCR2;
				TIM19->SR = ~TIM_IT_CC2;
			}
			if (TIM19->SR & TIM_IT_CC3)
			{
				_timPreviousCC[TimerIndex::Index19][2] = _timCurrentCC[TimerIndex::Index19][2];
				_timCurrentCC[TimerIndex::Index19][2] = TIM19->CCR3;
				TIM19->SR = ~TIM_IT_CC3;
			}
			if (TIM19->SR & TIM_IT_CC4)
			{
				_timPreviousCC[TimerIndex::Index19][3] = _timCurrentCC[TimerIndex::Index19][3];
				_timCurrentCC[TimerIndex::Index19][3] = TIM19->CCR4;
				TIM19->SR = ~TIM_IT_CC4;
			}
		}
#endif
#ifdef TIM20_BASE
		void TIM20_CC_IRQHandler(void)
		{
			if (_timCallBack[TimerIndex::Index20] != 0 && TIM20->SR & TIM_IT_CC1)
				_timCallBack[TimerIndex::Index20]();
			if (TIM20->SR & TIM_IT_CC1)
			{
				_timPreviousCC[TimerIndex::Index20][0] = _timCurrentCC[TimerIndex::Index20][0];
				_timCurrentCC[TimerIndex::Index20][0] = TIM20->CCR1;
				TIM20->SR = ~TIM_IT_CC1;
			}
			if (TIM20->SR & TIM_IT_CC2)
			{
				_timPreviousCC[TimerIndex::Index20][1] = _timCurrentCC[TimerIndex::Index20][1];
				_timCurrentCC[TimerIndex::Index20][1] = TIM20->CCR2;
				TIM20->SR = ~TIM_IT_CC2;
			}
			if (TIM20->SR & TIM_IT_CC3)
			{
				_timPreviousCC[TimerIndex::Index20][2] = _timCurrentCC[TimerIndex::Index20][2];
				_timCurrentCC[TimerIndex::Index20][2] = TIM20->CCR3;
				TIM20->SR = ~TIM_IT_CC3;
			}
			if (TIM20->SR & TIM_IT_CC4)
			{
				_timPreviousCC[TimerIndex::Index20][3] = _timCurrentCC[TimerIndex::Index20][3];
				_timCurrentCC[TimerIndex::Index20][3] = TIM20->CCR4;
				TIM20->SR = ~TIM_IT_CC4;
			}
		}
#endif
#ifdef TIM21_BASE
		void TIM21_CC_IRQHandler(void)
		{
			if (_timCallBack[TimerIndex::Index21] != 0 && TIM21->SR & TIM_IT_CC1)
				_timCallBack[TimerIndex::Index21]();
			if (TIM21->SR & TIM_IT_CC1)
			{
				_timPreviousCC[TimerIndex::Index21][0] = _timCurrentCC[TimerIndex::Index21][0];
				_timCurrentCC[TimerIndex::Index21][0] = TIM21->CCR1;
				TIM21->SR = ~TIM_IT_CC1;
			}
			if (TIM21->SR & TIM_IT_CC2)
			{
				_timPreviousCC[TimerIndex::Index21][1] = _timCurrentCC[TimerIndex::Index21][1];
				_timCurrentCC[TimerIndex::Index21][1] = TIM21->CCR2;
				TIM21->SR = ~TIM_IT_CC2;
			}
			if (TIM21->SR & TIM_IT_CC3)
			{
				_timPreviousCC[TimerIndex::Index21][2] = _timCurrentCC[TimerIndex::Index21][2];
				_timCurrentCC[TimerIndex::Index21][2] = TIM21->CCR3;
				TIM21->SR = ~TIM_IT_CC3;
			}
			if (TIM21->SR & TIM_IT_CC4)
			{
				_timPreviousCC[TimerIndex::Index21][3] = _timCurrentCC[TimerIndex::Index21][3];
				_timCurrentCC[TimerIndex::Index21][3] = TIM21->CCR4;
				TIM21->SR = ~TIM_IT_CC4;
			}
		}
#endif
#ifdef TIM22_BASE
		void TIM22_CC_IRQHandler(void)
		{
			if (_timCallBack[TimerIndex::Index22] != 0 && TIM22->SR & TIM_IT_CC1)
				_timCallBack[TimerIndex::Index22]();
			if (TIM22->SR & TIM_IT_CC1)
			{
				_timPreviousCC[TimerIndex::Index22][0] = _timCurrentCC[TimerIndex::Index22][0];
				_timCurrentCC[TimerIndex::Index22][0] = TIM22->CCR1;
				TIM22->SR = ~TIM_IT_CC1;
			}
			if (TIM22->SR & TIM_IT_CC2)
			{
				_timPreviousCC[TimerIndex::Index22][1] = _timCurrentCC[TimerIndex::Index22][1];
				_timCurrentCC[TimerIndex::Index22][1] = TIM22->CCR2;
				TIM22->SR = ~TIM_IT_CC2;
			}
			if (TIM22->SR & TIM_IT_CC3)
			{
				_timPreviousCC[TimerIndex::Index22][2] = _timCurrentCC[TimerIndex::Index22][2];
				_timCurrentCC[TimerIndex::Index22][2] = TIM22->CCR3;
				TIM22->SR = ~TIM_IT_CC3;
			}
			if (TIM22->SR & TIM_IT_CC4)
			{
				_timPreviousCC[TimerIndex::Index22][3] = _timCurrentCC[TimerIndex::Index22][3];
				_timCurrentCC[TimerIndex::Index22][3] = TIM22->CCR4;
				TIM22->SR = ~TIM_IT_CC4;
			}
		}
#endif
	}
}