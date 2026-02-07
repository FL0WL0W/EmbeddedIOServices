#include "DigitalService_ATTiny427Expander.h"

#ifdef DIGITALSERVICE_ATTINY427EXPANDER_H
namespace EmbeddedIOServices
{
    DigitalService_ATTiny427Expander::DigitalService_ATTiny427Expander(ATTiny427_ExpanderService *aTTiny427ExpanderService) : 
		_VPORTA_DIR(aTTiny427ExpanderService->GetRegister(ADDRESS_VPORTA_DIR)),
		_VPORTB_DIR(aTTiny427ExpanderService->GetRegister(ADDRESS_VPORTB_DIR)),
		_VPORTC_DIR(aTTiny427ExpanderService->GetRegister(ADDRESS_VPORTC_DIR)),
		_VPORTA_OUT(aTTiny427ExpanderService->GetRegister(ADDRESS_VPORTA_OUT)),
		_VPORTB_OUT(aTTiny427ExpanderService->GetRegister(ADDRESS_VPORTB_OUT)),
		_VPORTC_OUT(aTTiny427ExpanderService->GetRegister(ADDRESS_VPORTC_OUT)),
		_PORTA_Poller(new ATTiny427_ExpanderService::ATTiny427_ExpanderPoller(aTTiny427ExpanderService, ADDRESS_VPORTA_IN, 1, [this](uint8_t *data)
		{
			const uint8_t pORTA_IN_Toggle = _PORTA_IN ^ data[0];
			_PORTA_IN = data[0];
			if(pORTA_IN_Toggle == 0)
				return;

			for (DigitalInterruptList_ATTiny427Expander::iterator interrupt = _PORTAInterruptList.begin(); interrupt != _PORTAInterruptList.end(); ++interrupt)
			{
				const uint8_t DigitalPin = interrupt->DigitalPin;
				if(pORTA_IN_Toggle & DigitalPin)
					interrupt->CallBack();
				break;
			}
		})),
		_PORTB_Poller(new ATTiny427_ExpanderService::ATTiny427_ExpanderPoller(aTTiny427ExpanderService, ADDRESS_VPORTB_IN, 1, [this](uint8_t *data)
		{
			const uint8_t pORTB_IN_Toggle = _PORTB_IN ^ data[0];
			_PORTB_IN = data[0];
			if(pORTB_IN_Toggle == 0)
				return;

			for (DigitalInterruptList_ATTiny427Expander::iterator interrupt = _PORTBInterruptList.begin(); interrupt != _PORTBInterruptList.end(); ++interrupt)
			{
				const uint8_t DigitalPin = interrupt->DigitalPin;
				if(pORTB_IN_Toggle & DigitalPin)
					interrupt->CallBack();
				break;
			}
		})),
		_PORTC_Poller(new ATTiny427_ExpanderService::ATTiny427_ExpanderPoller(aTTiny427ExpanderService, ADDRESS_VPORTC_IN, 1, [this](uint8_t *data)
		{
			const uint8_t pORTC_IN_Toggle = _PORTC_IN ^ data[0];
			_PORTC_IN = data[0];
			if(pORTC_IN_Toggle == 0)
				return;

			for (DigitalInterruptList_ATTiny427Expander::iterator interrupt = _PORTCInterruptList.begin(); interrupt != _PORTCInterruptList.end(); ++interrupt)
			{
				const uint8_t DigitalPin = interrupt->DigitalPin;
				if(pORTC_IN_Toggle & DigitalPin)
					interrupt->CallBack();
				break;
			}
		}))
    {        
		_PORTA_Poller->SetLength(1);
		_PORTB_Poller->SetLength(1);
		_PORTC_Poller->SetLength(1);
		_PORTA_Poller->SetEnabled(true);
		_PORTB_Poller->SetEnabled(true);
		_PORTC_Poller->SetEnabled(true);
    }
	DigitalService_ATTiny427Expander::~DigitalService_ATTiny427Expander()
	{
		delete _PORTA_Poller;
		delete _PORTB_Poller;
		delete _PORTC_Poller;
	}

