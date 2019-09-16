#include "Service/IService.h"

#ifndef IVARIABLESERVICE_H
#define IVARIABLESERVICE_H
namespace VariableBus
{
    class IVariableService : public Service::IService
    {
        protected:
        static Service::ServiceLocator factoryLocator;
        public:
		virtual void TranslateValue() = 0;
		static void Build(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut);

		static IVariableService *Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut);
        
        ISERVICE_REGISTERSERVICEFACTORY_H
    };
}
#endif