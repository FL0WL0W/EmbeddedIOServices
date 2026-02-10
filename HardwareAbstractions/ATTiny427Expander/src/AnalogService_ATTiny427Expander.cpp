#include "AnalogService_ATTiny427Expander.h"

#ifdef ANALOGSERVICE_ATTINY427EXPANDER_H
namespace EmbeddedIOServices
{
    AnalogService_ATTiny427Expander::AnalogService_ATTiny427Expander(ATTiny427_ExpanderService *aTTiny427ExpanderService, uint8_t accumulate) : 
        _aTTiny427ExpanderService(aTTiny427ExpanderService),
        _analogEnableLow(aTTiny427ExpanderService->GetRegister(ADDRESS_ANALOG_ENABLE_LOW)),
        _analogEnableHigh(aTTiny427ExpanderService->GetRegister(ADDRESS_ANALOG_ENABLE_HIGH)),
        _accumulate(aTTiny427ExpanderService->GetRegister(ADDRESS_ANALOG_ACCUMULATE)),
        _poller(new ATTiny427_ExpanderService::ATTiny427_ExpanderPoller(aTTiny427ExpanderService, ADDRESS_ANALOG_VALUE_START, 27, [this](uint8_t *data){
            uint8_t pos = 0;
            _seq = data[pos++];
            const uint16_t analogEnable = (_analogEnableHigh << 8) | _analogEnableLow;
            if(analogEnable & 0b0000000000000010 && (pos += 2) > 0)
                _analogValues[0] = data[pos-2] | (data[pos-1] << 8);
            if(analogEnable & 0b0000000000000100 && (pos += 2) > 0)
                _analogValues[1] = data[pos-2] | (data[pos-1] << 8);
            if(analogEnable & 0b0000000000001000 && (pos += 2) > 0)
                _analogValues[2] = data[pos-2] | (data[pos-1] << 8);
            if(analogEnable & 0b0000000000010000 && (pos += 2) > 0)
                _analogValues[3] = data[pos-2] | (data[pos-1] << 8);
            if(analogEnable & 0b0000000000100000 && (pos += 2) > 0)
                _analogValues[4] = data[pos-2] | (data[pos-1] << 8);
            if(analogEnable & 0b0000000001000000 && (pos += 2) > 0)
                _analogValues[5] = data[pos-2] | (data[pos-1] << 8);
            if(analogEnable & 0b0000000010000000 && (pos += 2) > 0)
                _analogValues[6] = data[pos-2] | (data[pos-1] << 8);
            if(analogEnable & 0b0000000100000000 && (pos += 2) > 0)
                _analogValues[7] = data[pos-2] | (data[pos-1] << 8);
            if(analogEnable & 0b0000001000000000 && (pos += 2) > 0)
                _analogValues[8] = data[pos-2] | (data[pos-1] << 8);
            if(analogEnable & 0b0000010000000000 && (pos += 2) > 0)
                _analogValues[9] = data[pos-2] | (data[pos-1] << 8);
            if(analogEnable & 0b0000100000000000 && (pos += 2) > 0)
                _analogValues[10] = data[pos-2] | (data[pos-1] << 8);
            if(analogEnable & 0b0001000000000000 && (pos += 2) > 0)
                _analogValues[11] = data[pos-2] | (data[pos-1] << 8);
            if(analogEnable & 0b0010000000000000 && (pos += 2) > 0)
                _analogValues[12] = data[pos-2] | (data[pos-1] << 8);
            if(analogEnable & 0b0100000000000000 && (pos += 2) > 0)
                _analogValues[13] = data[pos-2] | (data[pos-1] << 8);
            if(analogEnable & 0b1000000000000000 && (pos += 2) > 0)
                _analogValues[14] = data[pos-2] | (data[pos-1] << 8);
        }))
    {
        _analogEnableLow = 0x00;
        _analogEnableHigh = 0x00;
        _accumulate = (_accumulate & 0x70) | 0x80 | (accumulate & 0x0F);// Reset and Accumulate
    }

    AnalogService_ATTiny427Expander::~AnalogService_ATTiny427Expander()
    {
        delete _poller;
    }

	void AnalogService_ATTiny427Expander::InitPin(analogpin_t pin)
	{
		switch(_aTTiny427ExpanderService->Comm)
		{
			case ATTiny427_ExpanderComm_SPI:
				if(pin < 5 && pin > 0)
					return;
				break;
			case ATTiny427_ExpanderComm_SPIAlternate:
				if(pin > 15 && pin < 20)
					return;
				break;
			case ATTiny427_ExpanderComm_UART0Alternate:
			case ATTiny427_ExpanderComm_UART1:
				if(pin < 3 && pin > 0)
					return;
				break;
			case ATTiny427_ExpanderComm_UART1Alternate:
				if(pin > 9 && pin < 12)
					return;
				break;
			default:
				break;
		}
        const AnalogChannel_ATTiny427Expander analogChannel = PinToAnalogChannel(pin);
        if(analogChannel == -1)
            return;
        uint16_t analogEnable = (_analogEnableHigh << 8) | _analogEnableLow;
        if(analogEnable & (0x1 << analogChannel))
            return;//already enabled

        //disable poller while changing settings
        _poller->SetEnabled(false);

        //enable analog on pin
        if(analogChannel < 8)
            _analogEnableLow |= (0x1 << analogChannel);
        else
            _analogEnableHigh |= (0x1 << (analogChannel - 8));
        analogEnable |= (0x1 << analogChannel);

        //reset accumulate
        _accumulate = _accumulate | 0x80; //must be written like this because reset bit is reset on attiny side

        //count enabled channels
        uint8_t length = 0;
        for(int ch = 1; ch <= 15; ++ch)
        {
            if(analogEnable & (0x1 << ch))
                length++;
        }
        if(length == 1)
        {
            _aTTiny427ExpanderService->Write(ADDRESS_ADC0_CTRLA, 0b00100001); //enable ADC with low latency
            _aTTiny427ExpanderService->Write(ADDRESS_ADC0_CTRLB, 0x01); //prescaler DIV4 to get ADCCLK 5MHZ
            _aTTiny427ExpanderService->Write(ADDRESS_ADC0_CTRLC, 0xA0); //set timebase and VDD reference
            _aTTiny427ExpanderService->Write(ADDRESS_ADC0_INTCTRL, 0x01); //enable RESRDY interrupt
            _aTTiny427ExpanderService->Write(ADDRESS_ADC0_CTRLE, 0x05); //SAMPDUR = 5. TODO add this as an adjustable parameter from analogservice
            _aTTiny427ExpanderService->Write(ADDRESS_ADC0_CTRLF, 0x00); //no accumulation. accumulation done in software so the readings are evenly spaced
            _aTTiny427ExpanderService->Write(ADDRESS_ADC0_COMMAND, 0x11); //single 12 bit mode and start
        }

        //set poller length
        _poller->SetLength(length * 2 + 1);

        //re-enable poller
        _poller->SetEnabled(true);
	}
	
	float AnalogService_ATTiny427Expander::ReadPin(analogpin_t pin)
	{
        const AnalogChannel_ATTiny427Expander analogChannel = PinToAnalogChannel(pin);
        if(analogChannel == -1)
            return 0;
        uint16_t analogValue;
        uint8_t seq;
        do
        {
            seq = _seq;
            analogValue = _analogValues[analogChannel - 1];
        } while (seq != _seq);
        
        return analogValue * (5.0f / (((1 << 12) - 1) * ((_accumulate & 0x0F) + 1)));
	}
}
#endif
