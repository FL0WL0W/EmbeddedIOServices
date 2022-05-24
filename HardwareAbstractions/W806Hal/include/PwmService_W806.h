#include "IPwmService.h"

#ifndef PWMSERVICE_W806_H
#define PWMSERVICE_W806_H
namespace EmbeddedIOServices
{
	class PwmService_W806 : public IPwmService
	{
	public:
		void InitPin(pwmpin_t pin, PinDirection direction, uint16_t minFreqeuncy);
		PwmValue ReadPin(pwmpin_t pin);
		void WritePin(pwmpin_t pin, PwmValue value);
	};
}
#endif
