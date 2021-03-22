#include "PinDirection.h"
#include "IDigitalService.h"
#include "IPwmService.h"
#include "IAnalogService.h"
#include "ITimerService.h"

#ifndef EMBEDDEDIOSERVICECOLLECTION_H
#define EMBEDDEDIOSERVICECOLLECTION_H
namespace EmbeddedIOServices
{
	struct EmbeddedIOServiceCollection
	{
	public:
		IDigitalService *DigitalService;
		IPwmService *PwmService;
		IAnalogService *AnalogService;
		ITimerService *TimerService;
	};
}
#endif
