#include "IOServices/BooleanOutputService/BooleanOutputService.h"

using namespace HardwareAbstraction;

#ifdef BOOLEANOUTPUTSERVICE_H
namespace IOServices
{
	BooleanOutputService::BooleanOutputService(IDigitalService *digitalService, const BooleanOutputServiceConfig *config)
	{
		_digitalService = digitalService;
		_config = config;		

		if (_config->HighZ && _config->NormalOn)
		{
			_digitalService->InitPin(_config->Pin, In);
		}
		else
		{
			_digitalService->InitPin(_config->Pin, Out);

			_digitalService->WritePin(_config->Pin, _config->NormalOn);
		}
	}
	
	void BooleanOutputService::OutputSet()
	{
		if (_config->HighZ && !_config->NormalOn)
		{
			_digitalService->InitPin(_config->Pin, In);
		}
		else
		{
			if (_config->HighZ)
			{
				_digitalService->InitPin(_config->Pin, Out);
			}
			
			_digitalService->WritePin(_config->Pin, !_config->NormalOn);
		}
	}
	
	void BooleanOutputService::OutputReset()
	{
		if (_config->HighZ && _config->NormalOn)
		{
			_digitalService->InitPin(_config->Pin, In);
		}
		else
		{
			if (_config->HighZ)
			{
				_digitalService->InitPin(_config->Pin, Out);
			}
			
			_digitalService->WritePin(_config->Pin, _config->NormalOn);
		}
	}
		
	void BooleanOutputService::OutputWrite(bool val)
	{
		if (val)
			OutputSet();
		else
			OutputReset();
	}
}
#endif
