#include "IOServices/FloatInputService/IFloatInputService.h"
#include "IOServices/FloatInputService/FloatInputService_Static.h"
#include "IOServices/FloatInputService/FloatInputService_AnalogPin.h"
#include "IOServices/FloatInputService/FloatInputService_FrequencyPin.h"
#include "IOServices/FloatInputService/FloatInputService_PulseWidthPin.h"
#include "IOServices/FloatInputService/FloatInputService_DutyCyclePin.h"
#include "IOServices/FloatInputService/FloatInputService_FaultDetectionWrapper.h"
#include "Service/HardwareAbstractionServiceBuilder.h"
#include "Service/IOServicesServiceBuilderRegister.h"
#include "Service/ServiceBuilder.h"

using namespace HardwareAbstraction;
using namespace Service;

#ifdef IFLOATINPUTSERVICE_H
namespace IOServices
{
	void IFloatInputService::BuildFloatInputService(ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
	{
		uint8_t instanceId = ServiceBuilder::CastAndOffset<uint8_t>(config, sizeOut);

		IFloatInputService *inputService = CreateFloatInputService(serviceLocator, config, sizeOut);
		
		serviceLocator->LocateAndCast<CallBackGroup>(TICK_CALL_BACK_GROUP)->Add(
			new CallBack<IFloatInputService>(inputService, &IFloatInputService::ReadValue));

		serviceLocator->RegisterIfNotNull(BUILDER_IFLOATINPUTSERVICE, instanceId, inputService);
	}

	IFloatInputService* IFloatInputService::CreateFloatInputService(const ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
	{
		return CreateFloatInputService(serviceLocator->LocateAndCast<const HardwareAbstractionCollection>(HARDWARE_ABSTRACTION_COLLECTION_ID), config, sizeOut);
	}
	
	IFloatInputService* IFloatInputService::CreateFloatInputService(const HardwareAbstractionCollection *hardwareAbstractionCollection, const void *config, unsigned int &sizeOut)
	{
		IFloatInputService *inputService = 0;

		switch (ServiceBuilder::CastAndOffset<uint8_t>(config, sizeOut))
		{
#ifdef FLOATINPUTSERVICE_STATIC_H
		case 1:
			{
				const float value = ServiceBuilder::CastAndOffset<float>(config, sizeOut);
				inputService = new FloatInputService_Static(value);
				break;
			}
#endif
			
#ifdef FLOATINPUTSERVICE_ANALOGPIN_H
		case 2:
			{
       			const uint16_t pin = ServiceBuilder::CastAndOffset<uint16_t>(config, sizeOut);
				inputService = new FloatInputService_AnalogPin(hardwareAbstractionCollection->AnalogService, pin);
				break;
			}
#endif
			
#ifdef FLOATINPUTSERVICE_FREQUENCYPIN_H
		case 3:
			{
       			const uint16_t pin = ServiceBuilder::CastAndOffset<uint16_t>(config, sizeOut);
       			const uint16_t minFrequency = ServiceBuilder::CastAndOffset<uint16_t>(config, sizeOut);
				inputService = new FloatInputService_FrequencyPin(hardwareAbstractionCollection->PwmService, pin, minFrequency);
				break;
			}
#endif

#ifdef FLOATINPUTSERVICE_PULSEWIDTHPIN_H
		case 4:
			{
       			const uint16_t pin = ServiceBuilder::CastAndOffset<uint16_t>(config, sizeOut);
       			const uint16_t minFrequency = ServiceBuilder::CastAndOffset<uint16_t>(config, sizeOut);
				inputService = new FloatInputService_PulseWidthPin(hardwareAbstractionCollection->PwmService, pin, minFrequency);
				break;
			}
#endif

#ifdef FLOATINPUTSERVICE_DUTYCYCLEPIN_H
		case 5:
			{
       			const uint16_t pin = ServiceBuilder::CastAndOffset<uint16_t>(config, sizeOut);
       			const uint16_t minFrequency = ServiceBuilder::CastAndOffset<uint16_t>(config, sizeOut);
				inputService = new FloatInputService_DutyCyclePin(hardwareAbstractionCollection->PwmService, pin, minFrequency);
				break;
			}
#endif

#ifdef FLOATINPUTSERVICE_FAULTDETECTIONWRAPPER_H
		case 6:
			{
				const FloatInputService_FaultDetectionWrapperConfig *faultDetectionConfig = reinterpret_cast<const FloatInputService_FaultDetectionWrapperConfig *>(config);
				sizeOut += faultDetectionConfig->Size();
				config = reinterpret_cast<const uint8_t *>(config) + faultDetectionConfig->Size();
				unsigned int childSize = 0;
				IFloatInputService *child = CreateFloatInputService(hardwareAbstractionCollection, config, childSize);
				sizeOut += childSize;
				inputService = new FloatInputService_FaultDetectionWrapper(faultDetectionConfig, child);
				break;
			}
#endif
		}
		
		return inputService;
	}
}
#endif
