#include "Service/HardwareAbstractionServiceBuilder.h"
#include "Service/OperationBusServiceBuilderIds.h"
#include "Service/VariableBusServiceBuilderIds.h"
#include "VariableBus/IVariableService.h"
#include "Operations/IOperation.h"
#include "stdint.h"
#include "Packed.h"
#include "Interpolation.h"
#include <tuple>

#ifndef VARIABLESERVICE_DIGITALPINRECORD_H
#define VARIABLESERVICE_DIGITALPINRECORD_H
namespace VariableBus
{
	PACK(struct Frame
	{
		public:
		uint32_t Tick;
		bool State;
		bool Valid;
	});
	struct Record
	{
		public:
		Record(uint8_t length)
		{
			Length = length;
			const uint32_t byteLength = sizeof(Frame) * length;
			Frames = (Frame *)calloc(byteLength, byteLength);
		}
		static uint8_t Subtract(const uint8_t &val1, uint8_t val2, const uint8_t &length)
		{
			val2 %= length;
			if(val2 > val1)
				return length - (val2 - val1);
			return val1 - val2;
		}
		static uint8_t Add(const uint8_t &val1, uint16_t val2, const uint8_t &length)
		{
			val2 %= length;
			if(val1 + val2 > length)
				return (val1 + val2) - length;
			return val1 + val2;
		}

		uint8_t Length; //Don't modify this -_-
		uint8_t Last = 0;
		Frame *Frames;
	};

	class VariableService_DigitalPinRecord : public IVariableService
	{
	protected:
		HardwareAbstraction::IDigitalService *_digitalService;
		HardwareAbstraction::ITimerService *_timerService;
		uint16_t _pin;
		bool _inverted;
		Record *_record;
	public:	
        VariableService_DigitalPinRecord(HardwareAbstraction::IDigitalService *digitalService, HardwareAbstraction::ITimerService *timerService, uint8_t length, uint16_t pin, bool inverted);
		void TranslateValue() override;
		void InterruptCallBack();

		static IVariableService *Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut);
		ISERVICE_REGISTERFACTORY_H
	};
}
#endif