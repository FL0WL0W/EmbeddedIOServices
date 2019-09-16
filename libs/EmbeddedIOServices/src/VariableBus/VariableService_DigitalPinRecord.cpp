#include "VariableBus/VariableService_DigitalPinRecord.h"

#ifdef VARIABLESERVICE_DIGITALPINRECORD_H
namespace VariableBus
{
	VariableService_DigitalPinRecord::VariableService_DigitalPinRecord(HardwareAbstraction::IDigitalService *digitalService, HardwareAbstraction::ITimerService *timerService, uint8_t length, uint16_t pin, bool inverted)
	{
		_digitalService = digitalService;
		_timerService = timerService;
		_pin = pin;
		_inverted = inverted;
		_record = new Record(length);

		_digitalService->ScheduleRecurringInterrupt(_pin, new HardwareAbstraction::CallBack<VariableService_DigitalPinRecord>(this, &VariableService_DigitalPinRecord::InterruptCallBack));
	}

	void VariableService_DigitalPinRecord::TranslateValue()
	{
	}

	void VariableService_DigitalPinRecord::InterruptCallBack()
	{
		bool state = _digitalService->ReadPin(_pin);
		uint32_t tick = _timerService->GetTick();
		if(_inverted)
			state = !state;
		uint8_t last = _record->Last;
		//only record toggles
		if(state == _record->Frames[last].State && _record->Frames[last].Valid)
			return;
		last++;
		if(last >= _record->Length)
			last = 0;
		_record->Frames[last].State = state;
		_record->Frames[last].Tick = tick;
		_record->Frames[last].Valid = true;
		_record->Last = last;
	}

	IVariableService *VariableService_DigitalPinRecord::Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
	{
		uint32_t variableId = Service::IService::CastAndOffset<uint16_t>(config, sizeOut);
		uint32_t length = Service::IService::CastAndOffset<uint8_t>(config, sizeOut);
		const uint16_t pin = IService::CastAndOffset<uint16_t>(config, sizeOut);
		const bool inverted = IService::CastAndOffset<bool>(config, sizeOut);
					
		VariableService_DigitalPinRecord *variableService = new VariableService_DigitalPinRecord(serviceLocator->LocateAndCast<HardwareAbstraction::IDigitalService>(DIGITAL_SERVICE_ID), serviceLocator->LocateAndCast<HardwareAbstraction::ITimerService>(TIMER_SERVICE_ID), length, pin, inverted);

		serviceLocator->Register(BUILDER_VARIABLEBUS, variableId, variableService->_record);

		return variableService;
	}
	ISERVICE_REGISTERFACTORY_CPP(VariableService_DigitalPinRecord, 4)
}
#endif
