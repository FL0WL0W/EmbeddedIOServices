#include "Service/ServiceLocator.h"

#ifdef SERVICELOCATOR_H
namespace Service
{
	void ServiceLocator::Register(uint16_t const &serviceId, void * const &service)
	{
		_services.insert(std::pair<uint16_t, void *>(serviceId, service));
	}

	void ServiceLocator::RegisterIfNotNull(uint16_t const &serviceId, void * const &service)
	{
		if(service != 0)
		{
			Register(serviceId, service);
		}
	}

	void ServiceLocator::Register(uint16_t const &serviceId, uint32_t const &instanceId, void * const &service)
	{
		void* serviceArray = Locate(serviceId);

		if(serviceArray == 0)
		{
			serviceArray = malloc((instanceId + 2) * sizeof(void *));
			*reinterpret_cast<uint8_t *>(serviceArray) = instanceId + 1;
		}
		else
		{
			uint32_t size = *reinterpret_cast<uint32_t *>(serviceArray);
			if(instanceId + 1 > size)
			{
				free(serviceArray);

				serviceArray = malloc((instanceId + 2) * sizeof(void *));
				*reinterpret_cast<uint32_t *>(serviceArray) = instanceId + 1;

				Register(serviceId, serviceArray);
			}
		}
		
		reinterpret_cast<void **>(serviceArray)[instanceId + 1] = service;
	}

	void ServiceLocator::RegisterIfNotNull(uint16_t const &serviceId, uint32_t const &instanceId, void * const &service)
	{
		if(service != 0)
		{
			Register(serviceId, instanceId, service);
		}
	}

	void* ServiceLocator::Locate(uint16_t const &serviceId) const
	{
		const std::map<uint16_t, void *>::const_iterator it = _services.find(serviceId);
		if (it != _services.end())
			return it->second;
		return 0;
	}

	void* ServiceLocator::Locate(uint16_t const &serviceId, uint32_t const &instanceId) const
	{
		void* serviceArray = Locate(serviceId);

		if(serviceArray != 0 && *reinterpret_cast<uint32_t *>(serviceArray) < instanceId)
		{
			return reinterpret_cast<void **>(serviceArray)[instanceId + 1];
		}

		return 0;
	}

	void ServiceLocator::Unregister(uint16_t const &serviceId)
	{
		const std::map<uint16_t, void *>::iterator it = _services.find(serviceId);
		if (it != _services.end())
			_services.erase(it);
	}
}
#endif
