#include "Stm32HalConf.h"
#include "Stm32HalPwmService.h"
#include <algorithm>
#include <math.h>
#include "TimerLocks.h"

namespace Stm32
{
	void Stm32HalPwmPinToTimAndChannel(unsigned short pin, unsigned char *TimNum, TIM_TypeDef **TIM, unsigned short *TIM_Channel)
	{
		switch (pin)
		{
#ifdef PWM_TIM1_CC1
		case PWM_TIM1_CC1 : 
			*TIM_Channel = TIM_CHANNEL_1;
			*TIM = TIM1;
			break;
#endif
#ifdef PWM_TIM1_CC2
		case PWM_TIM1_CC2 : 
			*TIM_Channel = TIM_CHANNEL_2;
			*TIM = TIM1;
			break;
#endif
#ifdef PWM_TIM1_CC3
		case PWM_TIM1_CC3 : 
			*TIM_Channel = TIM_CHANNEL_3;
			*TIM = TIM1;
			break;
#endif
#ifdef PWM_TIM1_CC4
		case PWM_TIM1_CC4 : 
			*TIM_Channel = TIM_CHANNEL_4;
			*TIM = TIM1;
			break;
#endif
#ifdef PWM_TIM2_CC1
		case PWM_TIM2_CC1 : 
			*TIM_Channel = TIM_CHANNEL_1;
			*TIM = TIM2;
			break;
#endif
#ifdef PWM_TIM2_CC2
		case PWM_TIM2_CC2 : 
			*TIM_Channel = TIM_CHANNEL_2;
			*TIM = TIM2;
			break;
#endif
#ifdef PWM_TIM2_CC3
		case PWM_TIM2_CC3 : 
			*TIM_Channel = TIM_CHANNEL_3;
			*TIM = TIM2;
			break;
#endif
#ifdef PWM_TIM2_CC4
		case PWM_TIM2_CC4 : 
			*TIM_Channel = TIM_CHANNEL_4;
			*TIM = TIM2;
			break;
#endif
#ifdef PWM_TIM3_CC1
		case PWM_TIM3_CC1 : 
			*TIM_Channel = TIM_CHANNEL_1;
			*TIM = TIM3;
			break;
#endif
#ifdef PWM_TIM3_CC2
		case PWM_TIM3_CC2 : 
			*TIM_Channel = TIM_CHANNEL_2;
			*TIM = TIM3;
			break;
#endif
#ifdef PWM_TIM3_CC3
		case PWM_TIM3_CC3 : 
			*TIM_Channel = TIM_CHANNEL_3;
			*TIM = TIM3;
			break;
#endif
#ifdef PWM_TIM3_CC4
		case PWM_TIM3_CC4 : 
			*TIM_Channel = TIM_CHANNEL_4;
			*TIM = TIM3;
			break;
#endif
#ifdef PWM_TIM4_CC1
		case PWM_TIM4_CC1 : 
			*TIM_Channel = TIM_CHANNEL_1;
			*TIM = TIM4;
			break;
#endif
#ifdef PWM_TIM4_CC2
		case PWM_TIM4_CC2 : 
			*TIM_Channel = TIM_CHANNEL_2;
			*TIM = TIM4;
			break;
#endif
#ifdef PWM_TIM4_CC3
		case PWM_TIM4_CC3 : 
			*TIM_Channel = TIM_CHANNEL_3;
			*TIM = TIM4;
			break;
#endif
#ifdef PWM_TIM4_CC4
		case PWM_TIM4_CC4 : 
			*TIM_Channel = TIM_CHANNEL_4;
			*TIM = TIM4;
			break;
#endif
#ifdef PWM_TIM5_CC1
		case PWM_TIM5_CC1 : 
			*TIM_Channel = TIM_CHANNEL_1;
			*TIM = TIM5;
			break;
#endif
#ifdef PWM_TIM5_CC2
		case PWM_TIM5_CC2 : 
			*TIM_Channel = TIM_CHANNEL_2;
			*TIM = TIM5;
			break;
#endif
#ifdef PWM_TIM5_CC3
		case PWM_TIM5_CC3 : 
			*TIM_Channel = TIM_CHANNEL_3;
			*TIM = TIM5;
			break;
#endif
#ifdef PWM_TIM5_CC4
		case PWM_TIM5_CC4 : 
			*TIM_Channel = TIM_CHANNEL_4;
			*TIM = TIM5;
			break;
#endif
#ifdef PWM_TIM6_CC1
		case PWM_TIM6_CC1 : 
			*TIM_Channel = TIM_CHANNEL_1;
			*TIM = TIM6;
			break;
#endif
#ifdef PWM_TIM6_CC2
		case PWM_TIM6_CC2 : 
			*TIM_Channel = TIM_CHANNEL_2;
			*TIM = TIM6;
			break;
#endif
#ifdef PWM_TIM6_CC3
		case PWM_TIM6_CC3 : 
			*TIM_Channel = TIM_CHANNEL_3;
			*TIM = TIM6;
			break;
#endif
#ifdef PWM_TIM6_CC4
		case PWM_TIM6_CC4 : 
			*TIM_Channel = TIM_CHANNEL_4;
			*TIM = TIM6;
			break;
#endif
#ifdef PWM_TIM7_CC1
		case PWM_TIM7_CC1 : 
			*TIM_Channel = TIM_CHANNEL_1;
			*TIM = TIM7;
			break;
#endif
#ifdef PWM_TIM7_CC2
		case PWM_TIM7_CC2 : 
			*TIM_Channel = TIM_CHANNEL_2;
			*TIM = TIM7;
			break;
#endif
#ifdef PWM_TIM7_CC3
		case PWM_TIM7_CC3 : 
			*TIM_Channel = TIM_CHANNEL_3;
			*TIM = TIM7;
			break;
#endif
#ifdef PWM_TIM7_CC4
		case PWM_TIM7_CC4 : 
			*TIM_Channel = TIM_CHANNEL_4;
			*TIM = TIM7;
			break;
#endif
#ifdef PWM_TIM8_CC1
		case PWM_TIM8_CC1 : 
			*TIM_Channel = TIM_CHANNEL_1;
			*TIM = TIM8;
			break;
#endif
#ifdef PWM_TIM8_CC2
		case PWM_TIM8_CC2 : 
			*TIM_Channel = TIM_CHANNEL_2;
			*TIM = TIM8;
			break;
#endif
#ifdef PWM_TIM8_CC3
		case PWM_TIM8_CC3 : 
			*TIM_Channel = TIM_CHANNEL_3;
			*TIM = TIM8;
			break;
#endif
#ifdef PWM_TIM8_CC4
		case PWM_TIM8_CC4 : 
			*TIM_Channel = TIM_CHANNEL_4;
			*TIM = TIM8;
			break;
#endif
#ifdef PWM_TIM9_CC1
		case PWM_TIM9_CC1 : 
			*TIM_Channel = TIM_CHANNEL_1;
			*TIM = TIM9;
			break;
#endif
#ifdef PWM_TIM9_CC2
		case PWM_TIM9_CC2 : 
			*TIM_Channel = TIM_CHANNEL_2;
			*TIM = TIM9;
			break;
#endif
#ifdef PWM_TIM9_CC3
		case PWM_TIM9_CC3 : 
			*TIM_Channel = TIM_CHANNEL_3;
			*TIM = TIM9;
			break;
#endif
#ifdef PWM_TIM9_CC4
		case PWM_TIM9_CC4 : 
			*TIM_Channel = TIM_CHANNEL_4;
			*TIM = TIM9;
			break;
#endif
#ifdef PWM_TIM10_CC1
		case PWM_TIM10_CC1 : 
			*TIM_Channel = TIM_CHANNEL_1;
			*TIM = TIM10;
			break;
#endif
#ifdef PWM_TIM10_CC2
		case PWM_TIM10_CC2 : 
			*TIM_Channel = TIM_CHANNEL_2;
			*TIM = TIM10;
			break;
#endif
#ifdef PWM_TIM10_CC3
		case PWM_TIM10_CC3 : 
			*TIM_Channel = TIM_CHANNEL_3;
			*TIM = TIM10;
			break;
#endif
#ifdef PWM_TIM10_CC4
		case PWM_TIM10_CC4 : 
			*TIM_Channel = TIM_CHANNEL_4;
			*TIM = TIM10;
			break;
#endif
#ifdef PWM_TIM11_CC1
		case PWM_TIM11_CC1 : 
			*TIM_Channel = TIM_CHANNEL_1;
			*TIM = TIM11;
			break;
#endif
#ifdef PWM_TIM11_CC2
		case PWM_TIM11_CC2 : 
			*TIM_Channel = TIM_CHANNEL_2;
			*TIM = TIM11;
			break;
#endif
#ifdef PWM_TIM11_CC3
		case PWM_TIM11_CC3 : 
			*TIM_Channel = TIM_CHANNEL_3;
			*TIM = TIM11;
			break;
#endif
#ifdef PWM_TIM11_CC4
		case PWM_TIM11_CC4 : 
			*TIM_Channel = TIM_CHANNEL_4;
			*TIM = TIM11;
			break;
#endif
#ifdef PWM_TIM12_CC1
		case PWM_TIM12_CC1 : 
			*TIM_Channel = TIM_CHANNEL_1;
			*TIM = TIM12;
			break;
#endif
#ifdef PWM_TIM12_CC2
		case PWM_TIM12_CC2 : 
			*TIM_Channel = TIM_CHANNEL_2;
			*TIM = TIM12;
			break;
#endif
#ifdef PWM_TIM12_CC3
		case PWM_TIM12_CC3 : 
			*TIM_Channel = TIM_CHANNEL_3;
			*TIM = TIM12;
			break;
#endif
#ifdef PWM_TIM12_CC4
		case PWM_TIM12_CC4 : 
			*TIM_Channel = TIM_CHANNEL_4;
			*TIM = TIM12;
			break;
#endif
#ifdef PWM_TIM13_CC1
		case PWM_TIM13_CC1 : 
			*TIM_Channel = TIM_CHANNEL_1;
			*TIM = TIM13;
			break;
#endif
#ifdef PWM_TIM13_CC2
		case PWM_TIM13_CC2 : 
			*TIM_Channel = TIM_CHANNEL_2;
			*TIM = TIM13;
			break;
#endif
#ifdef PWM_TIM13_CC3
		case PWM_TIM13_CC3 : 
			*TIM_Channel = TIM_CHANNEL_3;
			*TIM = TIM13;
			break;
#endif
#ifdef PWM_TIM13_CC4
		case PWM_TIM13_CC4 : 
			*TIM_Channel = TIM_CHANNEL_4;
			*TIM = TIM13;
			break;
#endif
#ifdef PWM_TIM14_CC1
		case PWM_TIM14_CC1 : 
			*TIM_Channel = TIM_CHANNEL_1;
			*TIM = TIM14;
			break;
#endif
#ifdef PWM_TIM14_CC2
		case PWM_TIM14_CC2 : 
			*TIM_Channel = TIM_CHANNEL_2;
			*TIM = TIM14;
			break;
#endif
#ifdef PWM_TIM14_CC3
		case PWM_TIM14_CC3 : 
			*TIM_Channel = TIM_CHANNEL_3;
			*TIM = TIM14;
			break;
#endif
#ifdef PWM_TIM14_CC4
		case PWM_TIM14_CC4 : 
			*TIM_Channel = TIM_CHANNEL_4;
			*TIM = TIM14;
			break;
#endif
		}
	}

