#include "IPwmService.h"

#ifndef PWMSERVICE_W80X_H
#define PWMSERVICE_W80X_H
namespace EmbeddedIOServices
{
	class PwmService_W80x : public IPwmService
	{
	public:
		void InitPin(pwmpin_t pin, PinDirection direction, uint16_t minFreqeuncy);
		PwmValue ReadPin(pwmpin_t pin);
		void WritePin(pwmpin_t pin, PwmValue value);
		static uint8_t PinToChannel(pwmpin_t pin);
	};
}
#endif
