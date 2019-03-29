#include "Service/ServiceBuilder.h"

#ifdef SERVICEBUILDER_H
namespace Service
{
	void ServiceBuilder::Build(ServiceLocator *&serviceLocator, const void *config, unsigned int &size)
	{
		size = 0;
		uint16_t serviceId;

		while ((serviceId = *reinterpret_cast<const uint16_t *>(config)) != 0)
		{
			OffsetConfig(config, size, sizeof(const uint16_t));

			void*(*factory)(ServiceLocator *, const void *config, unsigned int &size) = ServiceLocator::LocateAndCast<void*(ServiceLocator *, const void *config, unsigned int &size)>(serviceId);

			void *service = CreateServiceAndOffset(factory, serviceLocator, config, size);
			
			RegisterIfNotNull(serviceLocator, serviceId, service);
		}
	}
	
	void ServiceBuilder::Unregister(uint16_t serviceId)
	{
		ServiceLocator::Unregister(serviceId);
	}
}
#endif
