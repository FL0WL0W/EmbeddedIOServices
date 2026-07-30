#include <algorithm>
#include <cstring>
#include "CommunicationService_ISOTP.h"

#ifdef COMMUNICATIONSERVICE_ISOTP_H
namespace EmbeddedIOServices
{
    namespace
    {
        constexpr uint8_t ISOTP_PROTOCOL_CONTROL_INFORMATION_MASK = 0xF0;
        constexpr uint8_t ISOTP_SINGLE_FRAME = 0x00;
        constexpr uint8_t ISOTP_FIRST_FRAME = 0x10;
        constexpr uint8_t ISOTP_CONSECUTIVE_FRAME = 0x20;
        constexpr uint8_t ISOTP_FLOW_CONTROL = 0x30;
        constexpr uint8_t ISOTP_FLOW_STATUS_CONTINUE_TO_SEND = 0x00;
        constexpr uint8_t ISOTP_MAX_SINGLE_FRAME_PAYLOAD_LENGTH = 7;
        constexpr uint8_t ISOTP_FIRST_FRAME_PAYLOAD_LENGTH = 6;
        constexpr uint8_t ISOTP_CONSECUTIVE_FRAME_PAYLOAD_LENGTH = 7;
        constexpr size_t ISOTP_MAX_PAYLOAD_LENGTH = 0xFFF;

        void Clear(CANData_t& data)
        {
            std::memset(data.Data, 0, sizeof(data.Data));
        }
    }

    CommunicationService_ISOTP::CommunicationService_ISOTP(ICANService * const canService, const CANIdentifier_t listenId, const CANIdentifier_t transmitId) :
        _canService(canService),
        ListenId(listenId),
        TransmitId(transmitId)
    {
        _receiveState.CallbackID = _canService->RegisterReceiveCallBack(ListenId, [this](can_send_callback_t sendCallback, const CANData_t data, uint8_t dataLength) {
            ReceiveFrame(sendCallback, data, dataLength);
        });
    }

    CommunicationService_ISOTP::~CommunicationService_ISOTP()
    {
        _canService->UnRegisterReceiveCallBack(_receiveState.CallbackID);
    }

    void CommunicationService_ISOTP::Send(ICANService * const canService, const CANIdentifier_t transmitId, const void *data, size_t length)
    {
        if(canService == nullptr || data == nullptr || length == 0 || length > ISOTP_MAX_PAYLOAD_LENGTH)
        {
            return;
        }

        const uint8_t * const bytes = reinterpret_cast<const uint8_t *>(data);

        if(length <= ISOTP_MAX_SINGLE_FRAME_PAYLOAD_LENGTH)
        {
            CANData_t frame;
            Clear(frame);
            frame.Data[0] = static_cast<uint8_t>(ISOTP_SINGLE_FRAME | length);
            std::memcpy(&frame.Data[1], bytes, length);
            canService->Send(transmitId, frame, static_cast<uint8_t>(length + 1));
            return;
        }

        CANData_t firstFrame;
        Clear(firstFrame);
        firstFrame.Data[0] = static_cast<uint8_t>(ISOTP_FIRST_FRAME | ((length >> 8) & 0x0F));
        firstFrame.Data[1] = static_cast<uint8_t>(length & 0xFF);
        std::memcpy(&firstFrame.Data[2], bytes, ISOTP_FIRST_FRAME_PAYLOAD_LENGTH);
        canService->Send(transmitId, firstFrame, sizeof(firstFrame.Data));

        size_t offset = ISOTP_FIRST_FRAME_PAYLOAD_LENGTH;
        uint8_t sequenceNumber = 1;
        while(offset < length)
        {
            CANData_t consecutiveFrame;
            Clear(consecutiveFrame);
            const size_t remaining = length - offset;
            const size_t bytesThisFrame = std::min(remaining, static_cast<size_t>(ISOTP_CONSECUTIVE_FRAME_PAYLOAD_LENGTH));

            consecutiveFrame.Data[0] = static_cast<uint8_t>(ISOTP_CONSECUTIVE_FRAME | (sequenceNumber & 0x0F));
            std::memcpy(&consecutiveFrame.Data[1], bytes + offset, bytesThisFrame);
            canService->Send(transmitId, consecutiveFrame, static_cast<uint8_t>(bytesThisFrame + 1));

            offset += bytesThisFrame;
            sequenceNumber = static_cast<uint8_t>((sequenceNumber + 1) & 0x0F);
        }
    }

    void CommunicationService_ISOTP::Send(const void *data, size_t length)
    {
        if(_canService == nullptr || data == nullptr || length == 0 || length > ISOTP_MAX_PAYLOAD_LENGTH)
        {
            return;
        }

        const uint8_t * const bytes = reinterpret_cast<const uint8_t *>(data);

        if(length <= ISOTP_MAX_SINGLE_FRAME_PAYLOAD_LENGTH)
        {
            CANData_t frame;
            Clear(frame);
            frame.Data[0] = static_cast<uint8_t>(ISOTP_SINGLE_FRAME | length);
            std::memcpy(&frame.Data[1], bytes, length);
            _canService->Send(TransmitId, frame, static_cast<uint8_t>(length + 1));
            return;
        }

        // Multi-frame: send first frame and await flow control before sending consecutive frames
        _sendState.Buffer.assign(bytes, bytes + length);
        _sendState.Offset = ISOTP_FIRST_FRAME_PAYLOAD_LENGTH;
        _sendState.NextSequenceNumber = 1;
        _sendState.Active = true;

        CANData_t firstFrame;
        Clear(firstFrame);
        firstFrame.Data[0] = static_cast<uint8_t>(ISOTP_FIRST_FRAME | ((length >> 8) & 0x0F));
        firstFrame.Data[1] = static_cast<uint8_t>(length & 0xFF);
        std::memcpy(&firstFrame.Data[2], bytes, ISOTP_FIRST_FRAME_PAYLOAD_LENGTH);
        _canService->Send(TransmitId, firstFrame, sizeof(firstFrame.Data));
    }

