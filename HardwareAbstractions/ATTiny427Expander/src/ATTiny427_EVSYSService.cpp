#include "ATTiny427_EVSYSService.h"
#include <cstring>
#include <algorithm>

#ifdef ATTINY427_EVSYSSERVICE_H
namespace EmbeddedIOServices
{
	ATTiny427_EVSYSService::ATTiny427_EVSYSService(ATTiny427_ExpanderService *aTTiny427ExpanderService) :
		EVSYS_CCL_LUT0A(&aTTiny427ExpanderService->GetRegister(ADDRESS_EVSYS_CCL_LUT0A)),
		EVSYS_CCL_LUT1A(&aTTiny427ExpanderService->GetRegister(ADDRESS_EVSYS_CCL_LUT1A)),
		EVSYS_CCL_LUT2A(&aTTiny427ExpanderService->GetRegister(ADDRESS_EVSYS_CCL_LUT2A)),
		EVSYS_CCL_LUT3A(&aTTiny427ExpanderService->GetRegister(ADDRESS_EVSYS_CCL_LUT3A)),
		EVSYS_EVOUTA(&aTTiny427ExpanderService->GetRegister(ADDRESS_EVSYS_EVOUTA)),
		EVSYS_EVOUTB(&aTTiny427ExpanderService->GetRegister(ADDRESS_EVSYS_EVOUTB)),
		EVSYS_EVOUTC(&aTTiny427ExpanderService->GetRegister(ADDRESS_EVSYS_EVOUTC)),
		EVSYS_TCB0_CAPT(&aTTiny427ExpanderService->GetRegister(ADDRESS_EVSYS_TCB0_CAPT)),
		EVSYS_TCB1_CAPT(&aTTiny427ExpanderService->GetRegister(ADDRESS_EVSYS_TCB1_CAPT)),
		EVSYS_CHANNEL{
			&aTTiny427ExpanderService->GetRegister(ADDRESS_EVSYS_CHANNEL(0)),
			&aTTiny427ExpanderService->GetRegister(ADDRESS_EVSYS_CHANNEL(1)),
			&aTTiny427ExpanderService->GetRegister(ADDRESS_EVSYS_CHANNEL(2)),
			&aTTiny427ExpanderService->GetRegister(ADDRESS_EVSYS_CHANNEL(3)),
			&aTTiny427ExpanderService->GetRegister(ADDRESS_EVSYS_CHANNEL(4)),
			&aTTiny427ExpanderService->GetRegister(ADDRESS_EVSYS_CHANNEL(5)) }
	{
	}

