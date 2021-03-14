#include "Stm32HalGpio.h"

using namespace EmbeddedIOServices;

namespace Stm32
{
	ICallBack *ExternatlInterrupt0Callback;
	ICallBack *ExternatlInterrupt1Callback;
	ICallBack *ExternatlInterrupt2Callback;
	ICallBack *ExternatlInterrupt3Callback;
	ICallBack *ExternatlInterrupt4Callback;
	ICallBack *ExternatlInterrupt5Callback;
	ICallBack *ExternatlInterrupt6Callback;
	ICallBack *ExternatlInterrupt7Callback;
	ICallBack *ExternatlInterrupt8Callback;
	ICallBack *ExternatlInterrupt9Callback;
	ICallBack *ExternatlInterrupt10Callback;
	ICallBack *ExternatlInterrupt11Callback;
	ICallBack *ExternatlInterrupt12Callback;
	ICallBack *ExternatlInterrupt13Callback;
	ICallBack *ExternatlInterrupt14Callback;
	ICallBack *ExternatlInterrupt15Callback;
	
    bool ExternatlInterrupt0DeleteOnExecution;
    bool ExternatlInterrupt1DeleteOnExecution;
    bool ExternatlInterrupt2DeleteOnExecution;
    bool ExternatlInterrupt3DeleteOnExecution;
    bool ExternatlInterrupt4DeleteOnExecution;
    bool ExternatlInterrupt5DeleteOnExecution;
    bool ExternatlInterrupt6DeleteOnExecution;
    bool ExternatlInterrupt7DeleteOnExecution;
    bool ExternatlInterrupt8DeleteOnExecution;
    bool ExternatlInterrupt9DeleteOnExecution;
    bool ExternatlInterrupt10DeleteOnExecution;
    bool ExternatlInterrupt11DeleteOnExecution;
    bool ExternatlInterrupt12DeleteOnExecution;
    bool ExternatlInterrupt13DeleteOnExecution;
    bool ExternatlInterrupt14DeleteOnExecution;
    bool ExternatlInterrupt15DeleteOnExecution;
    
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
		
