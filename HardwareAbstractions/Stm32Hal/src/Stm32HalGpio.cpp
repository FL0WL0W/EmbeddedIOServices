#include "Stm32HalGpio.h"

namespace Stm32
{
	std::function<void()> ExternatlInterruptCallback[16];
    
    void EnableGPIOClock(uint16_t pin)
    {
		switch(pin / 16)
		{
#ifdef GPIOA
		case 0: //PA
  			__HAL_RCC_GPIOA_CLK_ENABLE();
            break;
#endif
#ifdef GPIOB
		case 1: //PB
  			__HAL_RCC_GPIOB_CLK_ENABLE();
            break;
#endif
#ifdef GPIOC
		case 2: //PC
  			__HAL_RCC_GPIOC_CLK_ENABLE();
            break;
#endif
#ifdef GPIOD
		case 3: //PD
  			__HAL_RCC_GPIOD_CLK_ENABLE();
            break;
#endif
#ifdef GPIOE
		case 4: //PE
  			__HAL_RCC_GPIOE_CLK_ENABLE();
            break;
#endif
#ifdef GPIOF
		case 5: //PF
  			__HAL_RCC_GPIOF_CLK_ENABLE();
            break;
#endif
#ifdef GPIOG
		case 6: //PG
  			__HAL_RCC_GPIOG_CLK_ENABLE();
            break;
#endif
#ifdef GPIOH
		case 7: //PH
  			__HAL_RCC_GPIOH_CLK_ENABLE();
            break;
#endif
#ifdef GPIOI
		case 8: //PI
  			__HAL_RCC_GPIOI_CLK_ENABLE();
            break;
#endif
#ifdef GPIOJ
		case 9: //PJ
  			__HAL_RCC_GPIOJ_CLK_ENABLE();
            break;
#endif
#ifdef GPIOK
		case 10: //PK
  			__HAL_RCC_GPIO_CLK_ENABLE();
            break;
#endif
		}
    }
		