    void CommunicationService_ISOTP::ReceiveFrame(can_send_callback_t sendCallback, const CANData_t data, uint8_t dataLength)
    {
        if(dataLength == 0)
        {
            return;
        }

        const uint8_t frameType = data.Data[0] & ISOTP_PROTOCOL_CONTROL_INFORMATION_MASK;

        switch(frameType)
        {
            case ISOTP_SINGLE_FRAME:
            {
                const size_t payloadLength = data.Data[0] & 0x0F;
                if(payloadLength > ISOTP_MAX_SINGLE_FRAME_PAYLOAD_LENGTH || payloadLength + 1 > dataLength)
                {
                    return;
                }

                _receiveState.Active = false;
                Receive(&data.Data[1], payloadLength);
                return;
            }

            case ISOTP_FIRST_FRAME:
            {
                if(dataLength < 2)
                {
                    return;
                }

                const size_t payloadLength = (static_cast<size_t>(data.Data[0] & 0x0F) << 8) | data.Data[1];
                if(payloadLength == 0 || payloadLength > ISOTP_MAX_PAYLOAD_LENGTH)
                {
                    _receiveState.Active = false;
                    return;
                }

                _receiveState.Buffer.clear();
                _receiveState.Buffer.reserve(payloadLength);
                _receiveState.ExpectedLength = payloadLength;
                _receiveState.NextSequenceNumber = 1;
                _receiveState.Active = true;

                const size_t bytesInFrame = std::min(static_cast<size_t>(dataLength - 2), std::min(payloadLength, static_cast<size_t>(ISOTP_FIRST_FRAME_PAYLOAD_LENGTH)));
                _receiveState.Buffer.insert(_receiveState.Buffer.end(), &data.Data[2], &data.Data[2] + bytesInFrame);

                CANData_t flowControlFrame;
                Clear(flowControlFrame);
                flowControlFrame.Data[0] = ISOTP_FLOW_CONTROL | ISOTP_FLOW_STATUS_CONTINUE_TO_SEND;
                flowControlFrame.Data[1] = 0; // block size: send all
                flowControlFrame.Data[2] = 0; // STmin: no separation time
                _canService->Send(TransmitId, flowControlFrame, 3);

                if(_receiveState.Buffer.size() >= _receiveState.ExpectedLength)
                {
                    _receiveState.Active = false;
                    Receive(_receiveState.Buffer.data(), _receiveState.ExpectedLength);
                }
                return;
            }

            case ISOTP_CONSECUTIVE_FRAME:
            {
                if(dataLength < 2)
                {
                    return;
                }

                const uint8_t sequenceNumber = data.Data[0] & 0x0F;
                if(!_receiveState.Active || sequenceNumber != _receiveState.NextSequenceNumber)
                {
                    _receiveState.Active = false;
                    return;
                }

                const size_t remaining = _receiveState.ExpectedLength - _receiveState.Buffer.size();
                const size_t bytesInFrame = std::min(static_cast<size_t>(dataLength - 1), remaining);
                _receiveState.Buffer.insert(_receiveState.Buffer.end(), &data.Data[1], &data.Data[1] + bytesInFrame);

                _receiveState.NextSequenceNumber = static_cast<uint8_t>((_receiveState.NextSequenceNumber + 1) & 0x0F);

                if(_receiveState.Buffer.size() >= _receiveState.ExpectedLength)
                {
                    _receiveState.Active = false;
                    Receive(_receiveState.Buffer.data(), _receiveState.ExpectedLength);
                }
                return;
            }

            case ISOTP_FLOW_CONTROL:
            {
                if(!_sendState.Active || dataLength < 1)
                {
                    return;
                }

                const uint8_t flowStatus = data.Data[0] & 0x0F;
                if(flowStatus != ISOTP_FLOW_STATUS_CONTINUE_TO_SEND)
                {
                    // Abort (Wait or Overflow) — discard pending send
                    _sendState.Active = false;
                    return;
                }

                // Send all remaining consecutive frames
                const size_t totalLength = _sendState.Buffer.size();
                while(_sendState.Offset < totalLength)
                {
                    CANData_t consecutiveFrame;
                    Clear(consecutiveFrame);
                    const size_t remaining = totalLength - _sendState.Offset;
                    const size_t bytesThisFrame = std::min(remaining, static_cast<size_t>(ISOTP_CONSECUTIVE_FRAME_PAYLOAD_LENGTH));

                    consecutiveFrame.Data[0] = static_cast<uint8_t>(ISOTP_CONSECUTIVE_FRAME | (_sendState.NextSequenceNumber & 0x0F));
                    std::memcpy(&consecutiveFrame.Data[1], _sendState.Buffer.data() + _sendState.Offset, bytesThisFrame);
                    _canService->Send(TransmitId, consecutiveFrame, static_cast<uint8_t>(bytesThisFrame + 1));

                    _sendState.Offset += bytesThisFrame;
                    _sendState.NextSequenceNumber = static_cast<uint8_t>((_sendState.NextSequenceNumber + 1) & 0x0F);
                }

                _sendState.Active = false;
                return;
            }

            default:
                return;
        }
    }
}
#endif
