#include "ICommunicationService.h"
#include "ICANService.h"
#include <vector>

#ifndef COMMUNICATIONSERVICE_ISOTP_H
#define COMMUNICATIONSERVICE_ISOTP_H
namespace EmbeddedIOServices
{
	class CommunicationService_ISOTP : public EmbeddedIOServices::ICommunicationService
	{
	protected:
        struct ReceiveState
        {
            std::vector<uint8_t> Buffer;
            size_t ExpectedLength = 0;
            can_receive_callback_id_t CallbackID;
            uint8_t NextSequenceNumber = 1;
            bool Active = false;
        };

        ICANService * const _canService;
        const CANIdentifier_t * const _transmitIds;
        const size_t _transmitIdsLength;
        std::vector<ReceiveState> _receiveStates;

        void ReceiveFrame(size_t index, can_send_callback_t sendCallback, const CANData_t data, uint8_t dataLength);
	public:
        CommunicationService_ISOTP(ICANService * const canService, const CANIdentifier_t listenIds[], const size_t listenIdsLength, const CANIdentifier_t transmitIds[], const size_t transmitIdsLength);
        ~CommunicationService_ISOTP();
        static void Send(ICANService * const canService, const CANIdentifier_t transmitId, const void *data, size_t length);
        void Send(const void *data, size_t length);
	};
}
#endif
