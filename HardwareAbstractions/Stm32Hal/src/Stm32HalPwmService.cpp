#include "Stm32HalConf.h"
#include "Stm32HalPwmService.h"
#include <algorithm>
#include <math.h>
#include "Stm32HalTimer.h"

using namespace EmbeddedIOServices;

namespace Stm32
{
    struct TimAndChannel
    {
        uint8_t TimNum;
        TIM_TypeDef *TIM;
        uint16_t TIM_Channel;
    };

	TimAndChannel Stm32HalPwmPinToTimAndChannel(uint16_t pin)
	{
		switch (pin)
		{
#ifdef PWM_TIM1_CC1
		case PWM_TIM1_CC1 : 
            return {1, TIM1, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM1_CC2
		case PWM_TIM1_CC2 : 
            return {1, TIM1, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM1_CC3
		case PWM_TIM1_CC3 : 
            return {1, TIM1, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM1_CC4
		case PWM_TIM1_CC4 : 
            return {1, TIM1, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM2_CC1
		case PWM_TIM2_CC1 : 
            return {2, TIM2, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM2_CC2
		case PWM_TIM2_CC2 : 
            return {2, TIM2, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM2_CC3
		case PWM_TIM2_CC3 : 
            return {2, TIM2, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM2_CC4
		case PWM_TIM2_CC4 : 
            return {2, TIM2, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM3_CC1
		case PWM_TIM3_CC1 : 
            return {3, TIM3, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM3_CC2
		case PWM_TIM3_CC2 : 
            return {3, TIM3, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM3_CC3
		case PWM_TIM3_CC3 : 
            return {3, TIM3, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM3_CC4
		case PWM_TIM3_CC4 : 
            return {3, TIM3, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM4_CC1
		case PWM_TIM4_CC1 : 
            return {4, TIM4, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM4_CC2
		case PWM_TIM4_CC2 : 
            return {4, TIM4, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM4_CC3
		case PWM_TIM4_CC3 : 
            return {4, TIM4, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM4_CC4
		case PWM_TIM4_CC4 : 
            return {4, TIM4, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM5_CC1
		case PWM_TIM5_CC1 : 
            return {5, TIM5, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM5_CC2
		case PWM_TIM5_CC2 : 
            return {5, TIM5, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM5_CC3
		case PWM_TIM5_CC3 : 
            return {5, TIM5, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM5_CC4
		case PWM_TIM5_CC4 : 
            return {5, TIM5, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM6_CC1
		case PWM_TIM6_CC1 : 
            return {6, TIM6, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM6_CC2
		case PWM_TIM6_CC2 : 
            return {6, TIM6, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM6_CC3
		case PWM_TIM6_CC3 : 
            return {6, TIM6, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM6_CC4
		case PWM_TIM6_CC4 : 
            return {6, TIM6, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM7_CC1
		case PWM_TIM7_CC1 : 
            return {7, TIM7, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM7_CC2
		case PWM_TIM7_CC2 : 
            return {7, TIM7, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM7_CC3
		case PWM_TIM7_CC3 : 
            return {7, TIM7, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM7_CC4
		case PWM_TIM7_CC4 : 
            return {7, TIM7, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM8_CC1
		case PWM_TIM8_CC1 : 
            return {8, TIM8, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM8_CC2
		case PWM_TIM8_CC2 : 
            return {8, TIM8, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM8_CC3
		case PWM_TIM8_CC3 : 
            return {8, TIM8, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM8_CC4
		case PWM_TIM8_CC4 : 
            return {8, TIM8, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM9_CC1
		case PWM_TIM9_CC1 : 
            return {9, TIM9, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM9_CC2
		case PWM_TIM9_CC2 : 
            return {9, TIM9, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM9_CC3
		case PWM_TIM9_CC3 : 
            return {9, TIM9, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM9_CC4
		case PWM_TIM9_CC4 : 
            return {9, TIM9, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM10_CC1
		case PWM_TIM10_CC1 : 
            return {10, TIM10, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM10_CC2
		case PWM_TIM10_CC2 : 
            return {10, TIM10, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM10_CC3
		case PWM_TIM10_CC3 : 
            return {10, TIM10, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM10_CC4
		case PWM_TIM10_CC4 : 
            return {10, TIM10, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM11_CC1
		case PWM_TIM11_CC1 : 
            return {11, TIM11, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM11_CC2
		case PWM_TIM11_CC2 : 
            return {11, TIM11, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM11_CC3
		case PWM_TIM11_CC3 : 
            return {11, TIM11, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM11_CC4
		case PWM_TIM11_CC4 : 
            return {11, TIM11, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM12_CC1
		case PWM_TIM12_CC1 : 
            return {12, TIM12, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM12_CC2
		case PWM_TIM12_CC2 : 
            return {12, TIM12, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM12_CC3
		case PWM_TIM12_CC3 : 
            return {12, TIM12, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM12_CC4
		case PWM_TIM12_CC4 : 
            return {12, TIM12, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM13_CC1
		case PWM_TIM13_CC1 : 
            return {13, TIM13, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM13_CC2
		case PWM_TIM13_CC2 : 
            return {13, TIM13, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM13_CC3
		case PWM_TIM13_CC3 : 
            return {13, TIM13, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM13_CC4
		case PWM_TIM13_CC4 : 
            return {13, TIM13, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM14_CC1
		case PWM_TIM14_CC1 : 
            return {13, TIM13, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM14_CC2
		case PWM_TIM14_CC2 : 
            return {14, TIM14, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM14_CC3
		case PWM_TIM14_CC3 : 
            return {14, TIM14, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM14_CC4
		case PWM_TIM14_CC4 : 
            return {14, TIM14, TIM_CHANNEL_4};
#endif
		}
	}

	Stm32HalPwmService *HalPwmService = 0;
	
	Stm32HalPwmService::Stm32HalPwmService()
	{
		HalPwmService = this;
	}
	
	void Stm32HalPwmService::InitPin(uint16_t pin, PinDirection direction, uint16_t minFrequency)
	{
		if (pin == 0)
			return;
					
		TimAndChannel timAndChannel = Stm32HalPwmPinToTimAndChannel(pin);
				
		EnableTimerClock(timAndChannel.TimNum);

		TIM_HandleTypeDef TIM_HandleStruct = {0};
		TIM_HandleStruct.Instance = timAndChannel.TIM;

		//set prescaler
		uint32_t clockFrequency = HAL_RCC_GetSysClockFreq();
		TIM_HandleStruct.Init.Prescaler = (clockFrequency / minFrequency) / 65535;

		if(direction == In)
		{		
			switch (timAndChannel.TimNum)
			{
	#ifdef TIM1
			case 1:
				if (TIM1_Freq_Locked && TIM1->PSC != TIM_HandleStruct.Init.Prescaler)
					return;
				TIM1_Freq_Locked = true;
				break;
	#endif
	#ifdef TIM2
			case 2:
				if (TIM2_Freq_Locked && TIM2->PSC != TIM_HandleStruct.Init.Prescaler)
					return;
				if(direction == Out)
					TIM2_Freq_Locked = true;
				break;
	#endif
	#ifdef TIM3
			case 3:
				if (TIM3_Freq_Locked && TIM3->PSC != TIM_HandleStruct.Init.Prescaler)
					return;
				TIM3_Freq_Locked = true;
				break;
	#endif
	#ifdef TIM4
			case 4:
				if (TIM4_Freq_Locked && TIM4->PSC != TIM_HandleStruct.Init.Prescaler)
					return;
				TIM4_Freq_Locked = true;
				break;
	#endif
	#ifdef TIM5
			case 5:
				if (TIM5_Freq_Locked && TIM5->PSC != TIM_HandleStruct.Init.Prescaler)
					return;
				TIM5_Freq_Locked = true;
				break;
	#endif
	#ifdef TIM6
			case 6:
				if (TIM6_Freq_Locked && TIM6->PSC != TIM_HandleStruct.Init.Prescaler)
					return;
				TIM6_Freq_Locked = true;
				break;
	#endif
	#ifdef TIM7
			case 7:
				if (TIM7_Freq_Locked && TIM7->PSC != TIM_HandleStruct.Init.Prescaler)
					return;
				TIM7_Freq_Locked = true;
				break;
	#endif
	#ifdef TIM8
			case 8:
				if (TIM8_Freq_Locked && TIM8->PSC != TIM_HandleStruct.Init.Prescaler)
					return;
				TIM8_Freq_Locked = true;
				break;
	#endif
	#ifdef TIM9
			case 9:
				if (TIM9_Freq_Locked && TIM9->PSC != TIM_HandleStruct.Init.Prescaler)
					return;
				TIM9_Freq_Locked = true;
				break;
	#endif
	#ifdef TIM10
			case 10:
				if (TIM10_Freq_Locked && TIM10->PSC != TIM_HandleStruct.Init.Prescaler)
					return;
				TIM10_Freq_Locked = true;
				break;
	#endif
	#ifdef TIM11
			case 11:
				if (TIM11_Freq_Locked && TIM11->PSC != TIM_HandleStruct.Init.Prescaler)
					return;
				TIM11_Freq_Locked = true;
				break;
	#endif
	#ifdef TIM12
			case 12:
				if (TIM12_Freq_Locked && TIM12->PSC != TIM_HandleStruct.Init.Prescaler)
					return;
				TIM12_Freq_Locked = true;
				break;
	#endif
	#ifdef TIM13
			case 13:
				if (TIM13_Freq_Locked && TIM13->PSC != TIM_HandleStruct.Init.Prescaler)
					return;
				TIM13_Freq_Locked = true;
				break;
	#endif
	#ifdef TIM14
			case 14:
				if (TIM14_Freq_Locked && TIM14->PSC != TIM_HandleStruct.Init.Prescaler)
					return;
				TIM14_Freq_Locked = true;
				break;
	#endif
			}
			EnableTimerInterrupts(timAndChannel.TimNum);
		}
		
		EnableGPIOClock(pin);
				
		//set mode
		TIM_HandleStruct.Init.CounterMode = TIM_COUNTERMODE_UP;
		TIM_HandleStruct.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		TIM_HandleStruct.Init.Period = static_cast<uint16_t>(ceil(minFrequency / (TIM_HandleStruct.Init.Prescaler+1)));
  		TIM_HandleStruct.Init.Period = 0xFFFF;
  		TIM_HandleStruct.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
		
		//init
		HAL_TIM_Base_Init(&TIM_HandleStruct);

		//configure clock
		TIM_ClockConfigTypeDef TIM_ClockConfigStruct = {0};
		TIM_ClockConfigStruct.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		HAL_TIM_ConfigClockSource(&TIM_HandleStruct, &TIM_ClockConfigStruct);

		if (direction == Out)
		{
			TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};

			TIM_OC_InitStruct.OCMode = TIM_OCMODE_PWM1;
			TIM_OC_InitStruct.Pulse = 32767;
			TIM_OC_InitStruct.OCPolarity = TIM_OCPOLARITY_HIGH;
			TIM_OC_InitStruct.OCFastMode = TIM_OCFAST_ENABLE;
			TIM_OC_InitStruct.OCIdleState = TIM_OCIDLESTATE_RESET;
			HAL_TIM_PWM_ConfigChannel(&TIM_HandleStruct, &TIM_OC_InitStruct, timAndChannel.TIM_Channel);

			GPIO_InitTypeDef GPIO_InitStruct = {0};
			GPIO_InitStruct.Pin = PinToGPIO_Pin(pin);
			GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
			HAL_GPIO_Init(PinToGPIO(pin), &GPIO_InitStruct);

			HAL_TIM_PWM_Start(&TIM_HandleStruct, timAndChannel.TIM_Channel);
		}
		else
		{
			TIM_IC_InitTypeDef TIM_IC_InitStruct = {0};

			TIM_IC_InitStruct.ICPolarity = (timAndChannel.TIM_Channel == TIM_CHANNEL_1 || timAndChannel.TIM_Channel == TIM_CHANNEL_3)? TIM_INPUTCHANNELPOLARITY_RISING : TIM_INPUTCHANNELPOLARITY_FALLING;
			TIM_IC_InitStruct.ICSelection = TIM_ICSELECTION_DIRECTTI;
			TIM_IC_InitStruct.ICPrescaler = TIM_ICPSC_DIV1;
			TIM_IC_InitStruct.ICFilter = 0;

			HAL_TIM_IC_ConfigChannel(&TIM_HandleStruct, &TIM_IC_InitStruct, timAndChannel.TIM_Channel);

			TIM_IC_InitStruct.ICPolarity = (timAndChannel.TIM_Channel == TIM_CHANNEL_1 || timAndChannel.TIM_Channel == TIM_CHANNEL_3)? TIM_INPUTCHANNELPOLARITY_FALLING : TIM_INPUTCHANNELPOLARITY_RISING;
			TIM_IC_InitStruct.ICSelection = TIM_ICSELECTION_INDIRECTTI;

			switch(timAndChannel.TIM_Channel)
			{
				case TIM_CHANNEL_1:
					HAL_TIM_IC_ConfigChannel(&TIM_HandleStruct, &TIM_IC_InitStruct, TIM_CHANNEL_2);
					__HAL_TIM_ENABLE_IT(&TIM_HandleStruct, TIM_IT_CC1);
					__HAL_TIM_ENABLE_IT(&TIM_HandleStruct, TIM_IT_CC2);
					HAL_TIM_IC_Start_IT(&TIM_HandleStruct, TIM_CHANNEL_2);
					break;
				case TIM_CHANNEL_2:
					HAL_TIM_IC_ConfigChannel(&TIM_HandleStruct, &TIM_IC_InitStruct, TIM_CHANNEL_1);
					__HAL_TIM_ENABLE_IT(&TIM_HandleStruct, TIM_IT_CC1);
					__HAL_TIM_ENABLE_IT(&TIM_HandleStruct, TIM_IT_CC2);
					HAL_TIM_IC_Start_IT(&TIM_HandleStruct, TIM_CHANNEL_1);
					break;
				case TIM_CHANNEL_3:
					HAL_TIM_IC_ConfigChannel(&TIM_HandleStruct, &TIM_IC_InitStruct, TIM_CHANNEL_4);
					__HAL_TIM_ENABLE_IT(&TIM_HandleStruct, TIM_IT_CC3);
					__HAL_TIM_ENABLE_IT(&TIM_HandleStruct, TIM_IT_CC4);
					HAL_TIM_IC_Start_IT(&TIM_HandleStruct, TIM_CHANNEL_4);
					break;
				case TIM_CHANNEL_4:
					HAL_TIM_IC_ConfigChannel(&TIM_HandleStruct, &TIM_IC_InitStruct, TIM_CHANNEL_3);
					__HAL_TIM_ENABLE_IT(&TIM_HandleStruct, TIM_IT_CC3);
					__HAL_TIM_ENABLE_IT(&TIM_HandleStruct, TIM_IT_CC4);
					HAL_TIM_IC_Start_IT(&TIM_HandleStruct, TIM_CHANNEL_3);
					break;
			}
			HAL_TIM_IC_Start_IT(&TIM_HandleStruct, timAndChannel.TIM_Channel);
			
			GPIO_InitTypeDef GPIO_InitStruct = {0};
			GPIO_InitStruct.Pin = PinToGPIO_Pin(pin);
			GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
			HAL_GPIO_Init(PinToGPIO(pin), &GPIO_InitStruct);
		}
	}
		
	PwmValue Stm32HalPwmService::ReadPin(uint16_t pin)
	{
		PwmValue value = PwmValue();
		if (pin == 0)
			return value;
		
		TimAndChannel timAndChannel = Stm32HalPwmPinToTimAndChannel(pin);

		uint8_t timerMinus1 = timAndChannel.TimNum - 1;
		uint8_t iCMinus1 = 0;
		uint8_t iCMinus1Neg = 0;
		
		switch(timAndChannel.TIM_Channel)
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

		int32_t pulseTick = static_cast<int32_t>(_currCC[timerMinus1][iCMinus1Neg]) - _currCC[timerMinus1][iCMinus1];
		if(pulseTick < 0)
			pulseTick = static_cast<int32_t>(_currCC[timerMinus1][iCMinus1Neg]) - _prevCC[timerMinus1][iCMinus1];
		if (pulseTick < 0)
			pulseTick = (static_cast<int32_t>(_currCC[timerMinus1][iCMinus1Neg]) - _currCC[timerMinus1][iCMinus1]) + 65535;
		uint16_t periodTick = _currCC[timerMinus1][iCMinus1] - _prevCC[timerMinus1][iCMinus1];
				
		float clockFactor = static_cast<float>(timAndChannel.TIM->PSC + 1) / HAL_RCC_GetSysClockFreq();

		value.PulseWidth = pulseTick * clockFactor;
		value.Period = periodTick * clockFactor;
		
		return value;
	}
	
	void Stm32HalPwmService::WritePin(uint16_t pin, PwmValue value)
	{
		if (pin == 0)
			return;
		
		TimAndChannel timAndChannel = Stm32HalPwmPinToTimAndChannel(pin);
				
		if (!(
#ifdef TIM1
			(timAndChannel.TimNum == 1 && (TIM1_Freq_Locked || TIM1_Input))
#endif
#ifdef TIM2
			|| (timAndChannel.TimNum == 2 && (TIM2_Freq_Locked || TIM2_Input)) 
#endif
#ifdef TIM3
			|| (timAndChannel.TimNum == 3 && (TIM3_Freq_Locked || TIM1_Input)) 
#endif
#ifdef TIM4
			|| (timAndChannel.TimNum == 4 && (TIM4_Freq_Locked || TIM4_Input)))
#endif
#ifdef TIM5
			|| (timAndChannel.TimNum == 5 && (TIM5_Freq_Locked || TIM5_Input)))
#endif
#ifdef TIM6
			|| (timAndChannel.TimNum == 6 && (TIM6_Freq_Locked || TIM6_Input)))
#endif
#ifdef TIM7
			|| (timAndChannel.TimNum == 7 && (TIM7_Freq_Locked || TIM7_Input)))
#endif
#ifdef TIM8
			|| (timAndChannel.TimNum == 8 && (TIM8_Freq_Locked || TIM8_Input)))
#endif
#ifdef TIM9
			|| (timAndChannel.TimNum == 9 && (TIM9_Freq_Locked || TIM9_Input)))
#endif
#ifdef TIM10
			|| (timAndChannel.TimNum == 10 && (TIM10_Freq_Locked || TIM10_Input)))
#endif
#ifdef TIM11
			|| (timAndChannel.TimNum == 11 && (TIM11_Freq_Locked || TIM11_Input)))
#endif
#ifdef TIM12
			|| (timAndChannel.TimNum == 12 && (TIM12_Freq_Locked || TIM12_Input)))
#endif
#ifdef TIM13
			|| (timAndChannel.TimNum == 13 && (TIM13_Freq_Locked || TIM13_Input)))
#endif
#ifdef TIM14
			|| (timAndChannel.TimNum == 14 && (TIM14_Freq_Locked || TIM14_Input)))
#endif
			)
		{
			//Set period
			uint32_t clockFrequency = HAL_RCC_GetSysClockFreq();
			timAndChannel.TIM->PSC = (clockFrequency * value.Period) / 65535;
			timAndChannel.TIM->ARR = static_cast<uint16_t>(ceil((clockFrequency * value.Period) / (timAndChannel.TIM->PSC+1)));
		}

		//set pulse width
		switch (timAndChannel.TIM_Channel)
		{
		case TIM_CHANNEL_1:
			timAndChannel.TIM->CCR1 = timAndChannel.TIM->ARR * (value.PulseWidth / value.Period);
			break;
		case TIM_CHANNEL_2:
			timAndChannel.TIM->CCR2 = timAndChannel.TIM->ARR * (value.PulseWidth / value.Period);
			break;
		case TIM_CHANNEL_3:
			timAndChannel.TIM->CCR3 = timAndChannel.TIM->ARR * (value.PulseWidth / value.Period);
			break;
		case TIM_CHANNEL_4:
			timAndChannel.TIM->CCR4 = timAndChannel.TIM->ARR * (value.PulseWidth / value.Period);
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