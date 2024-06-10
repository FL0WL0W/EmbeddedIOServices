#include "IAnalogService.h"
#include "wm_adc.h"

#ifndef ANALOGSERVICE_W80X_H
#define ANALOGSERVICE_W80X_H
namespace EmbeddedIOServices
{
	class AnalogService_W80x : public IAnalogService
	{
	protected: 
		ADC_HandleTypeDef _hadc;
	public:
		void InitPin(analogpin_t pin);
		float ReadPin(analogpin_t pin);

		static inline uint32_t PinToChannel(analogpin_t pin)
		{
			switch(pin)
			{
				default:
					return ADC_CHANNEL_0;
				case 2:
					return ADC_CHANNEL_1;
				case 3:
					return ADC_CHANNEL_2;
				case 4:
					return ADC_CHANNEL_3;
			}
		}
	};
}
#endif
