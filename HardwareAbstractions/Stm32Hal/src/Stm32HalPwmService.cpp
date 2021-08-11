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
        TimerIndex Index;
        TIM_TypeDef *TIM;
        uint16_t TIM_Channel;
    };

	TimAndChannel Stm32HalPwmPinToTimAndChannel(uint16_t pin)
	{
		switch (pin)
		{
#ifdef PWM_TIM1_CC1
		case PWM_TIM1_CC1 : 
            return {TimerIndex::Index1, TIM1, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM1_CC2
		case PWM_TIM1_CC2 : 
            return {TimerIndex::Index1, TIM1, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM1_CC3
		case PWM_TIM1_CC3 : 
            return {TimerIndex::Index1, TIM1, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM1_CC4
		case PWM_TIM1_CC4 : 
            return {TimerIndex::Index1, TIM1, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM2_CC1
		case PWM_TIM2_CC1 : 
            return {TimerIndex::Index2, TIM2, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM2_CC2
		case PWM_TIM2_CC2 : 
            return {TimerIndex::Index2, TIM2, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM2_CC3
		case PWM_TIM2_CC3 : 
            return {TimerIndex::Index2, TIM2, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM2_CC4
		case PWM_TIM2_CC4 : 
            return {TimerIndex::Index2, TIM2, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM3_CC1
		case PWM_TIM3_CC1 : 
            return {TimerIndex::Index3, TIM3, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM3_CC2
		case PWM_TIM3_CC2 : 
            return {TimerIndex::Index3, TIM3, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM3_CC3
		case PWM_TIM3_CC3 : 
            return {TimerIndex::Index3, TIM3, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM3_CC4
		case PWM_TIM3_CC4 : 
            return {TimerIndex::Index3, TIM3, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM4_CC1
		case PWM_TIM4_CC1 : 
            return {TimerIndex::Index4, TIM4, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM4_CC2
		case PWM_TIM4_CC2 : 
            return {TimerIndex::Index4, TIM4, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM4_CC3
		case PWM_TIM4_CC3 : 
            return {TimerIndex::Index4, TIM4, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM4_CC4
		case PWM_TIM4_CC4 : 
            return {TimerIndex::Index4, TIM4, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM5_CC1
		case PWM_TIM5_CC1 : 
            return {TimerIndex::Index5, TIM5, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM5_CC2
		case PWM_TIM5_CC2 : 
            return {TimerIndex::Index5, TIM5, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM5_CC3
		case PWM_TIM5_CC3 : 
            return {TimerIndex::Index5, TIM5, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM5_CC4
		case PWM_TIM5_CC4 : 
            return {TimerIndex::Index5, TIM5, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM6_CC1
		case PWM_TIM6_CC1 : 
            return {TimerIndex::Index6, TIM6, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM6_CC2
		case PWM_TIM6_CC2 : 
            return {TimerIndex::Index6, TIM6, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM6_CC3
		case PWM_TIM6_CC3 : 
            return {TimerIndex::Index6, TIM6, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM6_CC4
		case PWM_TIM6_CC4 : 
            return {TimerIndex::Index6, TIM6, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM7_CC1
		case PWM_TIM7_CC1 : 
            return {TimerIndex::Index7, TIM7, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM7_CC2
		case PWM_TIM7_CC2 : 
            return {TimerIndex::Index7, TIM7, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM7_CC3
		case PWM_TIM7_CC3 : 
            return {TimerIndex::Index7, TIM7, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM7_CC4
		case PWM_TIM7_CC4 : 
            return {TimerIndex::Index7, TIM7, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM8_CC1
		case PWM_TIM8_CC1 : 
            return {TimerIndex::Index8, TIM8, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM8_CC2
		case PWM_TIM8_CC2 : 
            return {TimerIndex::Index8, TIM8, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM8_CC3
		case PWM_TIM8_CC3 : 
            return {TimerIndex::Index8, TIM8, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM8_CC4
		case PWM_TIM8_CC4 : 
            return {TimerIndex::Index8, TIM8, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM9_CC1
		case PWM_TIM9_CC1 : 
            return {TimerIndex::Index9, TIM9, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM9_CC2
		case PWM_TIM9_CC2 : 
            return {TimerIndex::Index9, TIM9, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM9_CC3
		case PWM_TIM9_CC3 : 
            return {TimerIndex::Index9, TIM9, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM9_CC4
		case PWM_TIM9_CC4 : 
            return {TimerIndex::Index9, TIM9, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM10_CC1
		case PWM_TIM10_CC1 : 
            return {TimerIndex::Index10, TIM10, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM10_CC2
		case PWM_TIM10_CC2 : 
            return {TimerIndex::Index10, TIM10, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM10_CC3
		case PWM_TIM10_CC3 : 
            return {TimerIndex::Index10, TIM10, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM10_CC4
		case PWM_TIM10_CC4 : 
            return {TimerIndex::Index10, TIM10, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM11_CC1
		case PWM_TIM11_CC1 : 
            return {TimerIndex::Index11, TIM11, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM11_CC2
		case PWM_TIM11_CC2 : 
            return {TimerIndex::Index11, TIM11, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM11_CC3
		case PWM_TIM11_CC3 : 
            return {TimerIndex::Index11, TIM11, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM11_CC4
		case PWM_TIM11_CC4 : 
            return {TimerIndex::Index11, TIM11, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM12_CC1
		case PWM_TIM12_CC1 : 
            return {TimerIndex::Index12, TIM12, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM12_CC2
		case PWM_TIM12_CC2 : 
            return {TimerIndex::Index12, TIM12, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM12_CC3
		case PWM_TIM12_CC3 : 
            return {TimerIndex::Index12, TIM12, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM12_CC4
		case PWM_TIM12_CC4 : 
            return {TimerIndex::Index12, TIM12, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM13_CC1
		case PWM_TIM13_CC1 : 
            return {TimerIndex::Index13, TIM13, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM13_CC2
		case PWM_TIM13_CC2 : 
            return {TimerIndex::Index13, TIM13, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM13_CC3
		case PWM_TIM13_CC3 : 
            return {TimerIndex::Index13, TIM13, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM13_CC4
		case PWM_TIM13_CC4 : 
            return {TimerIndex::Index13, TIM13, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM14_CC1
		case PWM_TIM14_CC1 : 
            return {TimerIndex::Index14, TIM14, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM14_CC2
		case PWM_TIM14_CC2 : 
            return {TimerIndex::Index14, TIM14, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM14_CC3
		case PWM_TIM14_CC3 : 
            return {TimerIndex::Index14, TIM14, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM14_CC4
		case PWM_TIM14_CC4 : 
            return {TimerIndex::Index14, TIM14, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM15_CC1
		case PWM_TIM15_CC1 : 
            return {TimerIndex::Index15, TIM15, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM15_CC2
		case PWM_TIM15_CC2 : 
            return {TimerIndex::Index15, TIM15, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM15_CC3
		case PWM_TIM15_CC3 : 
            return {TimerIndex::Index15, TIM15, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM15_CC4
		case PWM_TIM15_CC4 : 
            return {TimerIndex::Index15, TIM15, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM16_CC1
		case PWM_TIM16_CC1 : 
            return {TimerIndex::Index16, TIM16, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM16_CC2
		case PWM_TIM16_CC2 : 
            return {TimerIndex::Index16, TIM16, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM16_CC3
		case PWM_TIM16_CC3 : 
            return {TimerIndex::Index16, TIM16, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM16_CC4
		case PWM_TIM16_CC4 : 
            return {TimerIndex::Index16, TIM16, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM17_CC1
		case PWM_TIM17_CC1 : 
            return {TimerIndex::Index17, TIM17, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM17_CC2
		case PWM_TIM17_CC2 : 
            return {TimerIndex::Index17, TIM17, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM17_CC3
		case PWM_TIM17_CC3 : 
            return {TimerIndex::Index17, TIM17, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM17_CC4
		case PWM_TIM17_CC4 : 
            return {TimerIndex::Index17, TIM17, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM18_CC1
		case PWM_TIM18_CC1 : 
            return {TimerIndex::Index18, TIM18, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM18_CC2
		case PWM_TIM18_CC2 : 
            return {TimerIndex::Index18, TIM18, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM18_CC3
		case PWM_TIM18_CC3 : 
            return {TimerIndex::Index18, TIM18, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM18_CC4
		case PWM_TIM18_CC4 : 
            return {TimerIndex::Index18, TIM18, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM19_CC1
		case PWM_TIM19_CC1 : 
            return {TimerIndex::Index19, TIM19, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM19_CC2
		case PWM_TIM19_CC2 : 
            return {TimerIndex::Index19, TIM19, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM19_CC3
		case PWM_TIM19_CC3 : 
            return {TimerIndex::Index19, TIM19, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM19_CC4
		case PWM_TIM19_CC4 : 
            return {TimerIndex::Index19, TIM19, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM20_CC1
		case PWM_TIM20_CC1 : 
            return {TimerIndex::Index20, TIM20, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM20_CC2
		case PWM_TIM20_CC2 : 
            return {TimerIndex::Index20, TIM20, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM20_CC3
		case PWM_TIM20_CC3 : 
            return {TimerIndex::Index20, TIM20, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM20_CC4
		case PWM_TIM20_CC4 : 
            return {TimerIndex::Index20, TIM20, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM21_CC1
		case PWM_TIM21_CC1 : 
            return {TimerIndex::Index21, TIM21, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM21_CC2
		case PWM_TIM21_CC2 : 
            return {TimerIndex::Index21, TIM21, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM21_CC3
		case PWM_TIM21_CC3 : 
            return {TimerIndex::Index21, TIM21, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM21_CC4
		case PWM_TIM21_CC4 : 
            return {TimerIndex::Index21, TIM21, TIM_CHANNEL_4};
#endif
#ifdef PWM_TIM22_CC1
		case PWM_TIM22_CC1 : 
            return {TimerIndex::Index22, TIM22, TIM_CHANNEL_1};
#endif
#ifdef PWM_TIM22_CC2
		case PWM_TIM22_CC2 : 
            return {TimerIndex::Index22, TIM22, TIM_CHANNEL_2};
#endif
#ifdef PWM_TIM22_CC3
		case PWM_TIM22_CC3 : 
            return {TimerIndex::Index22, TIM22, TIM_CHANNEL_3};
#endif
#ifdef PWM_TIM22_CC4
		case PWM_TIM22_CC4 : 
            return {TimerIndex::Index22, TIM22, TIM_CHANNEL_4};
#endif
		}

        //satisfy the compile warnings. should probably check this after calling the function to make sure we actually returned something
        return TimAndChannel();
	}
		
	void Stm32HalPwmService::InitPin(uint16_t pin, PinDirection direction, uint16_t minFrequency)
	{
		if (pin == 0)
			return;
					
		TimAndChannel timAndChannel = Stm32HalPwmPinToTimAndChannel(pin);
				
		EnableTimerClock(timAndChannel.Index);

		//set prescaler
		uint32_t prescaler = (HAL_RCC_GetSysClockFreq() / minFrequency) / 65535;
		
		if(direction == In)
		{		
			if (_timFrequencyLocked[timAndChannel.Index] && timAndChannel.TIM->PSC != prescaler)
				return;
			_timFrequencyLocked[timAndChannel.Index] = true;
			EnableTimerInterrupts(timAndChannel.Index);
		}
		
		TIM_HandleTypeDef TIM_Handle = TimInit(timAndChannel.Index, prescaler, static_cast<uint16_t>(ceil(minFrequency / (prescaler+1))));

		EnableGPIOClock(pin);

		if (direction == Out)
		{
			TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};

			TIM_OC_InitStruct.OCMode = TIM_OCMODE_PWM1;
			TIM_OC_InitStruct.Pulse = 32767;
			TIM_OC_InitStruct.OCPolarity = TIM_OCPOLARITY_HIGH;
			TIM_OC_InitStruct.OCFastMode = TIM_OCFAST_ENABLE;
			TIM_OC_InitStruct.OCIdleState = TIM_OCIDLESTATE_RESET;
			HAL_TIM_PWM_ConfigChannel(&TIM_Handle, &TIM_OC_InitStruct, timAndChannel.TIM_Channel);

			GPIO_InitTypeDef GPIO_InitStruct = {0};
			GPIO_InitStruct.Pin = PinToGPIO_Pin(pin);
			GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
			HAL_GPIO_Init(PinToGPIO(pin), &GPIO_InitStruct);

			HAL_TIM_PWM_Start(&TIM_Handle, timAndChannel.TIM_Channel);
		}
		else
		{
			TIM_IC_InitTypeDef TIM_IC_InitStruct = {0};

			TIM_IC_InitStruct.ICPolarity = (timAndChannel.TIM_Channel == TIM_CHANNEL_1 || timAndChannel.TIM_Channel == TIM_CHANNEL_3)? TIM_INPUTCHANNELPOLARITY_RISING : TIM_INPUTCHANNELPOLARITY_FALLING;
			TIM_IC_InitStruct.ICSelection = TIM_ICSELECTION_DIRECTTI;
			TIM_IC_InitStruct.ICPrescaler = TIM_ICPSC_DIV1;
			TIM_IC_InitStruct.ICFilter = 0;

			HAL_TIM_IC_ConfigChannel(&TIM_Handle, &TIM_IC_InitStruct, timAndChannel.TIM_Channel);

			TIM_IC_InitStruct.ICPolarity = (timAndChannel.TIM_Channel == TIM_CHANNEL_1 || timAndChannel.TIM_Channel == TIM_CHANNEL_3)? TIM_INPUTCHANNELPOLARITY_FALLING : TIM_INPUTCHANNELPOLARITY_RISING;
			TIM_IC_InitStruct.ICSelection = TIM_ICSELECTION_INDIRECTTI;

			switch(timAndChannel.TIM_Channel)
			{
				case TIM_CHANNEL_1:
					HAL_TIM_IC_ConfigChannel(&TIM_Handle, &TIM_IC_InitStruct, TIM_CHANNEL_2);
					__HAL_TIM_ENABLE_IT(&TIM_Handle, TIM_IT_CC1);
					__HAL_TIM_ENABLE_IT(&TIM_Handle, TIM_IT_CC2);
					HAL_TIM_IC_Start_IT(&TIM_Handle, TIM_CHANNEL_2);
					break;
				case TIM_CHANNEL_2:
					HAL_TIM_IC_ConfigChannel(&TIM_Handle, &TIM_IC_InitStruct, TIM_CHANNEL_1);
					__HAL_TIM_ENABLE_IT(&TIM_Handle, TIM_IT_CC1);
					__HAL_TIM_ENABLE_IT(&TIM_Handle, TIM_IT_CC2);
					HAL_TIM_IC_Start_IT(&TIM_Handle, TIM_CHANNEL_1);
					break;
				case TIM_CHANNEL_3:
					HAL_TIM_IC_ConfigChannel(&TIM_Handle, &TIM_IC_InitStruct, TIM_CHANNEL_4);
					__HAL_TIM_ENABLE_IT(&TIM_Handle, TIM_IT_CC3);
					__HAL_TIM_ENABLE_IT(&TIM_Handle, TIM_IT_CC4);
					HAL_TIM_IC_Start_IT(&TIM_Handle, TIM_CHANNEL_4);
					break;
				case TIM_CHANNEL_4:
					HAL_TIM_IC_ConfigChannel(&TIM_Handle, &TIM_IC_InitStruct, TIM_CHANNEL_3);
					__HAL_TIM_ENABLE_IT(&TIM_Handle, TIM_IT_CC3);
					__HAL_TIM_ENABLE_IT(&TIM_Handle, TIM_IT_CC4);
					HAL_TIM_IC_Start_IT(&TIM_Handle, TIM_CHANNEL_3);
					break;
			}
			HAL_TIM_IC_Start_IT(&TIM_Handle, timAndChannel.TIM_Channel);
			
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

		int32_t pulseTick = static_cast<int32_t>(_timCurrentCC[timAndChannel.Index][iCMinus1Neg]) - _timCurrentCC[timAndChannel.Index][iCMinus1];
		if(pulseTick < 0)
			pulseTick = static_cast<int32_t>(_timCurrentCC[timAndChannel.Index][iCMinus1Neg]) - _timPreviousCC[timAndChannel.Index][iCMinus1];
		if (pulseTick < 0)
			pulseTick = (static_cast<int32_t>(_timCurrentCC[timAndChannel.Index][iCMinus1Neg]) - _timCurrentCC[timAndChannel.Index][iCMinus1]) + 65535;
		uint16_t periodTick = _timCurrentCC[timAndChannel.Index][iCMinus1] - _timPreviousCC[timAndChannel.Index][iCMinus1];
				
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
				
		if (!_timFrequencyLocked[timAndChannel.Index])
		{
			//Set period
			const uint32_t clockFrequency = HAL_RCC_GetSysClockFreq();
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
}