	Stm32HalPwmService *PwmService = 0;
	
	Stm32HalPwmService::Stm32HalPwmService()
	{
		PwmService = this;
	}
	
	void Stm32HalPwmService::InitPin(unsigned short pin, PinDirection direction, unsigned short minFrequency)
	{
		if (pin == 0)
			return;
		
		unsigned short TIM_Channel;
		unsigned char TimNum = 0;
		TIM_TypeDef *TIM;
		Stm32HalPwmPinToTimAndChannel(pin, &TimNum, &TIM, &TIM_Channel);
				
		if(direction == In)
		{		
			switch (TimNum)
			{
	#ifdef TIM1
			case 1:
				if (TIM1_Freq_Locked)
					return;
				TIM1_Freq_Locked = true;
				__HAL_RCC_TIM1_CLK_ENABLE();
	#ifdef TIM1_SEPERATE_UP_CC_IT
				HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
	#else
				HAL_NVIC_SetPriority(TIM1_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM1_IRQn);
	#endif
				break;
	#endif
	#ifdef TIM2
			case 2:
				if (TIM2_Freq_Locked)
					return;
				if(direction == Out)
					TIM2_Freq_Locked = true;
				__HAL_RCC_TIM2_CLK_ENABLE();
	#ifdef TIM2_SEPERATE_UP_CC_IT
				HAL_NVIC_SetPriority(TIM2_CC_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM2_CC_IRQn);
	#else
				HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM2_IRQn);
	#endif
				break;
	#endif
	#ifdef TIM3
			case 3:
				if (TIM3_Freq_Locked)
					return;
				TIM3_Freq_Locked = true;
				__HAL_RCC_TIM3_CLK_ENABLE();
	#ifdef TIM3_SEPERATE_UP_CC_IT
				HAL_NVIC_SetPriority(TIM3_CC_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM3_CC_IRQn);
	#else
				HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM3_IRQn);
	#endif
				break;
	#endif
	#ifdef TIM4
			case 4:
				if (TIM4_Freq_Locked)
					return;
				TIM4_Freq_Locked = true;
				__HAL_RCC_TIM4_CLK_ENABLE();
	#ifdef TIM4_SEPERATE_UP_CC_IT
				HAL_NVIC_SetPriority(TIM4_CC_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM4_CC_IRQn);
	#else
				HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM4_IRQn);
	#endif
				break;
	#endif
	#ifdef TIM5
			case 5:
				if (TIM5_Freq_Locked)
					return;
				TIM5_Freq_Locked = true;
				__HAL_RCC_TIM5_CLK_ENABLE();
	#ifdef TIM5_SEPERATE_UP_CC_IT
				HAL_NVIC_SetPriority(TIM5_CC_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM5_CC_IRQn);
	#else
				HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM5_IRQn);
	#endif
				break;
	#endif
	#ifdef TIM6
			case 6:
				if (TIM6_Freq_Locked)
					return;
				TIM6_Freq_Locked = true;
				__HAL_RCC_TIM6_CLK_ENABLE();
	#ifdef TIM6_SEPERATE_UP_CC_IT
				HAL_NVIC_SetPriority(TIM6_CC_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM6_CC_IRQn);
	#else
				HAL_NVIC_SetPriority(TIM6_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM6_IRQn);
	#endif
				break;
	#endif
	#ifdef TIM7
			case 7:
				if (TIM7_Freq_Locked)
					return;
				TIM7_Freq_Locked = true;
				__HAL_RCC_TIM7_CLK_ENABLE();
	#ifdef TIM7_SEPERATE_UP_CC_IT
				HAL_NVIC_SetPriority(TIM7_CC_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM7_CC_IRQn);
	#else
				HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM7_IRQn);
	#endif
				break;
	#endif
	#ifdef TIM8
			case 8:
				if (TIM8_Freq_Locked)
					return;
				TIM8_Freq_Locked = true;
				__HAL_RCC_TIM8_CLK_ENABLE();
	#ifdef TIM8_SEPERATE_UP_CC_IT
				HAL_NVIC_SetPriority(TIM8_CC_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM8_CC_IRQn);
	#else
				HAL_NVIC_SetPriority(TIM8_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM8_IRQn);
	#endif
				break;
	#endif
	#ifdef TIM9
			case 9:
				if (TIM9_Freq_Locked)
					return;
				TIM9_Freq_Locked = true;
				__HAL_RCC_TIM9_CLK_ENABLE();
	#ifdef TIM9_SEPERATE_UP_CC_IT
				HAL_NVIC_SetPriority(TIM9_CC_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM9_CC_IRQn);
	#else
				HAL_NVIC_SetPriority(TIM9_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM9_IRQn);
	#endif
				break;
	#endif
	#ifdef TIM10
			case 10:
				if (TIM10_Freq_Locked)
					return;
				TIM10_Freq_Locked = true;
				__HAL_RCC_TIM10_CLK_ENABLE();
	#ifdef TIM10_SEPERATE_UP_CC_IT
				HAL_NVIC_SetPriority(TIM10_CC_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM10_CC_IRQn);
	#else
				HAL_NVIC_SetPriority(TIM10_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM10_IRQn);
	#endif
				break;
	#endif
	#ifdef TIM11
			case 11:
				if (TIM11_Freq_Locked)
					return;
				TIM11_Freq_Locked = true;
				__HAL_RCC_TIM11_CLK_ENABLE();
	#ifdef TIM11_SEPERATE_UP_CC_IT
				HAL_NVIC_SetPriority(TIM11_CC_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM11_CC_IRQn);
	#else
				HAL_NVIC_SetPriority(TIM11_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM11_IRQn);
	#endif
				break;
	#endif
	#ifdef TIM12
			case 12:
				if (TIM12_Freq_Locked)
					return;
				TIM12_Freq_Locked = true;
				__HAL_RCC_TIM12_CLK_ENABLE();
	#ifdef TIM12_SEPERATE_UP_CC_IT
				HAL_NVIC_SetPriority(TIM12_CC_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM12_CC_IRQn);
	#else
				HAL_NVIC_SetPriority(TIM12_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM12_IRQn);
	#endif
				break;
	#endif
	#ifdef TIM13
			case 13:
				if (TIM13_Freq_Locked)
					return;
				TIM13_Freq_Locked = true;
				__HAL_RCC_TIM13_CLK_ENABLE();
	#ifdef TIM13_SEPERATE_UP_CC_IT
				HAL_NVIC_SetPriority(TIM13_CC_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM13_CC_IRQn);
	#else
				HAL_NVIC_SetPriority(TIM13_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM13_IRQn);
	#endif
				break;
	#endif
	#ifdef TIM14
			case 14:
				if (TIM14_Freq_Locked)
					return;
				TIM14_Freq_Locked = true;
				__HAL_RCC_TIM14_CLK_ENABLE();
	#ifdef TIM14_SEPERATE_UP_CC_IT
				HAL_NVIC_SetPriority(TIM14_CC_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM14_CC_IRQn);
	#else
				HAL_NVIC_SetPriority(TIM14_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(TIM14_IRQn);
	#endif
				break;
	#endif
			}
		}

		TIM_HandleTypeDef TIM_HandleStruct = {0};
		TIM_HandleStruct.Instance = TIM;

		//set prescaler
		unsigned int clockFrequency = HAL_RCC_GetSysClockFreq();
		TIM_HandleStruct.Init.Prescaler = clockFrequency / minFrequency;
				
		//set mode
		TIM_HandleStruct.Init.CounterMode = TIM_COUNTERMODE_UP;
		TIM_HandleStruct.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  		TIM_HandleStruct.Init.Period = 0xFFFF;
  		TIM_HandleStruct.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
		
		//init
		HAL_TIM_Base_Init(&TIM_HandleStruct);

		//configure clock
		TIM_ClockConfigTypeDef TIM_ClockConfigStruct = {0};
		TIM_ClockConfigStruct.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		HAL_TIM_ConfigClockSource(&TIM_HandleStruct, &TIM_ClockConfigStruct);

		if (direction == HardwareAbstraction::Out)
		{
			TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
  			TIM_BreakDeadTimeConfigTypeDef TIM_BreakDeadTimeConfigStruct = {0};

			TIM_OC_InitStruct.OCMode = TIM_OCMODE_PWM1;
			TIM_OC_InitStruct.Pulse = 0;
			TIM_OC_InitStruct.OCPolarity = TIM_OCPOLARITY_HIGH;
			TIM_OC_InitStruct.OCNPolarity = TIM_OCNPOLARITY_HIGH;
			TIM_OC_InitStruct.OCFastMode = TIM_OCFAST_DISABLE;
			TIM_OC_InitStruct.OCIdleState = TIM_OCIDLESTATE_RESET;
			TIM_OC_InitStruct.OCNIdleState = TIM_OCNIDLESTATE_RESET;
			HAL_TIM_PWM_ConfigChannel(&TIM_HandleStruct, &TIM_OC_InitStruct, TIM_Channel);
			
			TIM_BreakDeadTimeConfigStruct.OffStateRunMode = TIM_OSSR_DISABLE;
			TIM_BreakDeadTimeConfigStruct.OffStateIDLEMode = TIM_OSSI_DISABLE;
			TIM_BreakDeadTimeConfigStruct.LockLevel = TIM_LOCKLEVEL_OFF;
			TIM_BreakDeadTimeConfigStruct.DeadTime = 0;
			TIM_BreakDeadTimeConfigStruct.BreakState = TIM_BREAK_DISABLE;
			TIM_BreakDeadTimeConfigStruct.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
			TIM_BreakDeadTimeConfigStruct.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
			HAL_TIMEx_ConfigBreakDeadTime(&TIM_HandleStruct, &TIM_BreakDeadTimeConfigStruct);
		}
		else
		{
			TIM_IC_InitTypeDef TIM_IC_InitStruct = {0};

			TIM_IC_InitStruct.ICPolarity = (TIM_Channel == TIM_CHANNEL_1 || TIM_Channel == TIM_CHANNEL_3)? TIM_INPUTCHANNELPOLARITY_RISING : TIM_INPUTCHANNELPOLARITY_FALLING;
			TIM_IC_InitStruct.ICSelection = TIM_ICSELECTION_DIRECTTI;
			TIM_IC_InitStruct.ICPrescaler = TIM_ICPSC_DIV1;
			TIM_IC_InitStruct.ICFilter = 0;

			HAL_TIM_IC_ConfigChannel(&TIM_HandleStruct, &TIM_IC_InitStruct, TIM_Channel);

			TIM_IC_InitStruct.ICPolarity = (TIM_Channel == TIM_CHANNEL_1 || TIM_Channel == TIM_CHANNEL_3)? TIM_INPUTCHANNELPOLARITY_FALLING : TIM_INPUTCHANNELPOLARITY_RISING;
			TIM_IC_InitStruct.ICSelection = TIM_ICSELECTION_INDIRECTTI;

			switch(TIM_Channel)
			{
				case TIM_CHANNEL_1:
					HAL_TIM_IC_ConfigChannel(&TIM_HandleStruct, &TIM_IC_InitStruct, TIM_CHANNEL_2);
					break;
				case TIM_CHANNEL_2:
					HAL_TIM_IC_ConfigChannel(&TIM_HandleStruct, &TIM_IC_InitStruct, TIM_CHANNEL_1);
					break;
				case TIM_CHANNEL_3:
					HAL_TIM_IC_ConfigChannel(&TIM_HandleStruct, &TIM_IC_InitStruct, TIM_CHANNEL_4);
					break;
				case TIM_CHANNEL_4:
					HAL_TIM_IC_ConfigChannel(&TIM_HandleStruct, &TIM_IC_InitStruct, TIM_CHANNEL_3);
					break;
			}
		}
	}
		
	PwmValue Stm32HalPwmService::ReadPin(unsigned short pin)
	{
		PwmValue value = PwmValue();
		if (pin == 0)
			return value;
		
		unsigned short TIM_Channel;
		unsigned char TimNum = 0;
		TIM_TypeDef *TIM;
		Stm32HalPwmPinToTimAndChannel(pin, &TimNum, &TIM, &TIM_Channel);

		unsigned char timerMinus1 = 0;;
		unsigned char iCMinus1 = 0;
		unsigned char iCMinus1Neg = 0;
		
		switch(TIM_Channel)
		{
			case TIM_CHANNEL_1:
				iCMinus1 = 0;
				iCMinus1Neg = 1;
				break;
			case TIM_CHANNEL_2:
				iCMinus1 = 1;
				iCMinus1Neg = 2;
				break;
			case TIM_CHANNEL_3:
				iCMinus1 = 2;
				iCMinus1Neg = 3;
				break;
			case TIM_CHANNEL_4:
				iCMinus1 = 3;
				iCMinus1Neg = 2;
				break;
		}

		switch (TimNum)
		{
#ifdef TIM1			
			case 1:
				timerMinus1 = 0;
				break;
#endif
#ifdef TIM2			
			case 2:
				timerMinus1 = 1;
				break;
#endif
#ifdef TIM3			
			case 3:
				timerMinus1 = 2;
				break;
#endif
#ifdef TIM4			
			case 4:
				timerMinus1 = 3;
				break;
#endif
#ifdef TIM5			
			case 5:
				timerMinus1 = 4;
				break;
#endif
#ifdef TIM6			
			case 6:
				timerMinus1 = 5;
				break;
#endif
#ifdef TIM7			
			case 7:
				timerMinus1 = 6;
				break;
#endif
#ifdef TIM8			
			case 8:
				timerMinus1 = 7;
				break;
#endif
#ifdef TIM9			
			case 9:
				timerMinus1 = 8;
				break;
#endif
#ifdef TIM10			
			case 10:
				timerMinus1 = 9;
				break;
#endif
#ifdef TIM11			
			case 11:
				timerMinus1 = 10;
				break;
#endif
#ifdef TIM12			
			case 12:
				timerMinus1 = 11;
				break;
#endif
#ifdef TIM13			
			case 13:
				timerMinus1 = 12;
				break;
#endif
#ifdef TIM14			
			case 14:
				timerMinus1 = 13;
				break;
#endif
		}

		int pulseTick = (int)_currCC[timerMinus1][iCMinus1Neg] - _currCC[timerMinus1][iCMinus1];
		if(pulseTick < 0)
			pulseTick = (int)_currCC[timerMinus1][iCMinus1Neg] - _prevCC[timerMinus1][iCMinus1];
		if (pulseTick < 0)
			pulseTick = ((int)_currCC[timerMinus1][iCMinus1Neg] - _currCC[timerMinus1][iCMinus1]) + 65535;
		unsigned short periodTick = _currCC[timerMinus1][iCMinus1] - _prevCC[timerMinus1][iCMinus1];
				
		float ticksPerSecond = (HAL_RCC_GetSysClockFreq() / ((float)(TIM->PSC)));

		value.PulseWidth = pulseTick * ticksPerSecond;
		value.Period = periodTick * ticksPerSecond;
		
		return value;
	}
	
	void Stm32HalPwmService::WritePin(unsigned short pin, HardwareAbstraction::PwmValue value)
	{
		if (pin == 0)
			return;
		
		unsigned short TIM_Channel;
		unsigned char TimNum = 0;
		TIM_TypeDef *TIM;
		Stm32HalPwmPinToTimAndChannel(pin, &TimNum, &TIM, &TIM_Channel);
				
		if (!(
#ifdef TIM1
			(TimNum == 1 && (TIM1_Freq_Locked || TIM1_Input))
#endif
#ifdef TIM2
			|| (TimNum == 2 && (TIM2_Freq_Locked || TIM2_Input)) 
#endif
#ifdef TIM3
			|| (TimNum == 3 && (TIM3_Freq_Locked || TIM1_Input)) 
#endif
#ifdef TIM4
			|| (TimNum == 4 && (TIM4_Freq_Locked || TIM4_Input)))
#endif
#ifdef TIM5
			|| (TimNum == 5 && (TIM5_Freq_Locked || TIM5_Input)))
#endif
#ifdef TIM6
			|| (TimNum == 6 && (TIM6_Freq_Locked || TIM6_Input)))