    ATTiny427_EVSYSChannel ATTiny427_EVSYSService::_firstOpenChannel(uint8_t generator)
    {
        for(int8_t i = 0; i < sizeof(EVSYS_CHANNEL) / sizeof(EVSYS_CHANNEL[0]); i++)
        {
            if(*EVSYS_CHANNEL[i] == 0 || *EVSYS_CHANNEL[i] == generator)
                return { i, generator };
        }
        return { -1, 0 };
    }
    void ATTiny427_EVSYSService::_reassignChannel(uint8_t channel, ATTiny427_EVSYSChannel to)
    {
		*EVSYS_CHANNEL[to.channel] = to.generator;

        for(int8_t i = 0; i < sizeof(EVSYS_USER) / sizeof(EVSYS_USER[0]); i++)
        {
			if(*EVSYS_USER[i] == channel + 1)
				*EVSYS_USER[i] = to.channel + 1;
        }

		*EVSYS_CHANNEL[channel] = 0;
    }
    ATTiny427_EVSYSChannel ATTiny427_EVSYSService::_getEVSYSChannel(uint8_t inputPin, bool useAC, int8_t depth)
	{
		if(useAC == true)
			return _firstOpenChannel(0x20);
		if(inputPin >= 22)
			return { -1, 0 };

		if(inputPin < 8)
		{
			if(*EVSYS_CHANNEL[0] == 0 || (depth == 0 && *EVSYS_CHANNEL[0] == ((inputPin & 0x7) | 0x40)))
				return { 0, (uint8_t)((inputPin & 0x7) | 0x40) };
			if(*EVSYS_CHANNEL[1] == 0 || (depth == 0 && *EVSYS_CHANNEL[1] == ((inputPin & 0x7) | 0x40)))
				return { 1, (uint8_t)((inputPin & 0x7) | 0x40) };
			if(*EVSYS_CHANNEL[2] == 0 || (depth == 0 && *EVSYS_CHANNEL[2] == ((inputPin & 0x7) | 0x48)))
				return { 2, (uint8_t)((inputPin & 0x7) | 0x48) };
			if(*EVSYS_CHANNEL[3] == 0 || (depth == 0 && *EVSYS_CHANNEL[3] == ((inputPin & 0x7) | 0x48)))
				return { 3, (uint8_t)((inputPin & 0x7) | 0x48) };
			if(depth > 1)
				return { -1, 0 };
			const ATTiny427_EVSYSChannel channel0ReassignTo = (*EVSYS_CHANNEL[0] & 0xF0) == 0x40? 
																			_getEVSYSChannel(*EVSYS_CHANNEL[0] & 0xF, false, depth + 1) : 
																			_firstOpenChannel(*EVSYS_CHANNEL[0]);
			if(channel0ReassignTo.channel != -1)
			{
				_reassignChannel(0, channel0ReassignTo);
				return _getEVSYSChannel(inputPin, false, depth + 1);
			}
			const ATTiny427_EVSYSChannel channel1ReassignTo = (*EVSYS_CHANNEL[1] & 0xF0) == 0x40? 
																			_getEVSYSChannel(*EVSYS_CHANNEL[1] & 0xF, false, depth + 1) : 
																			_firstOpenChannel(*EVSYS_CHANNEL[1]);
			if(channel1ReassignTo.channel != -1)
			{
				_reassignChannel(1, channel1ReassignTo);
				return _getEVSYSChannel(inputPin, false, depth + 1);
			}
			const ATTiny427_EVSYSChannel channel2ReassignTo = (*EVSYS_CHANNEL[2] & 0xF0) == 0x40? 
																			_getEVSYSChannel(*EVSYS_CHANNEL[2] < 0x48? ((*EVSYS_CHANNEL[2] & 0x7) + 16) : (*EVSYS_CHANNEL[2] & 0x7), false, depth + 1) : 
																			_firstOpenChannel(*EVSYS_CHANNEL[2]);
			if(channel2ReassignTo.channel != -1)
			{
				_reassignChannel(2, channel2ReassignTo);
				return _getEVSYSChannel(inputPin, false, depth + 1);
			}
			
			const ATTiny427_EVSYSChannel channel3ReassignTo = (*EVSYS_CHANNEL[3] & 0xF0) == 0x40? 
																			_getEVSYSChannel(*EVSYS_CHANNEL[3] < 0x48? ((*EVSYS_CHANNEL[3] & 0x7) + 16) : (*EVSYS_CHANNEL[3] & 0x7), false, depth + 1) : 
																			_firstOpenChannel(*EVSYS_CHANNEL[3]);
			if(channel3ReassignTo.channel != -1)
			{
				_reassignChannel(3, channel3ReassignTo);
				return _getEVSYSChannel(inputPin, false, depth + 1);
			}
			return { -1, 0 };
		}
		if(inputPin < 16)
		{
			if(*EVSYS_CHANNEL[4] == 0 || (depth == 0 && *EVSYS_CHANNEL[4] == ((inputPin & 0x7) | 0x40)))
				return { 4, (uint8_t)((inputPin & 0x7) | 0x40) };
			if(*EVSYS_CHANNEL[5] == 0 || (depth == 0 && *EVSYS_CHANNEL[5] == ((inputPin & 0x7) | 0x40)))
				return { 5, (uint8_t)((inputPin & 0x7) | 0x40) };
			if(*EVSYS_CHANNEL[0] == 0 || (depth == 0 && *EVSYS_CHANNEL[0] == ((inputPin & 0x7) | 0x48)))
				return { 0, (uint8_t)((inputPin & 0x7) | 0x48) };
			if(*EVSYS_CHANNEL[1] == 0 || (depth == 0 && *EVSYS_CHANNEL[1] == ((inputPin & 0x7) | 0x48)))
				return { 1, (uint8_t)((inputPin & 0x7) | 0x48) };
			if(depth > 1)
				return { -1, 0 };
			const ATTiny427_EVSYSChannel channel4ReassignTo = (*EVSYS_CHANNEL[4] & 0xF0) == 0x40? 
																			_getEVSYSChannel((*EVSYS_CHANNEL[4] & 0xF) + 8, false, depth + 1) : 
																			_firstOpenChannel(*EVSYS_CHANNEL[4]);
			if(channel4ReassignTo.channel != -1)
			{
				_reassignChannel(4, channel4ReassignTo);
				return _getEVSYSChannel(inputPin, false, depth + 1);
			}
			const ATTiny427_EVSYSChannel channel5ReassignTo = (*EVSYS_CHANNEL[5] & 0xF0) == 0x40? 
																			_getEVSYSChannel((*EVSYS_CHANNEL[5] & 0xF) + 8, false, depth + 1) : 
																			_firstOpenChannel(*EVSYS_CHANNEL[5]);
			if(channel5ReassignTo.channel != -1)
			{
				_reassignChannel(5, channel5ReassignTo);
				return _getEVSYSChannel(inputPin, false, depth + 1);
			}
			const ATTiny427_EVSYSChannel channel0ReassignTo = (*EVSYS_CHANNEL[0] & 0xF0) == 0x40? 
																			_getEVSYSChannel(*EVSYS_CHANNEL[0] & 0xF, false, depth + 1) : 
																			_firstOpenChannel(*EVSYS_CHANNEL[0]);
			if(channel0ReassignTo.channel != -1)
			{
				_reassignChannel(0, channel0ReassignTo);
				return _getEVSYSChannel(inputPin, false, depth + 1);
			}
			const ATTiny427_EVSYSChannel channel1ReassignTo = (*EVSYS_CHANNEL[1] & 0xF0) == 0x40? 
																			_getEVSYSChannel(*EVSYS_CHANNEL[1] & 0xF, false, depth + 1) : 
																			_firstOpenChannel(*EVSYS_CHANNEL[1]);
			if(channel1ReassignTo.channel != -1)
			{
				_reassignChannel(1, channel1ReassignTo);
				return _getEVSYSChannel(inputPin, false, depth + 1);
			}
			return { -1, 0 };
		}
		if(*EVSYS_CHANNEL[2] == 0 || (depth == 0 && *EVSYS_CHANNEL[2] == ((inputPin & 0x7) | 0x40)))
			return { 2, (uint8_t)((inputPin & 0x7) | 0x40) };
		if(*EVSYS_CHANNEL[3] == 0 || (depth == 0 && *EVSYS_CHANNEL[3] == ((inputPin & 0x7) | 0x40)))
			return { 3, (uint8_t)((inputPin & 0x7) | 0x40) };
		if(*EVSYS_CHANNEL[4] == 0 || (depth == 0 && *EVSYS_CHANNEL[4] == ((inputPin & 0x7) | 0x48)))
			return { 4, (uint8_t)((inputPin & 0x7) | 0x48) };
		if(*EVSYS_CHANNEL[5] == 0 || (depth == 0 && *EVSYS_CHANNEL[5] == ((inputPin & 0x7) | 0x48)))
			return { 5, (uint8_t)((inputPin & 0x7) | 0x48) };
		if(depth > 1)
			return { -1, 0 };
		const ATTiny427_EVSYSChannel channel2ReassignTo = (*EVSYS_CHANNEL[2] & 0xF0) == 0x40? 
																		_getEVSYSChannel(*EVSYS_CHANNEL[2] < 0x48? ((*EVSYS_CHANNEL[2] & 0x7) + 16) : (*EVSYS_CHANNEL[2] & 0x7), false, depth + 1) : 
																		_firstOpenChannel(*EVSYS_CHANNEL[2]);
		if(channel2ReassignTo.channel != -1)
		{
			_reassignChannel(2, channel2ReassignTo);
			return _getEVSYSChannel(inputPin, false, depth + 1);
		}
		
		const ATTiny427_EVSYSChannel channel3ReassignTo = (*EVSYS_CHANNEL[3] & 0xF0) == 0x40? 
																		_getEVSYSChannel(*EVSYS_CHANNEL[3] < 0x48? ((*EVSYS_CHANNEL[3] & 0x7) + 16) : (*EVSYS_CHANNEL[3] & 0x7), false, depth + 1) : 
																		_firstOpenChannel(*EVSYS_CHANNEL[3]);
		if(channel3ReassignTo.channel != -1)
		{
			_reassignChannel(3, channel3ReassignTo);
			return _getEVSYSChannel(inputPin, false, depth + 1);
		}
		const ATTiny427_EVSYSChannel channel4ReassignTo = (*EVSYS_CHANNEL[4] & 0xF0) == 0x40? 
																		_getEVSYSChannel((*EVSYS_CHANNEL[4] & 0xF) + 8, false, depth + 1) : 
																		_firstOpenChannel(*EVSYS_CHANNEL[4]);
		if(channel4ReassignTo.channel != -1)
		{
			_reassignChannel(4, channel4ReassignTo);
			return _getEVSYSChannel(inputPin, false, depth + 1);
		}
		const ATTiny427_EVSYSChannel channel5ReassignTo = (*EVSYS_CHANNEL[5] & 0xF0) == 0x40? 
																		_getEVSYSChannel((*EVSYS_CHANNEL[5] & 0xF) + 8, false, depth + 1) : 
																		_firstOpenChannel(*EVSYS_CHANNEL[5]);
		if(channel5ReassignTo.channel != -1)
		{
			_reassignChannel(5, channel5ReassignTo);
			return _getEVSYSChannel(inputPin, false, depth + 1);
		}
		return { -1, 0 };
	}

	bool ATTiny427_EVSYSService::InitEVSYSUser(ATTiny427_ExpanderService::Attiny427_ExpanderRegister &userRegister, uint8_t inputPin, bool useAC)
	{
		const ATTiny427_EVSYSChannel channel = _getEVSYSChannel(inputPin, useAC);
		if(channel.channel == -1)
			return false;

		*EVSYS_CHANNEL[channel.channel] = channel.generator;
		userRegister = channel.channel + 1;
		return true;
	}

	void ATTiny427_EVSYSService::DeinitEVSYSUser(ATTiny427_ExpanderService::Attiny427_ExpanderRegister &userRegister)
	{
		const uint8_t userValue = (uint8_t)userRegister;
		userRegister = 0;

		for(uint8_t i = 0; i < sizeof(EVSYS_USER) / sizeof(EVSYS_USER[0]); i++)
		{
			if(*EVSYS_USER[i] == userValue && &userRegister != EVSYS_USER[i])
				return;
		}

		*EVSYS_CHANNEL[userValue - 1] = 0;
	}
}
#endif
