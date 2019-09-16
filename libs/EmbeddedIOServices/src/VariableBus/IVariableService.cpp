#include "VariableBus/IVariableService.h"
#include "Service/VariableBusServiceBuilderIds.h"
#include "Service\HardwareAbstractionServiceBuilder.h"

#ifdef IVARIABLESERVICE_H

namespace VariableBus
{
    Service::ServiceLocator IVariableService::factoryLocator;

    void IVariableService::Build(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
    {
        uint8_t instanceId = IService::CastAndOffset<uint8_t>(config, sizeOut);

        IVariableService *variableService = Create(serviceLocator, config, sizeOut);

		serviceLocator->LocateAndCast<CallBackGroup>(TICK_CALL_BACK_GROUP)->Add(new CallBack<IVariableService>(variableService, &IVariableService::TranslateValue));
    }

    IVariableService *IVariableService::Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
    {
        const uint16_t factoryId = IService::CastAndOffset<uint16_t>(config, sizeOut);

        IVariableService*(*factory)(Service::ServiceLocator * const &, const void *, unsigned int &) = factoryLocator.LocateAndCast<IVariableService*(Service::ServiceLocator * const &, const void *, unsigned int &)>(factoryId);

        if(factory == 0)
            return 0;
        return factory(serviceLocator, config, sizeOut);
    }
    
    ISERVICE_REGISTERSERVICEFACTORY_CPP(IVariableService, BUILDER_VARIABLEBUS)
}

#endif