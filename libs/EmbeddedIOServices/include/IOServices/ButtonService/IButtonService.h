#include "HardwareAbstraction/HardwareAbstractionCollection.h"

using namespace HardwareAbstraction;

#ifndef IBUTTONSERVICE_H
#define IBUTTONSERVICE_H
namespace IOServices
{
	class IButtonService
	{
	protected:
		CallBackGroup *_callBackGroup;
		IButtonService();
	public:
		void Add(ICallBack *callBack);
		void Add(void(*callBackPointer)(void *), void *parameters);
		void Remove(ICallBack *callBack);
		void Clear();
		virtual void Tick() = 0;

		static IButtonService* CreateButtonService(const HardwareAbstractionCollection *hardwareAbstractionCollection, void *config, unsigned int *sizeOut);
		static void IButtonService::TickCallBack(void *buttonService);
	};
}
#endif