#endif
#ifdef TIM7
			|| (TimNum == 7 && (TIM7_Freq_Locked || TIM7_Input)))
#endif
#ifdef TIM8
			|| (TimNum == 8 && (TIM8_Freq_Locked || TIM8_Input)))
#endif
#ifdef TIM9
			|| (TimNum == 9 && (TIM9_Freq_Locked || TIM9_Input)))
#endif
#ifdef TIM10
			|| (TimNum == 10 && (TIM10_Freq_Locked || TIM10_Input)))
#endif
#ifdef TIM11
			|| (TimNum == 11 && (TIM11_Freq_Locked || TIM11_Input)))
#endif
#ifdef TIM12
			|| (TimNum == 12 && (TIM12_Freq_Locked || TIM12_Input)))
#endif
#ifdef TIM13
			|| (TimNum == 13 && (TIM13_Freq_Locked || TIM13_Input)))
#endif
#ifdef TIM14
			|| (TimNum == 14 && (TIM14_Freq_Locked || TIM14_Input)))
#endif
			)
		{
			//Set period
			unsigned int clockFrequency = HAL_RCC_GetSysClockFreq();
			TIM->PSC = std::max(1, (int)ceil((clockFrequency * value.Period) / 65535));
			TIM->ARR = (int)ceil((clockFrequency * value.Period) / TIM->PSC);
		}

		//set pulse width
		switch (TIM_Channel)
		{
		case TIM_CHANNEL_1:
			TIM->CCR1 = TIM->ARR * (value.PulseWidth / value.Period);
			break;
		case TIM_CHANNEL_2:
			TIM->CCR2 = TIM->ARR * (value.PulseWidth / value.Period);
			break;
		case TIM_CHANNEL_3:
			TIM->CCR3 = TIM->ARR * (value.PulseWidth / value.Period);
			break;
		case TIM_CHANNEL_4:
			TIM->CCR4 = TIM->ARR * (value.PulseWidth / value.Period);
		}
	}
	
