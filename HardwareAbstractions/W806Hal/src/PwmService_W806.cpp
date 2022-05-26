#include "PwmService_W806.h"
#include "wm_regs.h"
#include <cmath>

#define PWM        ((PWM_TypeDef *)PWM_BASE)

#ifdef PWMSERVICE_W806_H
namespace EmbeddedIOServices
{
	//still need to work on input
	void PwmService_W806::InitPin(pwmpin_t pin, PinDirection direction, uint16_t minFreqeuncy)
	{
		const uint8_t channel = PinToChannel(pin);

		if(channel == 4)
			PWM->CH4CR2 |= 1 << PWM_CH4CR2_CNTMODE_Pos;
		else
			PWM->CR |= 1 << (PWM_CR_CNTMODE_Pos + channel);
			
		if(channel == 4)
        	PWM->CH4CR2 = (PWM->CH4CR2 & ~PWM_CH4CR2_CNTTYPE)  | ((direction == Out? 1 : 0) << PWM_CH4CR2_CNTTYPE_Pos);
		else
			PWM->CR = (PWM->CR & ~(0x3 << (PWM_CR_CNTTYPE0_Pos + channel * 2))) | ((direction == Out? 1 : 0) << (PWM_CR_CNTTYPE0_Pos + channel * 2));
	}
	PwmValue PwmService_W806::ReadPin(pwmpin_t pin)
	{
		
	}
	void PwmService_W806::WritePin(pwmpin_t pin, PwmValue value)
	{
		const uint8_t apbclk = (0xFF & ((RCC_TypeDef *)RCC_BASE)->CLK_DIV) / (0xFF & (((RCC_TypeDef *)RCC_BASE)->CLK_DIV >> 16));
		const uint8_t channel = PinToChannel(pin);
		uint16_t prescaler = std::ceil(value.Period * apbclk * (1000000.0f / 256));
		if(prescaler == 0)
			return;// frequency too high
		const uint8_t period = std::ceil(value.Period * apbclk * (1000000.0f / prescaler));
		const uint8_t pulse = std::ceil(value.PulseWidth * apbclk * (1000000.0f / prescaler));
		prescaler -= 1;
		
		switch(channel)
		{
			case 0:
				PWM->CLKDIV01 = (PWM->CLKDIV01 & ~PWM_CLKDIV01_CH0_Msk) | (prescaler << PWM_CLKDIV01_CH0_Pos);
				PWM->PERIOD = (PWM->PERIOD & ~PWM_PERIOD_CH0_Msk) | (period << PWM_PERIOD_CH0_Pos);
				PWM->CMPDAT = (PWM->CMPDAT & ~PWM_CMPDAT_CH0_Msk) | (pulse << PWM_CMPDAT_CH0_Pos);
				break;
			case 1:
				PWM->CLKDIV01 = (PWM->CLKDIV01 & ~PWM_CLKDIV01_CH1_Msk) | (prescaler << PWM_CLKDIV01_CH1_Pos);
				PWM->PERIOD = (PWM->PERIOD & ~PWM_PERIOD_CH1_Msk) | (period << PWM_PERIOD_CH1_Pos);
				PWM->CMPDAT = (PWM->CMPDAT & ~PWM_CMPDAT_CH1_Msk) | (pulse << PWM_CMPDAT_CH1_Pos);
				break;
			case 2:
				PWM->CLKDIV23 = (PWM->CLKDIV23 & ~PWM_CLKDIV23_CH2_Msk) | (prescaler << PWM_CLKDIV23_CH2_Pos);
				PWM->PERIOD = (PWM->PERIOD & ~PWM_PERIOD_CH2_Msk) | (period << PWM_PERIOD_CH2_Pos);
				PWM->CMPDAT = (PWM->CMPDAT & ~PWM_CMPDAT_CH2_Msk) | (pulse << PWM_CMPDAT_CH2_Pos);
				break;
			case 3:
				PWM->CLKDIV23 = (PWM->CLKDIV23 & ~PWM_CLKDIV23_CH3_Msk) | (prescaler << PWM_CLKDIV23_CH3_Pos);
				PWM->PERIOD = (PWM->PERIOD & ~PWM_PERIOD_CH3_Msk) | (period << PWM_PERIOD_CH3_Pos);
				PWM->CMPDAT = (PWM->CMPDAT & ~PWM_CMPDAT_CH3_Msk) | (pulse << PWM_CMPDAT_CH3_Pos);
				break;
			case 4:
				PWM->CH4CR1 = (PWM->CH4CR1 & (~PWM_CH4CR1_DIV_Msk | PWM_CH4CR1_PRD_Msk)) | (prescaler << PWM_CH4CR1_DIV_Pos) | (period << PWM_CH4CR1_PRD_Pos);
        		PWM->CH4CR2 = (PWM->CH4CR2 & ~PWM_CH4CR2_CMP_Msk) | (pulse << PWM_CH4CR2_CMP_Pos);
				break;
		}
	}
	uint8_t PwmService_W806::PinToChannel(pwmpin_t pin)
	{
		switch(pin)
		{
			case 2:
			case 10:
			case 32:
			case 44:
			case 51:
				return 0;
			case 3:
			case 11:
			case 33:
			case 45:
			case 52:
				return 1;
			case 0:
			case 12:
			case 34:
			case 46:
			case 56:
				return 2;
			case 1:
			case 13:
			case 35:
			case 47:
			case 57:
				return 3;
			case 4:
			case 7:
			case 14:
			case 48:
			case 58:
				return 4;
		}
	}
}
#endif
