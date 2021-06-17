#include "Stm32HalGpio.h"

namespace Stm32
{
	std::function<void()> ExternatlInterrupt0Callback;
	std::function<void()> ExternatlInterrupt1Callback;
	std::function<void()> ExternatlInterrupt2Callback;
	std::function<void()> ExternatlInterrupt3Callback;
	std::function<void()> ExternatlInterrupt4Callback;
	std::function<void()> ExternatlInterrupt5Callback;
	std::function<void()> ExternatlInterrupt6Callback;
	std::function<void()> ExternatlInterrupt7Callback;
	std::function<void()> ExternatlInterrupt8Callback;
	std::function<void()> ExternatlInterrupt9Callback;
	std::function<void()> ExternatlInterrupt10Callback;
	std::function<void()> ExternatlInterrupt11Callback;
	std::function<void()> ExternatlInterrupt12Callback;
	std::function<void()> ExternatlInterrupt13Callback;
	std::function<void()> ExternatlInterrupt14Callback;
	std::function<void()> ExternatlInterrupt15Callback;
    
    void EnableGPIOClock(uint16_t pin)
    {
		//PA 1-16
		//PB 17-32
		//PC 33-48
		//PD 49-64
		//PE 65-80
		//PF 81-96
		//PG 97-112
		//PH 113-128
		//PI 129-144
		//PJ 145-160
		//PK 161-176
		switch((pin - 1) / 16)
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
				ExternatlInterrupt0Callback = callBack;
				HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI0_IRQn);
				break;
			case GPIO_PIN_1:
				ExternatlInterrupt1Callback = callBack;
				HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI1_IRQn);
				break;
			case GPIO_PIN_2:
				ExternatlInterrupt2Callback = callBack;
				HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI2_IRQn);
				break;
			case GPIO_PIN_3:
				ExternatlInterrupt3Callback = callBack;
				HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI3_IRQn);
				break;
			case GPIO_PIN_4:
				ExternatlInterrupt4Callback = callBack;
				HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI4_IRQn);
				break;
			case GPIO_PIN_5:
				ExternatlInterrupt5Callback = callBack;
				HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_6:
				ExternatlInterrupt6Callback = callBack;
				HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_7:
				ExternatlInterrupt7Callback = callBack;
				HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_8:
				ExternatlInterrupt8Callback = callBack;
				HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_9:
				ExternatlInterrupt9Callback = callBack;
				HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_10:
				ExternatlInterrupt10Callback = callBack;
				HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_11:
				ExternatlInterrupt11Callback = callBack;
				HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_12:
				ExternatlInterrupt12Callback = callBack;
				HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_13:
				ExternatlInterrupt13Callback = callBack;
				HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_14:
				ExternatlInterrupt14Callback = callBack;
				HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_15:
				ExternatlInterrupt15Callback = callBack;
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
				ExternatlInterrupt0Callback = 0;
				HAL_NVIC_DisableIRQ(EXTI0_IRQn);
				break;
			case GPIO_PIN_1:
				ExternatlInterrupt1Callback = 0;
				HAL_NVIC_DisableIRQ(EXTI1_IRQn);
				break;
			case GPIO_PIN_2:
				ExternatlInterrupt2Callback = 0;
				HAL_NVIC_DisableIRQ(EXTI2_IRQn);
				break;
			case GPIO_PIN_3:
				ExternatlInterrupt3Callback = 0;
				HAL_NVIC_DisableIRQ(EXTI3_IRQn);
				break;
			case GPIO_PIN_4:
				ExternatlInterrupt4Callback = 0;
				HAL_NVIC_DisableIRQ(EXTI4_IRQn);
				break;
			case GPIO_PIN_5:
				ExternatlInterrupt5Callback = 0;
				HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_6:
				ExternatlInterrupt6Callback = 0;
				HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_7:
				ExternatlInterrupt7Callback = 0;
				HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_8:
				ExternatlInterrupt8Callback = 0;
				HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_9:
				ExternatlInterrupt9Callback = 0;
				HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_10:
				ExternatlInterrupt10Callback = 0;
				HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_11:
				ExternatlInterrupt11Callback = 0;
				HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_12:
				ExternatlInterrupt12Callback = 0;
				HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_13:
				ExternatlInterrupt13Callback = 0;
				HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_14:
				ExternatlInterrupt14Callback = 0;
				HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_15:
				ExternatlInterrupt15Callback = 0;
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
				if(ExternatlInterrupt0Callback != 0)
				{
					ExternatlInterrupt0Callback();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
			}
		}
		void EXTI1_IRQHandler (void)
		{
			if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_1))
			{
				if(ExternatlInterrupt1Callback != 0)
				{
					ExternatlInterrupt1Callback();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
			}
		}
		void EXTI2_IRQHandler (void)
		{
			if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_2))
			{
				if(ExternatlInterrupt2Callback != 0)
				{
					ExternatlInterrupt2Callback();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
			}
		}
		void EXTI3_IRQHandler (void)
		{
			if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_3))
			{
				if(ExternatlInterrupt3Callback != 0)
				{
					ExternatlInterrupt3Callback();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
			}
		}
		void EXTI4_IRQHandler (void)
		{
			if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_4))
			{
				if(ExternatlInterrupt4Callback != 0)
				{
					ExternatlInterrupt4Callback();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);
			}
		}
		void EXTI9_5_IRQHandler (void)
		{
			if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5))
			{
				if(ExternatlInterrupt5Callback != 0)
				{
					ExternatlInterrupt5Callback();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_6))
			{
				if(ExternatlInterrupt6Callback != 0)
				{
					ExternatlInterrupt6Callback();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_6);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_7))
			{
				if(ExternatlInterrupt7Callback != 0)
				{
					ExternatlInterrupt7Callback();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_7);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_8))
			{
				if(ExternatlInterrupt8Callback != 0)
				{
					ExternatlInterrupt8Callback();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_9))
			{
				if(ExternatlInterrupt9Callback != 0)
				{
					ExternatlInterrupt9Callback();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_9);
			}
		}
		void EXTI15_10_IRQHandler (void)
		{
			if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_10))
			{
				if(ExternatlInterrupt10Callback != 0)
				{
					ExternatlInterrupt10Callback();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_10);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_11))
			{
				if(ExternatlInterrupt11Callback != 0)
				{
					ExternatlInterrupt11Callback();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_11);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_12))
			{
				if(ExternatlInterrupt12Callback != 0)
				{
					ExternatlInterrupt12Callback();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_12);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13))
			{
				if(ExternatlInterrupt13Callback != 0)
				{
					ExternatlInterrupt13Callback();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_14))
			{
				if(ExternatlInterrupt14Callback != 0)
				{
					ExternatlInterrupt14Callback();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_14);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_15))
			{
				if(ExternatlInterrupt15Callback != 0)
				{
					ExternatlInterrupt15Callback();
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_15);
			}
		}
	}
}