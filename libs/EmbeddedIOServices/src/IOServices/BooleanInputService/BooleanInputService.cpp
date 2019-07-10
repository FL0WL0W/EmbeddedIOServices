#include "IOServices/BooleanInputService/BooleanInputService.h"

using namespace HardwareAbstraction;

#ifdef BOOLEANINPUTSERVICE_H
namespace IOServices
{
	BooleanInputService::BooleanInputService(IDigitalService *digitalService, const BooleanInputServiceConfig *config)
	{
		_digitalService = digitalService;
		_config = config;
		
		_digitalService->InitPin(_config->Pin, In);
	}
	
	void BooleanInputService::ReadValue()
	{
		Value = _digitalService->ReadPin(_config->Pin);
		if (_config->Inverted)
			Value = !Value;
	}
}
#endif
