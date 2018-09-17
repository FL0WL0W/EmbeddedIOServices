#ifndef STM32F103CXDEFINES_H
#define STM32F103CXDEFINES_H

typedef struct
{
  volatile unsigned int CR;
  volatile unsigned int CFGR;
  volatile unsigned int CIR;
  volatile unsigned int APB2RSTR;
  volatile unsigned int APB1RSTR;
  volatile unsigned int AHBENR;
  volatile unsigned int APB2ENR;
  volatile unsigned int APB1ENR;
  volatile unsigned int BDCR;
  volatile unsigned int CSR;
} RCC_TypeDef;

typedef struct
{
  volatile unsigned int ISER[8];                      /*!< Offset: 0x000  Interrupt Set Enable Register           */
       unsigned int RESERVED0[24];                                   
  volatile unsigned int ICER[8];                      /*!< Offset: 0x080  Interrupt Clear Enable Register         */
       unsigned int RSERVED1[24];                                    
  volatile unsigned int ISPR[8];                      /*!< Offset: 0x100  Interrupt Set Pending Register          */
       unsigned int RESERVED2[24];                                   
  volatile unsigned int ICPR[8];                      /*!< Offset: 0x180  Interrupt Clear Pending Register        */
       unsigned int RESERVED3[24];                                   
  volatile unsigned int IABR[8];                      /*!< Offset: 0x200  Interrupt Active bit Register           */
       unsigned int RESERVED4[56];                                   
  volatile unsigned char  IP[240];                      /*!< Offset: 0x300  Interrupt Priority Register (8Bit wide) */
       unsigned int RESERVED5[644];                                  
  volatile  unsigned int STIR;                         /*!< Offset: 0xE00  Software Trigger Interrupt Register     */
}  NVIC_Type;          

typedef struct
{
  volatile unsigned short CR1;
  unsigned short  RESERVED0;
  volatile unsigned short CR2;
  unsigned short  RESERVED1;
  volatile unsigned short SMCR;
  unsigned short  RESERVED2;
  volatile unsigned short DIER;
  unsigned short  RESERVED3;
  volatile unsigned short SR;
  unsigned short  RESERVED4;
  volatile unsigned short EGR;
  unsigned short  RESERVED5;
  volatile unsigned short CCMR1;
  unsigned short  RESERVED6;
  volatile unsigned short CCMR2;
  unsigned short  RESERVED7;
  volatile unsigned short CCER;
  unsigned short  RESERVED8;
  volatile unsigned short CNT;
  unsigned short  RESERVED9;
  volatile unsigned short PSC;
  unsigned short  RESERVED10;
  volatile unsigned short ARR;
  unsigned short  RESERVED11;
  volatile unsigned short RCR;
  unsigned short  RESERVED12;
  volatile unsigned short CCR1;
  unsigned short  RESERVED13;
  volatile unsigned short CCR2;
  unsigned short  RESERVED14;
  volatile unsigned short CCR3;
  unsigned short  RESERVED15;
  volatile unsigned short CCR4;
  unsigned short  RESERVED16;
  volatile unsigned short BDTR;
  unsigned short  RESERVED17;
  volatile unsigned short DCR;
  unsigned short  RESERVED18;
  volatile unsigned short DMAR;
  unsigned short  RESERVED19;
} TIM_TypeDef;

typedef struct
{
  volatile unsigned int CRL;
  volatile unsigned int CRH;
  volatile unsigned int IDR;
  volatile unsigned int ODR;
  volatile unsigned int BSRR;
  volatile unsigned int BRR;
  volatile unsigned int LCKR;
} GPIO_TypeDef;

typedef struct
{
  volatile unsigned int SR;
  volatile unsigned int CR1;
  volatile unsigned int CR2;
  volatile unsigned int SMPR1;
  volatile unsigned int SMPR2;
  volatile unsigned int JOFR1;
  volatile unsigned int JOFR2;
  volatile unsigned int JOFR3;
  volatile unsigned int JOFR4;
  volatile unsigned int HTR;
  volatile unsigned int LTR;
  volatile unsigned int SQR1;
  volatile unsigned int SQR2;
  volatile unsigned int SQR3;
  volatile unsigned int JSQR;
  volatile unsigned int JDR1;
  volatile unsigned int JDR2;
  volatile unsigned int JDR3;
  volatile unsigned int JDR4;
  volatile unsigned int DR;
} ADC_TypeDef;

#define PERIPH_BASE           ((unsigned int)0x40000000)

#define SCS_BASE              (0xE000E000)
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x20000)

#define RCC_BASE              (AHBPERIPH_BASE + 0x1000)
#define NVIC_BASE             (SCS_BASE +  0x0100)
#define GPIOA_BASE            (APB2PERIPH_BASE + 0x0800)
#define GPIOB_BASE            (APB2PERIPH_BASE + 0x0C00)
#define GPIOC_BASE            (APB2PERIPH_BASE + 0x1000)
#define ADC1_BASE             (APB2PERIPH_BASE + 0x2400)
#define TIM1_BASE             (APB2PERIPH_BASE + 0x2C00)
#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000)
#define TIM3_BASE             (APB1PERIPH_BASE + 0x0400)
#define TIM4_BASE             (APB1PERIPH_BASE + 0x0800)

#define RCC                 ((RCC_TypeDef *) RCC_BASE)
#define NVIC                ((NVIC_Type *) NVIC_BASE)
#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)
#define ADC1                ((ADC_TypeDef *) ADC1_BASE)
#define TIM1                ((TIM_TypeDef *) TIM1_BASE)
#define TIM2                ((TIM_TypeDef *) TIM2_BASE)
#define TIM3                ((TIM_TypeDef *) TIM3_BASE)
#define TIM4                ((TIM_TypeDef *) TIM4_BASE)

#endif