#include "HardwareAbstraction/HardwareAbstractionCollection.h"
#include "Service/ServiceLocator.h"
#include "stdint.h"

#if !defined(IBOOLEANINPUTSERVICE_H) && defined(HARDWAREABSTRACTIONCOLLECTION_H)
#define IBOOLEANINPUTSERVICE_H
namespace IOServices
{
	class IBooleanInputService
	{
	public:
		virtual void ReadValue() = 0;
		bool Value = false;

		static void* BuildBooleanInputService(const Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut);
		static IBooleanInputService* CreateBooleanInputService(const HardwareAbstraction::HardwareAbstractionCollection *hardwareAbstractionCollection, const void *config, unsigned int &sizeOut);
	};
}
#endif