	void attachInterrupt(uint16_t pin, std::function<void()> callBack)
	{
		EnableGPIOClock(pin);

		GPIO_InitTypeDef GPIO_InitStruct;

		GPIO_InitStruct.Pin = PinToGPIO_Pin(pin);
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(PinToGPIO(pin), &GPIO_InitStruct);

		switch (GPIO_InitStruct.Pin)
		{
			case GPIO_PIN_0:
				ExternatlInterruptCallback[0] = callBack;
				HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI0_IRQn);
				break;
			case GPIO_PIN_1:
				ExternatlInterruptCallback[1] = callBack;
				HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI1_IRQn);
				break;
			case GPIO_PIN_2:
				ExternatlInterruptCallback[2] = callBack;
				HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI2_IRQn);
				break;
			case GPIO_PIN_3:
				ExternatlInterruptCallback[3] = callBack;
				HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI3_IRQn);
				break;
			case GPIO_PIN_4:
				ExternatlInterruptCallback[4] = callBack;
				HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI4_IRQn);
				break;
			case GPIO_PIN_5:
				ExternatlInterruptCallback[5] = callBack;
				HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_6:
				ExternatlInterruptCallback[6] = callBack;
				HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_7:
				ExternatlInterruptCallback[7] = callBack;
				HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_8:
				ExternatlInterruptCallback[8] = callBack;
				HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_9:
				ExternatlInterruptCallback[9] = callBack;
				HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_10:
				ExternatlInterruptCallback[10] = callBack;
				HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_11:
				ExternatlInterruptCallback[11] = callBack;
				HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_12:
				ExternatlInterruptCallback[12] = callBack;
				HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_13:
				ExternatlInterruptCallback[13] = callBack;
				HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_14:
				ExternatlInterruptCallback[14] = callBack;
				HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_15:
				ExternatlInterruptCallback[15] = callBack;
				HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
				break;
			default:
				break;
		}
		
	}

	void detachInterrupt(uint16_t pin)
	{
		GPIO_InitTypeDef GPIO_InitStruct;

		GPIO_InitStruct.Pin = PinToGPIO_Pin(pin);
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(PinToGPIO(pin), &GPIO_InitStruct);

		uint16_t GPIO_Pin = PinToGPIO_Pin(pin);
		switch (GPIO_Pin)
		{
			case GPIO_PIN_0:
				ExternatlInterruptCallback[0] = 0;
				HAL_NVIC_DisableIRQ(EXTI0_IRQn);
				break;
			case GPIO_PIN_1:
				ExternatlInterruptCallback[1] = 0;
				HAL_NVIC_DisableIRQ(EXTI1_IRQn);
				break;
			case GPIO_PIN_2:
				ExternatlInterruptCallback[2] = 0;
				HAL_NVIC_DisableIRQ(EXTI2_IRQn);
				break;
			case GPIO_PIN_3:
				ExternatlInterruptCallback[3] = 0;
				HAL_NVIC_DisableIRQ(EXTI3_IRQn);
				break;
			case GPIO_PIN_4:
				ExternatlInterruptCallback[4] = 0;
				HAL_NVIC_DisableIRQ(EXTI4_IRQn);
				break;
			case GPIO_PIN_5:
				ExternatlInterruptCallback[5] = 0;
				HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_6:
				ExternatlInterruptCallback[6] = 0;
				HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_7:
				ExternatlInterruptCallback[7] = 0;
				HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_8:
				ExternatlInterruptCallback[8] = 0;
				HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_9:
				ExternatlInterruptCallback[9] = 0;
				HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_10:
				ExternatlInterruptCallback[10] = 0;
				HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_11:
				ExternatlInterruptCallback[11] = 0;
				HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_12:
				ExternatlInterruptCallback[12] = 0;
				HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_13:
				ExternatlInterruptCallback[13] = 0;
				HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_14:
				ExternatlInterruptCallback[14] = 0;
				HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_15:
				ExternatlInterruptCallback[15] = 0;
				HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
				break;
			default:
				break;
		}
	}
	
	extern "C"
	{
		void EXTI0_IRQHandler (void)
		{
			if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0))
			{
				if(ExternatlInterruptCallback[0] != 0)
				{
					ExternatlInterruptCallback[0]();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
			}
		}
		void EXTI1_IRQHandler (void)
		{
			if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_1))
			{
				if(ExternatlInterruptCallback[1] != 0)
				{
					ExternatlInterruptCallback[1]();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
			}
		}
		void EXTI2_IRQHandler (void)
		{
			if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_2))
			{
				if(ExternatlInterruptCallback[2] != 0)
				{
					ExternatlInterruptCallback[2]();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
			}
		}
		void EXTI3_IRQHandler (void)
		{
			if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_3))
			{
				if(ExternatlInterruptCallback[3] != 0)
				{
					ExternatlInterruptCallback[3]();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
			}
		}
		void EXTI4_IRQHandler (void)
		{
			if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_4))
			{
				if(ExternatlInterruptCallback[4] != 0)
				{
					ExternatlInterruptCallback[4]();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);
			}
		}
		void EXTI9_5_IRQHandler (void)
		{
			if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5))
			{
				if(ExternatlInterruptCallback[5] != 0)
				{
					ExternatlInterruptCallback[5]();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_6))
			{
				if(ExternatlInterruptCallback[6] != 0)
				{
					ExternatlInterruptCallback[6]();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_6);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_7))
			{
				if(ExternatlInterruptCallback[7] != 0)
				{
					ExternatlInterruptCallback[7]();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_7);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_8))
			{
				if(ExternatlInterruptCallback[8] != 0)
				{
					ExternatlInterruptCallback[8]();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_9))
			{
				if(ExternatlInterruptCallback[9] != 0)
				{
					ExternatlInterruptCallback[9]();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_9);
			}
		}
		void EXTI15_10_IRQHandler (void)
		{
			if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_10))
			{
				if(ExternatlInterruptCallback[10] != 0)
				{
					ExternatlInterruptCallback[10]();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_10);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_11))
			{
				if(ExternatlInterruptCallback[11] != 0)
				{
					ExternatlInterruptCallback[11]();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_11);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_12))
			{
				if(ExternatlInterruptCallback[12] != 0)
				{
					ExternatlInterruptCallback[12]();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_12);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13))
			{
				if(ExternatlInterruptCallback[13] != 0)
				{
					ExternatlInterruptCallback[13]();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_14))
			{
				if(ExternatlInterruptCallback[14] != 0)
				{
					ExternatlInterruptCallback[14]();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_14);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_15))
			{
				if(ExternatlInterruptCallback[15] != 0)
				{
					ExternatlInterruptCallback[15]();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_15);
			}
		}
	}
}