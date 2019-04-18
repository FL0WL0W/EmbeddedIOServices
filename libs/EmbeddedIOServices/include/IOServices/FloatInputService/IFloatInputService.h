#include "HardwareAbstraction/HardwareAbstractionCollection.h"
#include "Service/ServiceLocator.h"
#include "stdint.h"

#if !defined(IFLOATINPUTSERVICE_H) && defined(HARDWAREABSTRACTIONCOLLECTION_H)
#define IFLOATINPUTSERVICE_H
namespace IOServices
{
	class IFloatInputService
	{
	public:
		virtual void ReadValue() = 0;
		float Value = 0;
		float ValueDot = 0;

		static void* BuildFloatInputService(const Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut);
		static IFloatInputService* CreateFloatInputService(const HardwareAbstraction::HardwareAbstractionCollection *hardwareAbstractionCollection, const void *config, unsigned int &sizeOut);
	};
}
#endif
