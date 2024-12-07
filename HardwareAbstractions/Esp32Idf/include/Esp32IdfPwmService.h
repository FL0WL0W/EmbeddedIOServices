#include "PinDirection.h"
#include "IPwmService.h"
#include "stdint.h"
#include <map>

#ifndef ESP32IDFPWMSERVICE_H
#define ESP32IDFPWMSERVICE_H

namespace Esp32
{
	struct Esp32IdfPwmHandle
	{
		uint8_t Timer;
		uint8_t Channel;
	};
	class Esp32IdfPwmService : public EmbeddedIOServices::IPwmService
	{
	protected:
		std::map<EmbeddedIOServices::pwmpin_t, Esp32IdfPwmHandle> _pinHandleMap;
		uint8_t _ledcTimerIdx = 0;
	public:
		void InitPin(EmbeddedIOServices::pwmpin_t pin, EmbeddedIOServices::PinDirection direction, uint16_t minFrequency);
		EmbeddedIOServices::PwmValue ReadPin(EmbeddedIOServices::pwmpin_t pin);
		void WritePin(EmbeddedIOServices::pwmpin_t pin, EmbeddedIOServices::PwmValue value);
	};
}
#endif