#ifdef TIM1
	void Stm32HalPwmService::InterruptTim1()
	{
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
	
#ifdef TIM2
	void Stm32HalPwmService::InterruptTim2()
	{
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
	
#ifdef TIM3
	void Stm32HalPwmService::InterruptTim3()
	{
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
	
#ifdef TIM4
	void Stm32HalPwmService::InterruptTim4()
	{
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

#ifdef TIM5
	void Stm32HalPwmService::InterruptTim5()
	{
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

#ifdef TIM6
	void Stm32HalPwmService::InterruptTIM6()
	{
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

#ifdef TIM7
	void Stm32HalPwmService::InterruptTIM7()
	{
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

#ifdef TIM8
	void Stm32HalPwmService::InterruptTIM8()
	{
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

#ifdef TIM9
	void Stm32HalPwmService::InterruptTIM9()
	{
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

#ifdef TIM10
	void Stm32HalPwmService::InterruptTIM10()
	{
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

#ifdef TIM11
	void Stm32HalPwmService::InterruptTIM11()
	{
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

#ifdef TIM12
	void Stm32HalPwmService::InterruptTIM12()
	{
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

#ifdef TIM13
	void Stm32HalPwmService::InterruptTIM13()
	{
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

#ifdef TIM14
	void Stm32HalPwmService::InterruptTIM14()
	{
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