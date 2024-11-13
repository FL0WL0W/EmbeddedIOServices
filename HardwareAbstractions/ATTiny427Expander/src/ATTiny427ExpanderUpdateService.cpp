#include "ATTiny427ExpanderUpdateService.h"
#include <cstring>
#include "esp_log.h"

#ifdef ATTINY427EXPANDERUPDATESERVICE_H
namespace EmbeddedIOServices
{
    ATTiny427ExpanderUpdateService::ATTiny427ExpanderUpdateService(ATTiny427Expander_Registers *registers) : _registers(registers)
    {
		_previousRegisters = *_registers; 
    }
    void ATTiny427ExpanderUpdateService::Receive(uint8_t *data, size_t size)
    {
        size_t dataIndex = 0;
        while(dataIndex < size)
        {
            switch(_receiveState)
            {
                default: // waiting on command
                    {
                        const uint8_t cmd = data[dataIndex++];
                        _receiveLength = cmd & 0xF;
                        if(_receiveLength > 0)
                        {
                            if(cmd & 0x40)
                            {
                                if(cmd & 0x20)
                                {
                                    _receiveAddress = 0;
                                }
                                _receiveState = 2;
                            }
                            else
                            {
                                _receiveState = 1;
                            }
                        }
                    }
                    break;
                case 1: // receive High Address
                    _receiveAddress = data[dataIndex++] << 8;
                    _receiveState = 2;
                    break;
                case 2: // receive Low Address
                    _receiveAddress |= data[dataIndex++];
                    _receiveState = 3;
                    _receiveIndex = 0;
                    break;
                case 3: // receive data
                    switch(_receiveAddress)
                    {
                        case 0x1C: //reading AnalogEnable pins and then Analog Values
                            if(_receiveIndex > 3)
                            {
                                if(_receiveIndex % 2 == 0)
                                {
                                    _receiveAnalogValueHighByte = data[dataIndex++];
                                    _receiveIndex++;
                                    while(!(_receiveAnalogEnable & 0x1) && _receiveIndex < 36)
                                    {
                                        _receiveIndex += 2;
                                        _receiveAnalogEnable >>= 1;
                                    }
                                }
                                else
                                {
                                    _registers->AnalogValues[(_receiveIndex - 6)/2] = (_receiveAnalogValueHighByte << 8) | data[dataIndex++];
                                    _receiveAnalogEnable >>= 1;
                                    _receiveIndex++;
                                    if(_receiveAnalogEnable == 0)
                                    {
                                        _receiveState = 0;
                                        _registers->AnalogCounter = _receiveAnalogCounter;
                                    }
                                }
                            }
                            else if(_receiveIndex == 0)
                            {
                                _receiveAnalogEnable = data[dataIndex++];
                                _receiveIndex++;
                            }
                            else if(_receiveIndex == 1)
                            {
                                _receiveAnalogEnable |= data[dataIndex++] << 8;
                                _receiveIndex++;
                            }
                            else if(_receiveIndex == 2)
                            {
                                dataIndex++;
                                _receiveIndex++;
                            }
                            else if(_receiveIndex == 3)
                            {
                                _receiveAnalogCounter = data[dataIndex++];
                                _receiveIndex++;
                            }
                            break;
                        case 0x0A: //reading GPIO
                            switch(_receiveIndex)
                            {
                                case 0:
                                    _registers->PORTC_IN = data[dataIndex++];
                                    break;
                                case 2:
                                    _registers->PORTB_IN = data[dataIndex++];
                                    break;
                                case 4:
                                    _registers->PORTA_IN = data[dataIndex++];
                                    _receiveState = 0;
                                    break;
                                default: 
                                    dataIndex++;
                                    break;
                            }
                            _receiveIndex++;
                            break;
                        case 0x3400: //reading TCB0
                            switch(_receiveIndex)
                            {
                                case 0:
                                    _receiveTcbCounter = data[dataIndex++];
                                    _receiveIndex++;
                                    break;
                                case 1:
                                    _receiveTcbBuffer[0] = data[dataIndex++];
                                    _receiveIndex++;
                                    break;
                                case 2:
                                    _receiveTcbBuffer[1] = data[dataIndex++];
                                    _receiveIndex++;
                                    break;
                                case 3:
                                    _receiveTcbBuffer[2] = data[dataIndex++];
                                    _receiveIndex++;
                                    break;
                                case 4:
                                    _registers->TCB0VALUE[0] = _receiveTcbBuffer[0];
                                    _registers->TCB0VALUE[1] = _receiveTcbBuffer[1];
                                    _registers->TCB0VALUE[2] = _receiveTcbBuffer[2];
                                    _registers->TCB0VALUE[2] = data[dataIndex++];
                                    _receiveIndex = 0;
                                    if(_receiveLength < 6)
                                        _receiveState = 0;
                                    break;
                            }
                        case 0x3405: //reading TCB1
                            switch(_receiveIndex)
                            {
                                case 0:
                                    _receiveTcbCounter = data[dataIndex++];
                                    _receiveIndex++;
                                    break;
                                case 1:
                                    _receiveTcbBuffer[0] = data[dataIndex++];
                                    _receiveIndex++;
                                    break;
                                case 2:
                                    _receiveTcbBuffer[1] = data[dataIndex++];
                                    _receiveIndex++;
                                    break;
                                case 3:
                                    _receiveTcbBuffer[2] = data[dataIndex++];
                                    _receiveIndex++;
                                    break;
                                case 4:
                                    _registers->TCB1VALUE[0] = _receiveTcbBuffer[0];
                                    _registers->TCB1VALUE[1] = _receiveTcbBuffer[1];
                                    _registers->TCB1VALUE[2] = _receiveTcbBuffer[2];
                                    _registers->TCB1VALUE[2] = data[dataIndex++];
                                    _receiveState = 0;
                                    break;
                            }

                        default:
                            if(data[dataIndex++] == 0)
                                _receiveState = 0;
                            break;
                            
                    }
                    break;
            }
        }
    }
    size_t ATTiny427ExpanderUpdateService::Transmit(uint8_t data[1024])
    {
        ATTiny427Expander_Registers transmitRegisters = *_registers;
        //make sure we don't lock ourselves out
        switch(transmitRegisters.Comm)
        {
            case UART0:
                transmitRegisters.PORTB_DIR &= 0b11110011;
                transmitRegisters.PORTB_DIR |= 0b00000100;
                break;
            case UART0Alternate:
            case UART1:
                transmitRegisters.PORTA_DIR &= 0b11111000;
                transmitRegisters.PORTA_DIR |= 0b00000010;
                transmitRegisters.AnalogEnable &= 0xFFF8;
                break;
            case UART1Alternate:
                transmitRegisters.PORTC_DIR &= 0b11110011;
                transmitRegisters.PORTC_DIR |= 0b00000100;
                transmitRegisters.AnalogEnable &= 0xFFCF;
                break;
            case SPI:
                transmitRegisters.PORTA_DIR &= 0b11100000;
                transmitRegisters.PORTA_DIR |= 0b00000100;
                transmitRegisters.AnalogEnable &= 0xFFE0;
                break;
            case SPIAlternate:
                transmitRegisters.PORTC_DIR &= 0b11110000;
                transmitRegisters.PORTC_DIR |= 0b00000010;
                transmitRegisters.AnalogEnable &= 0x0FFE;
                break;
        }

        const bool resetAccumulate = transmitRegisters.AnalogAccumulate != _previousRegisters.AnalogAccumulate || transmitRegisters.AnalogEnable != _previousRegisters.AnalogEnable || _first;
        const bool analogStop = transmitRegisters.AnalogEnable == 0 && (_analogRunning || _first);
        const bool analogStart = transmitRegisters.AnalogEnable != 0 && (!_analogRunning || _first);
        const bool GPIOR0_changed = transmitRegisters.GPIOR0 != _previousRegisters.GPIOR0 || _first;
        const bool GPIOR1_changed = transmitRegisters.GPIOR1 != _previousRegisters.GPIOR1 || _first;
        const bool GPIOR2_changed = transmitRegisters.GPIOR2 != _previousRegisters.GPIOR2 || _first;
        const bool PORTC_OUT_changed = transmitRegisters.PORTC_OUT != _previousRegisters.PORTC_OUT || _first;
        const bool PORTB_OUT_changed = transmitRegisters.PORTB_OUT != _previousRegisters.PORTB_OUT || _first;
        const bool PORTA_OUT_changed = transmitRegisters.PORTA_OUT != _previousRegisters.PORTA_OUT || _first;
        const bool PORTC_DIR_changed = transmitRegisters.PORTC_DIR != _previousRegisters.PORTC_DIR || _first;
        const bool PORTB_DIR_changed = transmitRegisters.PORTB_DIR != _previousRegisters.PORTB_DIR || _first;
        const bool PORTA_DIR_changed = transmitRegisters.PORTA_DIR != _previousRegisters.PORTA_DIR || _first;
        const bool AC_CTRLA_changed = transmitRegisters.AC_CTRLA != _previousRegisters.AC_CTRLA || _first;
        const bool AC_MUXCTRLA_changed = transmitRegisters.AC_MUXCTRLA != _previousRegisters.AC_MUXCTRLA || _first;
        const bool AC_DACREF_changed = transmitRegisters.AC_DACREF != _previousRegisters.AC_DACREF || _first;

		const bool PORTMUX_EVSYSROUTEA_changed = transmitRegisters.PORTMUX_EVSYSROUTEA != _previousRegisters.PORTMUX_EVSYSROUTEA || _first;
		const bool PORTMUX_CCLROUTEA_changed = transmitRegisters.PORTMUX_CCLROUTEA != _previousRegisters.PORTMUX_CCLROUTEA || _first;
        const bool EVSYS_CHANNEL_changed =  transmitRegisters.EVSYS_CHANNEL0 != _previousRegisters.EVSYS_CHANNEL0 ||
                                            transmitRegisters.EVSYS_CHANNEL1 != _previousRegisters.EVSYS_CHANNEL1 ||
                                            transmitRegisters.EVSYS_CHANNEL2 != _previousRegisters.EVSYS_CHANNEL2 ||
                                            transmitRegisters.EVSYS_CHANNEL3 != _previousRegisters.EVSYS_CHANNEL3 ||
                                            transmitRegisters.EVSYS_CHANNEL4 != _previousRegisters.EVSYS_CHANNEL4 ||
                                            transmitRegisters.EVSYS_CHANNEL5 != _previousRegisters.EVSYS_CHANNEL5 ||
                                            _first;

        const bool EVSYS_USER_changed = transmitRegisters.EVSYS_CCL_LUT0A != _previousRegisters.EVSYS_CCL_LUT0A ||
                                        transmitRegisters.EVSYS_CCL_LUT1A != _previousRegisters.EVSYS_CCL_LUT1A ||
                                        transmitRegisters.EVSYS_CCL_LUT2A != _previousRegisters.EVSYS_CCL_LUT2A ||
                                        transmitRegisters.EVSYS_CCL_LUT3A != _previousRegisters.EVSYS_CCL_LUT3A ||
                                        transmitRegisters.EVSYS_EVOUTA != _previousRegisters.EVSYS_EVOUTA ||
                                        transmitRegisters.EVSYS_EVOUTB != _previousRegisters.EVSYS_EVOUTB ||
                                        transmitRegisters.EVSYS_EVOUTC != _previousRegisters.EVSYS_EVOUTC ||
                                        _first;
                                            
        const bool EVSYS_TCB0_changed = transmitRegisters.EVSYS_TCB0_CAPT != _previousRegisters.EVSYS_TCB0_CAPT || _first;
        const bool EVSYS_TCB1_changed = transmitRegisters.EVSYS_TCB1_CAPT != _previousRegisters.EVSYS_TCB1_CAPT || _first;

        const bool CCL_CTRLA_changed = transmitRegisters.CCL_CTRLA != _previousRegisters.CCL_CTRLA || _first;

        const bool CCL_LUT0_changed =   transmitRegisters.CCL_LUT0CTRLA != _previousRegisters.CCL_LUT0CTRLA ||
                                        transmitRegisters.CCL_LUT0CTRLB != _previousRegisters.CCL_LUT0CTRLB ||
                                        transmitRegisters.CCL_LUT0CTRLC != _previousRegisters.CCL_LUT0CTRLC ||
                                        transmitRegisters.CCL_LUT0TRUTH != _previousRegisters.CCL_LUT0TRUTH ||
                                        _first;
        const bool CCL_LUT1_changed =   transmitRegisters.CCL_LUT1CTRLA != _previousRegisters.CCL_LUT1CTRLA ||
                                        transmitRegisters.CCL_LUT1CTRLB != _previousRegisters.CCL_LUT1CTRLB ||
                                        transmitRegisters.CCL_LUT1CTRLC != _previousRegisters.CCL_LUT1CTRLC ||
                                        transmitRegisters.CCL_LUT1TRUTH != _previousRegisters.CCL_LUT1TRUTH ||
                                        _first;
        const bool CCL_LUT2_changed =   transmitRegisters.CCL_LUT2CTRLA != _previousRegisters.CCL_LUT2CTRLA ||
                                        transmitRegisters.CCL_LUT2CTRLB != _previousRegisters.CCL_LUT2CTRLB ||
                                        transmitRegisters.CCL_LUT2CTRLC != _previousRegisters.CCL_LUT2CTRLC ||
                                        transmitRegisters.CCL_LUT2TRUTH != _previousRegisters.CCL_LUT2TRUTH ||
                                        _first;
        const bool CCL_LUT3_changed =   transmitRegisters.CCL_LUT3CTRLA != _previousRegisters.CCL_LUT3CTRLA ||
                                        transmitRegisters.CCL_LUT3CTRLB != _previousRegisters.CCL_LUT3CTRLB ||
                                        transmitRegisters.CCL_LUT3CTRLC != _previousRegisters.CCL_LUT3CTRLC ||
                                        transmitRegisters.CCL_LUT3TRUTH != _previousRegisters.CCL_LUT3TRUTH ||
                                        _first;

        uint16_t dataIndex = 0;
        uint16_t currentReadFinishedIndex = 0;
        int8_t state = 0;
        bool writeGPIOR = false;
        bool readGPIO = false;
        bool readTCB = false;

        while(state != -1)
        {
            if(writeGPIOR && currentReadFinishedIndex <= dataIndex)
            {
                if(GPIOR0_changed)
                {
                    data[dataIndex++] = 0xE3;
                    data[dataIndex++] = 0x1C;
                    data[dataIndex++] = transmitRegisters.GPIOR0;
                    data[dataIndex++] = transmitRegisters.GPIOR1;
                    data[dataIndex++] = transmitRegisters.GPIOR2 | 0x80;
                }
                else if(GPIOR1_changed)
                {
                    data[dataIndex++] = 0xE2;
                    data[dataIndex++] = 0x1D;
                    data[dataIndex++] = transmitRegisters.GPIOR1;
                    data[dataIndex++] = transmitRegisters.GPIOR2 | 0x80;
                }
                else if(GPIOR2_changed || resetAccumulate)
                {
                    data[dataIndex++] = 0xE1;
                    data[dataIndex++] = 0x1E;
                    data[dataIndex++] = transmitRegisters.GPIOR2 | (resetAccumulate? 0x80 : 0x00);
                }

                if(analogStart)
                {
                    data[dataIndex++] = 0x85; //write 16bit address
                    data[dataIndex++] = 0x06; //address high
                    data[dataIndex++] = 0x00; //address low
                    data[dataIndex++] = 0b00100001; //enable ADC with low latency
                    data[dataIndex++] = 0x01; //prescaler DIV4 to get ADCCLK 5MHZ
                    data[dataIndex++] = 0xA0; //set timebase and VDD reference
                    data[dataIndex++] = 0x00; //no window source mode
                    data[dataIndex++] = 0x01; //enable RESRDY interrupt
                    data[dataIndex++] = 0xC3; //write 3 bytes to 8 bit address using existing high byte
                    data[dataIndex++] = 0x08; //address low
                    data[dataIndex++] = 0x05; //SAMPDUR = 5. TODO add this as an adjustable parameter from analogservice
                    data[dataIndex++] = 0x00; //co accumulation. accumulation done in software so the readings are evenly spaced
                    data[dataIndex++] = 0x11; //single 12 bit mode and start
                    _analogRunning = true;
                }

                writeGPIOR = false;
            }
            if(readGPIO && currentReadFinishedIndex <= dataIndex)
            {
                data[dataIndex++] = 0x61; //read 1 byte from 8 bit address with 0 as high address byte
                data[dataIndex++] = 0x0A; //address
                data[dataIndex++] = 0x61; //read 1 byte from 8 bit address with 0 as high address byte
                data[dataIndex++] = 0x06; //address
                data[dataIndex++] = 0x61; //read 1 byte from 8 bit address with 0 as high address byte
                data[dataIndex++] = 0x02; //address
                currentReadFinishedIndex = dataIndex + 1;
                readGPIO = false;
            }
            if(readTCB && currentReadFinishedIndex <= dataIndex)
            {

            }

            switch(state)
            {
                case 0:
                    writeGPIOR = true;
                    if(_previousRegisters.AnalogEnable != 0 && _analogRunning)
                    {
                        //count number of analog channels
                        uint8_t analogChannelCount = 0;
                        if(_previousRegisters.AnalogEnable & 0b0000000000000010)
                            analogChannelCount++;
                        if(_previousRegisters.AnalogEnable & 0b0000000000000100)
                            analogChannelCount++;
                        if(_previousRegisters.AnalogEnable & 0b0000000000001000)
                            analogChannelCount++;
                        if(_previousRegisters.AnalogEnable & 0b0000000000010000)
                            analogChannelCount++;
                        if(_previousRegisters.AnalogEnable & 0b0001000000000000)
                            analogChannelCount++;
                        if(_previousRegisters.AnalogEnable & 0b0010000000000000)
                            analogChannelCount++;
                        if(_previousRegisters.AnalogEnable & 0b0100000000000000)
                            analogChannelCount++;
                        if(_previousRegisters.AnalogEnable & 0b1000000000000000)
                            analogChannelCount++;
                        if(_previousRegisters.AnalogEnable & 0b0000000000100000)
                            analogChannelCount++;
                        if(_previousRegisters.AnalogEnable & 0b0000000001000000)
                            analogChannelCount++;
                        if(_previousRegisters.AnalogEnable & 0b0000000010000000)
                            analogChannelCount++;
                        if(_previousRegisters.AnalogEnable & 0b0000000100000000)
                            analogChannelCount++;
                        if(_previousRegisters.AnalogEnable & 0b0000001000000000)
                            analogChannelCount++;
                        if(_previousRegisters.AnalogEnable & 0b0000010000000000)
                            analogChannelCount++;
                        if(_previousRegisters.AnalogEnable & 0b0000100000000000)
                            analogChannelCount++;

                        //read AnalogEnable register 
                        data[dataIndex++] = 0x62;
                        data[dataIndex++] = 0x1C;

                        //then read analog values
                        if(analogChannelCount > 14)
                            data[dataIndex++] = analogChannelCount;
                        else if(analogChannelCount > 7)
                            data[dataIndex++] = analogChannelCount + 1;
                        else
                            data[dataIndex++] = analogChannelCount * 2 + 1;
                        data[dataIndex++] = 0x34;
                        data[dataIndex++] = 0x0D;

                        currentReadFinishedIndex = dataIndex + analogChannelCount * 2 + 1;

                        //if a big read, do follow on read command
                        if(analogChannelCount > 14)
                        {
                            data[dataIndex++] = 0x10 | analogChannelCount;
                            data[dataIndex++] = 0x11;
                        }
                        else if(analogChannelCount > 7)
                        {
                            data[dataIndex++] = 0x10 | analogChannelCount;
                        }
                    }
                    state++;
                    break;
                case 1:
                    if(analogStop)
                    {
                        //todo stop analog
                    }
                    state++;
                    break;
                case 2:
                    if(PORTC_OUT_changed)
                    {
                        data[dataIndex++] = 0xE1; //write 1 byte to 8 bit address with 0 as high address byte
                        data[dataIndex++] = 0x09; //address
                        data[dataIndex++] = transmitRegisters.PORTC_OUT; //OUT
                    }
                    state++;
                    break;
                case 3:
                    if(PORTB_OUT_changed)
                    {
                        data[dataIndex++] = PORTC_DIR_changed? 0xE3 : 0xE1; //write 1 or 3 bytes to 8 bit address with 0 as high address byte
                        data[dataIndex++] = 0x05; //address
                        data[dataIndex++] = transmitRegisters.PORTB_OUT; //OUT
                        if(PORTC_DIR_changed)
                        {
                            data[dataIndex++] = 0; //INTFlags. writing 0 to this does nothing
                            data[dataIndex++] = transmitRegisters.PORTC_DIR; //DIR
                        }
                    }
                    state++;
                    break;
                case 4:
                    if(PORTA_OUT_changed)
                    {
                        data[dataIndex++] = PORTB_DIR_changed? 0xE3 : 0xE1; //write 1 or 3 bytes to 8 bit address with 0 as high address byte
                        data[dataIndex++] = 0x01; //address
                        data[dataIndex++] = transmitRegisters.PORTA_OUT; //OUT
                        if(PORTB_DIR_changed)
                        {
                            data[dataIndex++] = 0; //INTFlags. writing 0 to this does nothing
                            data[dataIndex++] = transmitRegisters.PORTB_DIR; //DIR
                        }
                    }
                    state++;
                    break;
                case 5:
                    if(PORTC_DIR_changed && !PORTB_OUT_changed)
                    {
                        data[dataIndex++] = 0xE1; //write 1 byte to 8 bit address with 0 as high address byte
                        data[dataIndex++] = 0x08; //address
                        data[dataIndex++] = transmitRegisters.PORTC_DIR; //DIR

                    }
                    state++;
                    break;
                case 6:
                    if(PORTB_DIR_changed && !PORTA_OUT_changed)
                    {
                        data[dataIndex++] = 0xE1; //write 1 byte to 8 bit address with 0 as high address byte
                        data[dataIndex++] = 0x04; //address
                        data[dataIndex++] = transmitRegisters.PORTB_DIR; //DIR
                    }
                    state++;
                    break;
                case 7:
                    if(PORTA_DIR_changed)
                    {
                        data[dataIndex++] = 0xE1; //write 1 byte to 8 bit address with 0 as high address byte
                        data[dataIndex++] = 0x00; //address
                        data[dataIndex++] = transmitRegisters.PORTA_DIR; //DIR
                    }
                    readGPIO = true;
                    state++;
                    break;
                case 8:
                    if(AC_CTRLA_changed)
                    {
                        data[dataIndex++] = 0x81 + (AC_MUXCTRLA_changed && AC_DACREF_changed? 4 : (AC_MUXCTRLA_changed? 2 : 0)); //write 16bit address
                        data[dataIndex++] = 0x06; //address high
                        data[dataIndex++] = 0x80; //address low
                        data[dataIndex++] = transmitRegisters.AC_CTRLA; //CTRLA
                        if(AC_MUXCTRLA_changed)
                        {
                            data[dataIndex++] = 0; //nothing byte
                            data[dataIndex++] = transmitRegisters.AC_MUXCTRLA; //MUXCTRLA
                            if(AC_DACREF_changed)
                            {
                                data[dataIndex++] = 0; //nothing byte
                                data[dataIndex++] = transmitRegisters.AC_DACREF; //DACREF
                            }
                        }
                        else if(AC_DACREF_changed)
                        {
                            data[dataIndex++] = 0xC1; //write 1 byte to 8 bit address using existing high byte
                            data[dataIndex++] = 0x82; //low byte
                            data[dataIndex++] = transmitRegisters.AC_DACREF; //DACREF
                        }
                    }
                    else if(AC_MUXCTRLA_changed)
                    {
                        data[dataIndex++] = 0x81 + (AC_DACREF_changed? 2 : 0); //write 16bit address
                        data[dataIndex++] = 0x06; //address high
                        data[dataIndex++] = 0x82; //address low
                        data[dataIndex++] = transmitRegisters.AC_MUXCTRLA; //MUXCTRLA
                        if(AC_DACREF_changed)
                        {
                            data[dataIndex++] = 0; //nothing byte
                            data[dataIndex++] = transmitRegisters.AC_DACREF; //DACREF
                        }
                    }
                    else if(AC_DACREF_changed)
                    {
                        data[dataIndex++] = 0x81; //write 16bit address
                        data[dataIndex++] = 0x06; //address high
                        data[dataIndex++] = 0x84; //address low
                        data[dataIndex++] = transmitRegisters.AC_DACREF; //DACREF
                    }
                    state++;
                    break;
                case 9:
                    if(PORTMUX_EVSYSROUTEA_changed)
                    {
                        if(PORTMUX_CCLROUTEA_changed)
                        {
                            data[dataIndex++] = 0x82; //write 16bit address
                            data[dataIndex++] = 0x05; //address high
                            data[dataIndex++] = 0xE0; //address low
                            data[dataIndex++] = transmitRegisters.PORTMUX_EVSYSROUTEA; //PORTMUX_EVSYSROUTEA
                            data[dataIndex++] = transmitRegisters.PORTMUX_CCLROUTEA; //PORTMUX_CCLROUTEA
                        }
                        else
                        {
                            data[dataIndex++] = 0x81; //write 16bit address
                            data[dataIndex++] = 0x05; //address high
                            data[dataIndex++] = 0xE0; //address low
                            data[dataIndex++] = transmitRegisters.PORTMUX_EVSYSROUTEA; //PORTMUX_EVSYSROUTEA
                        }
                    }
                    else if(PORTMUX_CCLROUTEA_changed)
                    {
                        data[dataIndex++] = 0x81; //write 16bit address
                        data[dataIndex++] = 0x05; //address high
                        data[dataIndex++] = 0xE1; //address low
                        data[dataIndex++] = transmitRegisters.PORTMUX_CCLROUTEA; //PORTMUX_CCLROUTEA
                    }
                    state++;
                    break;
                case 10:
                    if(EVSYS_CHANNEL_changed)
                    {
                        data[dataIndex++] = 0x86; //write 16bit address
                        data[dataIndex++] = 0x01; //address high
                        data[dataIndex++] = 0x90; //address low
                        data[dataIndex++] = transmitRegisters.EVSYS_CHANNEL0;
                        data[dataIndex++] = transmitRegisters.EVSYS_CHANNEL1;
                        data[dataIndex++] = transmitRegisters.EVSYS_CHANNEL2;
                        data[dataIndex++] = transmitRegisters.EVSYS_CHANNEL3;
                        data[dataIndex++] = transmitRegisters.EVSYS_CHANNEL4;
                        data[dataIndex++] = transmitRegisters.EVSYS_CHANNEL5;
                    }
                    state++;
                    break;
                case 11:
                    if(EVSYS_USER_changed)
                    {
                        data[dataIndex++] = 0x87; //write 16bit address
                        data[dataIndex++] = 0x01; //address high
                        data[dataIndex++] = 0xA0; //address low
                        data[dataIndex++] = transmitRegisters.EVSYS_CCL_LUT0A;
                        data[dataIndex++] = 0;
                        data[dataIndex++] = transmitRegisters.EVSYS_CCL_LUT1A;
                        data[dataIndex++] = 0;
                        data[dataIndex++] = transmitRegisters.EVSYS_CCL_LUT2A;
                        data[dataIndex++] = 0;
                        data[dataIndex++] = transmitRegisters.EVSYS_CCL_LUT3A;
                        data[dataIndex++] = 0xC3;
                        data[dataIndex++] = 0xA9; //address low
                        data[dataIndex++] = transmitRegisters.EVSYS_EVOUTA;
                        data[dataIndex++] = transmitRegisters.EVSYS_EVOUTB;
                        data[dataIndex++] = transmitRegisters.EVSYS_EVOUTC;
                    }
                    state++;
                    break;
                case 12:
                    if(EVSYS_TCB0_changed)
                    {
                        if(EVSYS_TCB1_changed)
                        {
                            data[dataIndex++] = 0x83;
                            data[dataIndex++] = 0x01; //address high
                            data[dataIndex++] = 0xB0; //address low
                            data[dataIndex++] = transmitRegisters.EVSYS_TCB0_CAPT;
                            data[dataIndex++] = 0;
                            data[dataIndex++] = transmitRegisters.EVSYS_TCB1_CAPT;
                        }
                        else
                        {
                            data[dataIndex++] = 0x81;
                            data[dataIndex++] = 0x01; //address high
                            data[dataIndex++] = 0xB0; //address low
                            data[dataIndex++] = transmitRegisters.EVSYS_TCB0_CAPT;
                        }
                    }
                    else if(EVSYS_TCB1_changed)
                    {
                        data[dataIndex++] = 0x81;
                        data[dataIndex++] = 0x01; //address high
                        data[dataIndex++] = 0xB2; //address low
                        data[dataIndex++] = transmitRegisters.EVSYS_TCB1_CAPT;
                    }
                    state++;
                    break;
                case 13:
                    if(CCL_CTRLA_changed)
                    {
                        data[dataIndex++] = 0x81;
                        data[dataIndex++] = 0x01; //address high
                        data[dataIndex++] = 0xC0; //address low
                        data[dataIndex++] = transmitRegisters.CCL_CTRLA;
                    }
                    state++;
                    break;
                case 14:
                    if(CCL_LUT0_changed)
                    {
                        data[dataIndex++] = 0x83;
                        data[dataIndex++] = 0x01; //address high
                        data[dataIndex++] = 0xC9; //address low
                        data[dataIndex++] = transmitRegisters.CCL_LUT0CTRLB;
                        data[dataIndex++] = transmitRegisters.CCL_LUT0CTRLC;
                        data[dataIndex++] = transmitRegisters.CCL_LUT0TRUTH;

                        data[dataIndex++] = 0xC1;
                        data[dataIndex++] = 0xC8; //address low
                        data[dataIndex++] = transmitRegisters.CCL_LUT0CTRLA;
                    }
                    state++;
                    break;
                case 15:
                    if(CCL_LUT1_changed)
                    {
                        data[dataIndex++] = 0x83;
                        data[dataIndex++] = 0x01; //address high
                        data[dataIndex++] = 0xCD; //address low
                        data[dataIndex++] = transmitRegisters.CCL_LUT1CTRLB;
                        data[dataIndex++] = transmitRegisters.CCL_LUT1CTRLC;
                        data[dataIndex++] = transmitRegisters.CCL_LUT1TRUTH;

                        data[dataIndex++] = 0xC1;
                        data[dataIndex++] = 0xCC; //address low
                        data[dataIndex++] = transmitRegisters.CCL_LUT1CTRLA;
                    }
                    state++;
                    break;
                case 16:
                    if(CCL_LUT2_changed)
                    {
                        data[dataIndex++] = 0x83;
                        data[dataIndex++] = 0x01; //address high
                        data[dataIndex++] = 0xD1; //address low
                        data[dataIndex++] = transmitRegisters.CCL_LUT2CTRLB;
                        data[dataIndex++] = transmitRegisters.CCL_LUT2CTRLC;
                        data[dataIndex++] = transmitRegisters.CCL_LUT2TRUTH;

                        data[dataIndex++] = 0xC1;
                        data[dataIndex++] = 0xD0; //address low
                        data[dataIndex++] = transmitRegisters.CCL_LUT2CTRLA;
                    }
                    state++;
                    break;
                case 17:
                    if(CCL_LUT3_changed)
                    {
                        data[dataIndex++] = 0x83;
                        data[dataIndex++] = 0x01; //address high
                        data[dataIndex++] = 0xD5; //address low
                        data[dataIndex++] = transmitRegisters.CCL_LUT3CTRLB;
                        data[dataIndex++] = transmitRegisters.CCL_LUT3CTRLC;
                        data[dataIndex++] = transmitRegisters.CCL_LUT3TRUTH;

                        data[dataIndex++] = 0xC1;
                        data[dataIndex++] = 0xD4; //address low
                        data[dataIndex++] = transmitRegisters.CCL_LUT3CTRLA;
                    }
                    state++;
                    break;


                default: 
                    if(dataIndex < currentReadFinishedIndex)
                    {
                        std::memset(&data[dataIndex], 0, currentReadFinishedIndex - dataIndex);
                        dataIndex = currentReadFinishedIndex;
                    }
                    else
                    {
                        state = -1;
                    }
                break;
            }
        }

        if(dataIndex > 0 && (transmitRegisters.Comm == SPI || transmitRegisters.Comm== SPIAlternate))
        {
            std::memset(&data[dataIndex], 0, 2);
            dataIndex += 2;
        }

        _first = false;
        _previousRegisters = transmitRegisters;

        return dataIndex;
    }
}
#endif
