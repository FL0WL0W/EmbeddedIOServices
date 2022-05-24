#include "PwmService_W806.h"

#ifdef PWMSERVICE_W806_H
namespace EmbeddedIOServices
{
	//This all needs some work. the PWM HAL only has output and is very primitive. need to modify registers manually like digital service
	void PwmService_W806::InitPin(pwmpin_t pin, PinDirection direction, uint16_t minFreqeuncy)
	{
		
	}
	PwmValue PwmService_W806::ReadPin(pwmpin_t pin)
	{
		
	}
	void PwmService_W806::WritePin(pwmpin_t pin, PwmValue value)
	{
		
	}
}
#endif
