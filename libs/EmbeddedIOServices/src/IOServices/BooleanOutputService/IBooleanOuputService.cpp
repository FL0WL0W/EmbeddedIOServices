#include "IOServices/BooleanOutputService/IBooleanOutputService.h"
#include "IOServices/BooleanOutputService/BooleanOutputService.h"
#include "Service/HardwareAbstractionServiceBuilder.h"
#include "Service/IOServicesServiceBuilderRegister.h"
#include "Service/ServiceBuilder.h"

using namespace HardwareAbstraction;
using namespace Service;

#ifdef IBOOLEANOUTPUTSERVICE_H
namespace IOServices
{
	void IBooleanOutputService::OutputSetCallBack(void *booleanOutputService)
	{
		reinterpret_cast<IBooleanOutputService *>(booleanOutputService)->OutputSet();
	}
	
	void IBooleanOutputService::OutputResetCallBack(void *booleanOutputService)
	{
		reinterpret_cast<IBooleanOutputService *>(booleanOutputService)->OutputReset();
	}
	
	void IBooleanOutputService::BuildBooleanOutputService(ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
	{
		uint8_t instanceId = ServiceBuilder::CastAndOffset<uint8_t>(config, sizeOut);

		IBooleanOutputService *outputService = CreateBooleanOutputService(serviceLocator, config, sizeOut);

		serviceLocator->RegisterIfNotNull(BUILDER_IBOOLEANOUTPUTSERVICE, instanceId, outputService);
	}
	
	IBooleanOutputService *IBooleanOutputService::CreateBooleanOutputService(const ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
	{
		return CreateBooleanOutputService(serviceLocator->LocateAndCast<const HardwareAbstractionCollection>(HARDWARE_ABSTRACTION_COLLECTION_ID), config, sizeOut);
	}
	
	IBooleanOutputService *IBooleanOutputService::CreateBooleanOutputService(const HardwareAbstractionCollection *hardwareAbstractionCollection, const void *config, unsigned int &sizeOut)
	{
		IBooleanOutputService *outputService = 0;

		switch (ServiceBuilder::CastAndOffset<uint8_t>(config, sizeOut))
		{
#ifdef BOOLEANOUTPUTSERVICE_H
			case 1:
			{
				const BooleanOutputServiceConfig *booleanOutputServiceConfig = reinterpret_cast<const BooleanOutputServiceConfig *>(config);
				sizeOut += booleanOutputServiceConfig->Size();
				outputService = new BooleanOutputService(hardwareAbstractionCollection->DigitalService, booleanOutputServiceConfig);
				break;
			}
#endif
		}

		return outputService;
	}
}
#endif