	void EnableInterrupt(uint16_t pin, ICallBack *callBack, bool deleteOnExecution)
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
				ExternatlInterrupt0DeleteOnExecution = deleteOnExecution;
				HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI0_IRQn);
				break;
			case GPIO_PIN_1:
				ExternatlInterrupt1Callback = callBack;
				ExternatlInterrupt1DeleteOnExecution = deleteOnExecution;
				HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI1_IRQn);
				break;
			case GPIO_PIN_2:
				ExternatlInterrupt2Callback = callBack;
				ExternatlInterrupt2DeleteOnExecution = deleteOnExecution;
				HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI2_IRQn);
				break;
			case GPIO_PIN_3:
				ExternatlInterrupt3Callback = callBack;
				ExternatlInterrupt3DeleteOnExecution = deleteOnExecution;
				HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI3_IRQn);
				break;
			case GPIO_PIN_4:
				ExternatlInterrupt4Callback = callBack;
				ExternatlInterrupt4DeleteOnExecution = deleteOnExecution;
				HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI4_IRQn);
				break;
			case GPIO_PIN_5:
				ExternatlInterrupt5Callback = callBack;
				ExternatlInterrupt5DeleteOnExecution = deleteOnExecution;
				HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_6:
				ExternatlInterrupt6Callback = callBack;
				ExternatlInterrupt6DeleteOnExecution = deleteOnExecution;
				HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_7:
				ExternatlInterrupt7Callback = callBack;
				ExternatlInterrupt7DeleteOnExecution = deleteOnExecution;
				HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_8:
				ExternatlInterrupt8Callback = callBack;
				ExternatlInterrupt8DeleteOnExecution = deleteOnExecution;
				HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_9:
				ExternatlInterrupt9Callback = callBack;
				ExternatlInterrupt9DeleteOnExecution = deleteOnExecution;
				HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
				break;
			case GPIO_PIN_10:
				ExternatlInterrupt10Callback = callBack;
				ExternatlInterrupt10DeleteOnExecution = deleteOnExecution;
				HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_11:
				ExternatlInterrupt11Callback = callBack;
				ExternatlInterrupt11DeleteOnExecution = deleteOnExecution;
				HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_12:
				ExternatlInterrupt12Callback = callBack;
				ExternatlInterrupt12DeleteOnExecution = deleteOnExecution;
				HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_13:
				ExternatlInterrupt13Callback = callBack;
				ExternatlInterrupt13DeleteOnExecution = deleteOnExecution;
				HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_14:
				ExternatlInterrupt14Callback = callBack;
				ExternatlInterrupt14DeleteOnExecution = deleteOnExecution;
				HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
				break;
			case GPIO_PIN_15:
				ExternatlInterrupt15Callback = callBack;
				ExternatlInterrupt15DeleteOnExecution = deleteOnExecution;
				HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
				break;
			default:
				break;
		}
		
	}

	void DisableInterrupt(uint16_t pin)
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
					ExternatlInterrupt0Callback->Execute();
					if(ExternatlInterrupt0DeleteOnExecution)
					{
						delete ExternatlInterrupt0Callback;
						ExternatlInterrupt0Callback = 0;
					}
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
					ExternatlInterrupt1Callback->Execute();
					if(ExternatlInterrupt1DeleteOnExecution)
					{
						delete ExternatlInterrupt1Callback;
						ExternatlInterrupt1Callback = 0;
					}
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
					ExternatlInterrupt2Callback->Execute();
					if(ExternatlInterrupt2DeleteOnExecution)
					{
						delete ExternatlInterrupt2Callback;
						ExternatlInterrupt2Callback = 0;
					}
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
					ExternatlInterrupt3Callback->Execute();
					if(ExternatlInterrupt3DeleteOnExecution)
					{
						delete ExternatlInterrupt3Callback;
						ExternatlInterrupt3Callback = 0;
					}
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
					ExternatlInterrupt4Callback->Execute();
					if(ExternatlInterrupt4DeleteOnExecution)
					{
						delete ExternatlInterrupt4Callback;
						ExternatlInterrupt4Callback = 0;
					}
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
					ExternatlInterrupt5Callback->Execute();
					if(ExternatlInterrupt5DeleteOnExecution)
					{
						delete ExternatlInterrupt5Callback;
						ExternatlInterrupt5Callback = 0;
					}
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_6))
			{
				if(ExternatlInterrupt6Callback != 0)
				{
					ExternatlInterrupt6Callback->Execute();
					if(ExternatlInterrupt6DeleteOnExecution)
					{
						delete ExternatlInterrupt6Callback;
						ExternatlInterrupt6Callback = 0;
					}
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_6);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_7))
			{
				if(ExternatlInterrupt7Callback != 0)
				{
					ExternatlInterrupt7Callback->Execute();
					if(ExternatlInterrupt7DeleteOnExecution)
					{
						delete ExternatlInterrupt7Callback;
						ExternatlInterrupt7Callback = 0;
					}
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_7);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_8))
			{
				if(ExternatlInterrupt8Callback != 0)
				{
					ExternatlInterrupt8Callback->Execute();
					if(ExternatlInterrupt8DeleteOnExecution)
					{
						delete ExternatlInterrupt8Callback;
						ExternatlInterrupt8Callback = 0;
					}
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_9))
			{
				if(ExternatlInterrupt9Callback != 0)
				{
					ExternatlInterrupt9Callback->Execute();
					if(ExternatlInterrupt9DeleteOnExecution)
					{
						delete ExternatlInterrupt9Callback;
						ExternatlInterrupt9Callback = 0;
					}
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
					ExternatlInterrupt10Callback->Execute();
					if(ExternatlInterrupt10DeleteOnExecution)
					{
						delete ExternatlInterrupt10Callback;
						ExternatlInterrupt10Callback = 0;
					}
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_10);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_11))
			{
				if(ExternatlInterrupt11Callback != 0)
				{
					ExternatlInterrupt11Callback->Execute();
					if(ExternatlInterrupt11DeleteOnExecution)
					{
						delete ExternatlInterrupt11Callback;
						ExternatlInterrupt11Callback = 0;
					}
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_11);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_12))
			{
				if(ExternatlInterrupt12Callback != 0)
				{
					ExternatlInterrupt12Callback->Execute();
					if(ExternatlInterrupt12DeleteOnExecution)
					{
						delete ExternatlInterrupt12Callback;
						ExternatlInterrupt12Callback = 0;
					}
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_12);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13))
			{
				if(ExternatlInterrupt13Callback != 0)
				{
					ExternatlInterrupt13Callback->Execute();
					if(ExternatlInterrupt13DeleteOnExecution)
					{
						delete ExternatlInterrupt13Callback;
						ExternatlInterrupt13Callback = 0;
					}
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_14))
			{
				if(ExternatlInterrupt14Callback != 0)
				{
					ExternatlInterrupt14Callback->Execute();
					if(ExternatlInterrupt14DeleteOnExecution)
					{
						delete ExternatlInterrupt14Callback;
						ExternatlInterrupt14Callback = 0;
					}
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_14);
			}
			else if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_15))
			{
				if(ExternatlInterrupt15Callback != 0)
				{
					ExternatlInterrupt15Callback->Execute();
					if(ExternatlInterrupt15DeleteOnExecution)
					{
						delete ExternatlInterrupt15Callback;
						ExternatlInterrupt15Callback = 0;
					}
				}
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_15);
			}
		}
	}
}