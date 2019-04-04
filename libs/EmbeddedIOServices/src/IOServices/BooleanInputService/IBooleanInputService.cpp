#include "IOServices/BooleanInputService/IBooleanInputService.h"
#include "IOServices/BooleanInputService/BooleanInputService_Static.h"
#include "IOServices/BooleanInputService/BooleanInputService.h"
#include "Service/HardwareAbstractionServiceBuilder.h"
#include "Service/ServiceBuilder.h"

#ifdef IBOOLEANINPUTSERVICE_H
namespace IOServices
{
	void IBooleanInputService::ReadValueCallBack(void *booleanInputService)
	{
		reinterpret_cast<IBooleanInputService *>(booleanInputService)->ReadValue();
	}

	void* IBooleanInputService::BuildBooleanInputService(const ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
	{
		IBooleanInputService *ret = CreateBooleanInputService(serviceLocator->LocateAndCast<const HardwareAbstractionCollection>(HARDWARE_ABSTRACTION_COLLECTION_ID), config, sizeOut);
		
		serviceLocator->LocateAndCast<CallBackGroup>(TICK_CALL_BACK_GROUP)->AddIfParametersNotNull(
			IBooleanInputService::ReadValueCallBack,
			ret);

		return ret;
	}

	IBooleanInputService* IBooleanInputService::CreateBooleanInputService(const HardwareAbstractionCollection *hardwareAbstractionCollection, const void *config, unsigned int &sizeOut)
	{
		sizeOut = 0;
		IBooleanInputService *inputService = 0;

		switch (ServiceBuilder::CastAndOffset<uint8_t>(config, sizeOut))
		{
#ifdef BOOLEANINPUTSERVICE_STATIC_H
		case 1:
			sizeOut += sizeof(float);
			inputService = new BooleanInputService_Static(*reinterpret_cast<const bool *>(config));
			break;
#endif
			
#ifdef BOOLEANINPUTSERVICE_H
		case 2:
			{
				const BooleanInputServiceConfig *booleanInputServiceConfig = reinterpret_cast<const BooleanInputServiceConfig *>(config);
				sizeOut += booleanInputServiceConfig->Size();
				inputService = new BooleanInputService(hardwareAbstractionCollection, booleanInputServiceConfig);
				break;
			}
#endif
		}
		
		return inputService;
	}
}
#endif
