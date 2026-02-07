#include "ATTiny427_ExpanderService.h"

#ifndef ATTINY427_EVSYSSERVICE_H
#define ATTINY427_EVSYSSERVICE_H

#define OFFSET_EVSYS					0x180
#define ADDRESS_EVSYS_CHANNEL(i)		(OFFSET_EVSYS + 0x10 + i)
#define ADDRESS_EVSYS_USER(i)	        (OFFSET_EVSYS + 0x20 + i)
#define ADDRESS_EVSYS_CCL_LUT0A			ADDRESS_EVSYS_USER(0x00)
#define ADDRESS_EVSYS_CCL_LUT1A			ADDRESS_EVSYS_USER(0x02)
#define ADDRESS_EVSYS_CCL_LUT2A			ADDRESS_EVSYS_USER(0x04)
#define ADDRESS_EVSYS_CCL_LUT3A			ADDRESS_EVSYS_USER(0x06)
#define ADDRESS_EVSYS_EVOUTA			ADDRESS_EVSYS_USER(0x09)
#define ADDRESS_EVSYS_EVOUTB			ADDRESS_EVSYS_USER(0x0A)
#define ADDRESS_EVSYS_EVOUTC			ADDRESS_EVSYS_USER(0x0B)
#define ADDRESS_EVSYS_TCB0_CAPT			ADDRESS_EVSYS_USER(0x11)
#define ADDRESS_EVSYS_TCB1_CAPT			ADDRESS_EVSYS_USER(0x13)
#define ADDRESS_EVSYS_USER_COUNT 0x15

namespace EmbeddedIOServices
{
	struct ATTiny427_EVSYSChannel
	{
		int8_t channel;
		uint8_t generator;
	};

	class ATTiny427_EVSYSService
	{       
		ATTiny427_EVSYSChannel _firstOpenChannel(uint8_t generator = 0);
		void _reassignChannel(uint8_t channel, ATTiny427_EVSYSChannel to);
        ATTiny427_EVSYSChannel _getEVSYSChannel(uint8_t inputPin, bool useAC = false, int8_t depth = 0);

	public:
        ATTiny427_ExpanderService::Attiny427_ExpanderRegister * const EVSYS_CHANNEL[6];
        union 
        {
            ATTiny427_ExpanderService::Attiny427_ExpanderRegister * const EVSYS_USER[9];
            struct 
            {
                ATTiny427_ExpanderService::Attiny427_ExpanderRegister * const EVSYS_CCL_LUT0A;
                ATTiny427_ExpanderService::Attiny427_ExpanderRegister * const EVSYS_CCL_LUT1A;
                ATTiny427_ExpanderService::Attiny427_ExpanderRegister * const EVSYS_CCL_LUT2A;
                ATTiny427_ExpanderService::Attiny427_ExpanderRegister * const EVSYS_CCL_LUT3A;
                ATTiny427_ExpanderService::Attiny427_ExpanderRegister * const EVSYS_EVOUTA;
                ATTiny427_ExpanderService::Attiny427_ExpanderRegister * const EVSYS_EVOUTB;
                ATTiny427_ExpanderService::Attiny427_ExpanderRegister * const EVSYS_EVOUTC;
                ATTiny427_ExpanderService::Attiny427_ExpanderRegister * const EVSYS_TCB0_CAPT;
                ATTiny427_ExpanderService::Attiny427_ExpanderRegister * const EVSYS_TCB1_CAPT;
            };
        };
        
        ATTiny427_EVSYSService(ATTiny427_ExpanderService *aTTiny427ExpanderService);

        bool InitEVSYSUser(ATTiny427_ExpanderService::Attiny427_ExpanderRegister &userRegister, uint8_t inputPin, bool useAC = false);
        void DeinitEVSYSUser(ATTiny427_ExpanderService::Attiny427_ExpanderRegister &userRegister);
	};
}
#endif
