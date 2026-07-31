#include "ICommunicationService.h"
#include "ICANService.h"
#include <vector>

#ifndef COMMUNICATIONSERVICE_ISOTP_H
#define COMMUNICATIONSERVICE_ISOTP_H
namespace EmbeddedIOServices
{
        class ICANService;

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

        struct SendState
        {
            std::vector<uint8_t> Buffer;
            size_t Offset = 0;
            uint8_t NextSequenceNumber = 1;
            bool Active = false;
        };

        ICANService * const _canService;
        ReceiveState _receiveState;
        SendState _sendState;

        void ReceiveFrame(can_send_callback_t sendCallback, const CANData_t data, uint8_t dataLength);
	public:
        const CANIdentifier_t ListenId;
        const CANIdentifier_t TransmitId;
        CommunicationService_ISOTP(ICANService * const canService, const CANIdentifier_t listenId, const CANIdentifier_t transmitId);
        ~CommunicationService_ISOTP() override;
        void Send(const void *data, size_t length);
        bool Ready() override;
	};
}
#endif
