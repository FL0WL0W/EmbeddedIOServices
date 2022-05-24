#include "IAnalogService.h"

#ifndef ANALOGSERVICE_W806_H
#define ANALOGSERVICE_W806_H
namespace EmbeddedIOServices
{
	class AnalogService_W806 : public IAnalogService
	{
	protected: 
		ADC_HandleTypeDef _hadc;
	public:
		void InitPin(analogpin_t pin);
		float ReadPin(analogpin_t pin);

		static uint32_t PinToChannel(analogpin_t pin);
	};
}
#endif
