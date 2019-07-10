#include "Service/ServiceBuilder.h"

#ifdef SERVICEBUILDER_H
namespace Service
{
	void ServiceBuilder::Build(ServiceLocator *&serviceLocator, const void *config, unsigned int &sizeOut)
	{
		sizeOut = 0;
		uint16_t builderId;

		while ((builderId = CastAndOffset<uint16_t>(config, sizeOut)) != 0)
		{
			void(*factory)(ServiceLocator * const &, const void *, unsigned int &) = ServiceLocator::LocateAndCast<void(ServiceLocator * const&, const void *, unsigned int &)>(builderId);
			
			CreateServiceAndOffset(factory, serviceLocator, config, sizeOut);
		}
	}
	
	void ServiceBuilder::Register(uint16_t serviceId, void(*factory)(ServiceLocator * const &, const void *, unsigned int &))
	{
		ServiceLocator::Register(serviceId, reinterpret_cast<void *>(factory));
	}

	void ServiceBuilder::CreateServiceAndOffset(void(*factory)(ServiceLocator * const &, const void *, unsigned int &), ServiceLocator * const &serviceLocator, const void *&config, unsigned int &totalSize)
	{
		unsigned int size;
		factory(serviceLocator, config, size);
		OffsetConfig(config, totalSize, size);
	}
}
#endif
