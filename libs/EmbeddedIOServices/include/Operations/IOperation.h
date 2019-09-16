#include "Service/IService.h"
                           
#ifndef IOPERATION_H
#define IOPERATION_H
namespace Operations
{
    class IOperationBase : public Service::IService
    {
        protected:
        static Service::ServiceLocator factoryLocator;
        public:
		static void Build(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut);

		static IOperationBase *Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut);
        
        ISERVICE_REGISTERSERVICEFACTORY_H
    };

    template<typename RET, typename... PARAMS>
    class IOperation : public IOperationBase
    {
        public:
        virtual RET Execute(PARAMS...) = 0;
    };
}
#endif