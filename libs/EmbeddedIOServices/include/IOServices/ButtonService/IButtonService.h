#include "HardwareAbstraction/HardwareAbstractionCollection.h"
#include "Service/ServiceLocator.h"
#include "stdint.h"

#if !defined(IBUTTONSERVICE_H) && defined(HARDWAREABSTRACTIONCOLLECTION_H)
#define IBUTTONSERVICE_H
namespace IOServices
{
	class IButtonService
	{
	protected:
		HardwareAbstraction::CallBackGroup *_callBackGroup;
		IButtonService();
	public:
		void Add(HardwareAbstraction::ICallBack *callBack);
		void Remove(HardwareAbstraction::ICallBack *callBack);
		void Clear();
		virtual void Tick() = 0;

		static void* BuildButtonService(const Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut);
		static IButtonService* CreateButtonService(const HardwareAbstraction::HardwareAbstractionCollection *hardwareAbstractionCollection, const void *config, unsigned int &sizeOut);
	};
}
#endif
