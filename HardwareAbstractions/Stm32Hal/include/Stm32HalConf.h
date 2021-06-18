#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"

#ifndef STM32HALCONF_H
#define STM32HALCONF_H

#define TIM2_CC_IRQn            TIM2_IRQn
#define TIM3_CC_IRQn            TIM3_IRQn
#define TIM4_CC_IRQn            TIM4_IRQn
#define TIM5_CC_IRQn            TIM5_IRQn
#define TIM9_CC_IRQn            TIM1_BRK_TIM9_IRQn
#define TIM10_CC_IRQn           TIM1_UP_TIM10_IRQn
#define TIM11_CC_IRQn           TIM1_TRG_COM_TIM11_IRQn

#define TIM2_CC_IRQHandler      TIM2_IRQHandler
#define TIM3_CC_IRQHandler      TIM3_IRQHandler
#define TIM4_CC_IRQHandler      TIM4_IRQHandler
#define TIM5_CC_IRQHandler      TIM5_IRQHandler
#define TIM9_CC_IRQHandler      TIM1_BRK_TIM9_IRQHandler
#define TIM10_CC_IRQHandler     TIM1_UP_TIM10_IRQHandler
#define TIM11_CC_IRQHandler     TIM1_TRG_COM_TIM11_IRQHandler

#define ADC1_VOLTAGE_CONVERSION_CONSTANT 3.3 / 4095
#define ADC1_IN0_PIN 0
#define ADC1_IN1_PIN 1
#define ADC1_IN2_PIN 2
#define ADC1_IN3_PIN 3
#define ADC1_IN4_PIN 4
#define ADC1_IN5_PIN 5
#define ADC1_IN6_PIN 6
#define ADC1_IN7_PIN 7
#define ADC1_IN8_PIN 16
#define ADC1_IN9_PIN 17
//#define ADC1_IN10_PIN 
//#define ADC1_IN11_PIN 
//#define ADC1_IN12_PIN 
//#define ADC1_IN13_PIN 
//#define ADC1_IN14_PIN 
//#define ADC1_IN15_PIN 

//#define ADC2_VOLTAGE_CONVERSION_CONSTANT
//#define ADC2_IN0_PIN 
//#define ADC2_IN1_PIN 
//#define ADC2_IN2_PIN 
//#define ADC2_IN3_PIN 
//#define ADC2_IN4_PIN 
//#define ADC2_IN5_PIN 
//#define ADC2_IN6_PIN 
//#define ADC2_IN7_PIN 
//#define ADC2_IN8_PIN 
//#define ADC2_IN9_PIN 
//#define ADC2_IN10_PIN 
//#define ADC2_IN11_PIN 
//#define ADC2_IN12_PIN 
//#define ADC2_IN13_PIN 
//#define ADC2_IN14_PIN 
//#define ADC2_IN15_PIN 

//#define ADC3_VOLTAGE_CONVERSION_CONSTANT
//#define ADC3_IN0_PIN 
//#define ADC3_IN1_PIN 
//#define ADC3_IN2_PIN 
//#define ADC3_IN3_PIN 
//#define ADC3_IN4_PIN 
//#define ADC3_IN5_PIN 
//#define ADC3_IN6_PIN 
//#define ADC3_IN7_PIN 
//#define ADC3_IN8_PIN 
//#define ADC3_IN9_PIN 
//#define ADC3_IN10_PIN 
//#define ADC3_IN11_PIN 
//#define ADC3_IN12_PIN 
//#define ADC3_IN13_PIN 
//#define ADC3_IN14_PIN 
//#define ADC3_IN15_PIN 

//#define ADC4_VOLTAGE_CONVERSION_CONSTANT
//#define ADC4_IN0_PIN 
//#define ADC4_IN1_PIN 
//#define ADC4_IN2_PIN 
//#define ADC4_IN3_PIN 
//#define ADC4_IN4_PIN 
//#define ADC4_IN5_PIN 
//#define ADC4_IN6_PIN 
//#define ADC4_IN7_PIN 
//#define ADC4_IN8_PIN 
//#define ADC4_IN9_PIN 
//#define ADC4_IN10_PIN 
//#define ADC4_IN11_PIN 
//#define ADC4_IN12_PIN 
//#define ADC4_IN13_PIN 
//#define ADC4_IN14_PIN 
//#define ADC4_IN15_PIN 

// #define PWM_TIM1_CC1
// #define PWM_TIM1_CC2
// #define PWM_TIM1_CC3
// #define PWM_TIM1_CC4

#define PWM_TIM2_CC1 0
#define PWM_TIM2_CC2 1
#define PWM_TIM2_CC3 2
#define PWM_TIM2_CC4 3

#define PWM_TIM3_CC1 6
#define PWM_TIM3_CC2 7
#define PWM_TIM3_CC3 16
#define PWM_TIM3_CC4 17

#define PWM_TIM4_CC1 22
#define PWM_TIM4_CC2 23
#define PWM_TIM4_CC3 24
#define PWM_TIM4_CC4 25

#define PWM_TIM5_CC1 8
#define PWM_TIM5_CC2 9
#define PWM_TIM5_CC3 10
#define PWM_TIM5_CC4 11

//#define PWM_TIM6_CC1 
//#define PWM_TIM6_CC2 
//#define PWM_TIM6_CC3 
//#define PWM_TIM6_CC4 

//#define PWM_TIM7_CC1 
//#define PWM_TIM7_CC2 
//#define PWM_TIM7_CC3 
//#define PWM_TIM7_CC4 

//#define PWM_TIM8_CC1 
//#define PWM_TIM8_CC2 
//#define PWM_TIM8_CC3 
//#define PWM_TIM8_CC4 

//#define PWM_TIM9_CC1 
//#define PWM_TIM9_CC2 
//#define PWM_TIM9_CC3 
//#define PWM_TIM9_CC4 

//#define PWM_TIM10_CC1 
//#define PWM_TIM10_CC2 
//#define PWM_TIM10_CC3 
//#define PWM_TIM10_CC4 

//#define PWM_TIM11_CC1 
//#define PWM_TIM11_CC2 
//#define PWM_TIM11_CC3 
//#define PWM_TIM11_CC4 

//#define PWM_TIM12_CC1 
//#define PWM_TIM12_CC2 
//#define PWM_TIM12_CC3 
//#define PWM_TIM12_CC4 

//#define PWM_TIM13_CC1 
//#define PWM_TIM13_CC2 
//#define PWM_TIM13_CC3 
//#define PWM_TIM13_CC4 

//#define PWM_TIM14_CC1 
//#define PWM_TIM14_CC2 
//#define PWM_TIM14_CC3 
//#define PWM_TIM14_CC4 

#endif