#include "IOServices/ButtonService/ButtonService_Polling.h"
#include "IOServices/ButtonService/IButtonService.h"
#include "Service/HardwareAbstractionServiceBuilder.h"
#include "Service/ServiceBuilder.h"

using namespace HardwareAbstraction;
using namespace Service;

#ifdef IBUTTONSERVICE_H
namespace IOServices
{
	IButtonService::IButtonService()
	{
		_callBackGroup = new CallBackGroup();
	}
	
	void IButtonService::Add(ICallBack *callBack)
	{
		_callBackGroup->Add(callBack);
	}
	
	void IButtonService::Remove(ICallBack *callBack)
	{
		_callBackGroup->Remove(callBack);
	}
	
	void IButtonService::Clear()
	{
		_callBackGroup->Clear();
	}
	
	void* IButtonService::BuildButtonService(const ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
	{
		IButtonService *ret = CreateButtonService(serviceLocator->LocateAndCast<const HardwareAbstractionCollection>(HARDWARE_ABSTRACTION_COLLECTION_ID), config, sizeOut);
		
		if(ret != 0)
		{
			serviceLocator->LocateAndCast<CallBackGroup>(TICK_CALL_BACK_GROUP)->Add(
				new CallBack<IButtonService>(ret, &IButtonService::Tick));
		}

		return ret;
	}
	
	IButtonService* IButtonService::CreateButtonService(const HardwareAbstractionCollection *hardwareAbstractionCollection, const void *config, unsigned int &sizeOut)
	{
		sizeOut = 0;		
		IButtonService *buttonService = 0;

		switch (ServiceBuilder::CastAndOffset<uint8_t>(config, sizeOut))
		{
#ifdef BUTTONSERVICE_POLLING_H
		case 1:
			{
				unsigned int size;
				IBooleanInputService *booleanInputService = IBooleanInputService::CreateBooleanInputService(hardwareAbstractionCollection, config, size);
				config = reinterpret_cast<const uint8_t *>(config) + size;
				sizeOut += size;

				buttonService = new ButtonService_Polling(hardwareAbstractionCollection->TimerService, booleanInputService);
				break;
			}
#endif
		}
		
		return buttonService;
	}
	
}
#endif