	void DigitalService_ATTiny427Expander::InitPin(digitalpin_t pin, PinDirection direction)
	{
		const DigitalPort_ATTiny427Expander DigitalPort = PinToDigitalPort(pin);
		const DigitalPin_ATTiny427Expander DigitalPin = PinToDigitalPin(pin);
		if(direction == In)
		{
			switch(DigitalPort)
			{
				case PORTA:
					_VPORTA_DIR &= ~DigitalPin;
					return;
				case PORTB:
					_VPORTB_DIR &= ~DigitalPin;
					return;
				case PORTC:
					_VPORTC_DIR &= ~DigitalPin;
					return;
			}
		}

		switch(DigitalPort)
		{
			case PORTA:
				_VPORTA_DIR |= DigitalPin;
				return;
			case PORTB:
				_VPORTB_DIR |= DigitalPin;
				return;
			case PORTC:
				_VPORTC_DIR |= DigitalPin;
				return;
		}
	}
	bool DigitalService_ATTiny427Expander::ReadPin(digitalpin_t pin)
	{
		const DigitalPort_ATTiny427Expander DigitalPort = PinToDigitalPort(pin);
		const DigitalPin_ATTiny427Expander DigitalPin = PinToDigitalPin(pin);
		switch(DigitalPort)
		{
			case PORTA:
				return _PORTA_IN & DigitalPin;
			case PORTB:
				return _PORTB_IN & DigitalPin;
			case PORTC:
				return _PORTC_IN & DigitalPin;
			default:
				return false;
		}
	}
	void DigitalService_ATTiny427Expander::WritePin(digitalpin_t pin, bool value)
	{
		const DigitalPort_ATTiny427Expander DigitalPort = PinToDigitalPort(pin);
		const DigitalPin_ATTiny427Expander DigitalPin = PinToDigitalPin(pin);

		if(value == true)
		{
			switch(DigitalPort)
			{
				case PORTA:
					_VPORTA_OUT |= DigitalPin;
					return;
				case PORTB:
					_VPORTB_OUT |= DigitalPin;
					return;
				case PORTC:
					_VPORTC_OUT |= DigitalPin;
					return;
			}
		}

		switch(DigitalPort)
		{
			case PORTA:
				_VPORTA_OUT &= ~DigitalPin;
				return;
			case PORTB:
				_VPORTB_OUT &= ~DigitalPin;
				return;
			case PORTC:
				_VPORTC_OUT &= ~DigitalPin;
				return;
		}
	}
	void DigitalService_ATTiny427Expander::AttachInterrupt(digitalpin_t pin, callback_t callBack)
	{
		const DigitalPort_ATTiny427Expander DigitalPort = PinToDigitalPort(pin);
		const DigitalPin_ATTiny427Expander DigitalPin = PinToDigitalPin(pin);
		switch(DigitalPort)
		{
			case PORTA:
				_PORTAInterruptList.push_front(DigitalInterrupt_ATTiny427Expander(DigitalPin, callBack));
				return;
			case PORTB:
				_PORTBInterruptList.push_front(DigitalInterrupt_ATTiny427Expander(DigitalPin, callBack));
				return;
			case PORTC:
				_PORTCInterruptList.push_front(DigitalInterrupt_ATTiny427Expander(DigitalPin, callBack));
				return;
		}
	}
	void DigitalService_ATTiny427Expander::DetachInterrupt(digitalpin_t pin)
	{
		const DigitalPort_ATTiny427Expander DigitalPort = PinToDigitalPort(pin);
		const DigitalPin_ATTiny427Expander DigitalPin = PinToDigitalPin(pin);
		switch(DigitalPort)
		{
			case PORTA:
				_PORTAInterruptList.remove_if([DigitalPin](const DigitalInterrupt_ATTiny427Expander& interrupt) { return interrupt.DigitalPin == DigitalPin; });
				return;
			case PORTB:
				_PORTBInterruptList.remove_if([DigitalPin](const DigitalInterrupt_ATTiny427Expander& interrupt) { return interrupt.DigitalPin == DigitalPin; });
				return;
			case PORTC:
				_PORTCInterruptList.remove_if([DigitalPin](const DigitalInterrupt_ATTiny427Expander& interrupt) { return interrupt.DigitalPin == DigitalPin; });
				return;
		}
		
	}
}
#endif
