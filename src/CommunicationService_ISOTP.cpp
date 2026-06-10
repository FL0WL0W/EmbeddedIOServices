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

    CommunicationService_ISOTP::CommunicationService_ISOTP(ICANService * const canService, const CANIdentifier_t listenIds[], const size_t listenIdsLength, const CANIdentifier_t transmitIds[], const size_t transmitIdsLength) :
        _canService(canService),
        _transmitIds(transmitIds),
        _transmitIdsLength(transmitIdsLength)
    {
        _receiveStates.resize(listenIdsLength);

        for(size_t i = 0; i < listenIdsLength; ++i)
        {
            _receiveStates[i].CallbackID = _canService->RegisterReceiveCallBack(listenIds[i], [this, i](can_send_callback_t sendCallback, const CANData_t data, uint8_t dataLength) {
                ReceiveFrame(i, sendCallback, data, dataLength);
            });
        }
    }

    CommunicationService_ISOTP::~CommunicationService_ISOTP()
    {
        for(const auto& state : _receiveStates)
        {
            _canService->UnRegisterReceiveCallBack(state.CallbackID);
        }
    }

    void CommunicationService_ISOTP::Send(const void *data, size_t length)
    {
        if(data == nullptr || length == 0 || length > ISOTP_MAX_PAYLOAD_LENGTH)
        {
            return;
        }

        const uint8_t * const bytes = reinterpret_cast<const uint8_t *>(data);

        for(size_t transmitIdIndex = 0; transmitIdIndex < _transmitIdsLength; ++transmitIdIndex)
        {
            if(length <= ISOTP_MAX_SINGLE_FRAME_PAYLOAD_LENGTH)
            {
                CANData_t frame;
                Clear(frame);
                frame.Data[0] = static_cast<uint8_t>(ISOTP_SINGLE_FRAME | length);
                std::memcpy(&frame.Data[1], bytes, length);
                _canService->Send(_transmitIds[transmitIdIndex], frame, static_cast<uint8_t>(length + 1));
                continue;
            }

            CANData_t firstFrame;
            Clear(firstFrame);
            firstFrame.Data[0] = static_cast<uint8_t>(ISOTP_FIRST_FRAME | ((length >> 8) & 0x0F));
            firstFrame.Data[1] = static_cast<uint8_t>(length & 0xFF);
            std::memcpy(&firstFrame.Data[2], bytes, ISOTP_FIRST_FRAME_PAYLOAD_LENGTH);
            _canService->Send(_transmitIds[transmitIdIndex], firstFrame, sizeof(firstFrame.Data));

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
                _canService->Send(_transmitIds[transmitIdIndex], consecutiveFrame, static_cast<uint8_t>(bytesThisFrame + 1));

                offset += bytesThisFrame;
                sequenceNumber = static_cast<uint8_t>((sequenceNumber + 1) & 0x0F);
            }
        }
    }

    void CommunicationService_ISOTP::ReceiveFrame(size_t index, can_send_callback_t sendCallback, const CANData_t data, uint8_t dataLength)
    {
        if(dataLength == 0 || index >= _receiveStates.size())
        {
            return;
        }

        const uint8_t frameType = data.Data[0] & ISOTP_PROTOCOL_CONTROL_INFORMATION_MASK;
        ReceiveState& state = _receiveStates[index];

        switch(frameType)
        {
            case ISOTP_SINGLE_FRAME:
            {
                const size_t payloadLength = data.Data[0] & 0x0F;
                if(payloadLength > ISOTP_MAX_SINGLE_FRAME_PAYLOAD_LENGTH || payloadLength + 1 > dataLength)
                {
                    return;
                }

                state.Active = false;
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
                    state.Active = false;
                    return;
                }

                state.Buffer.clear();
                state.Buffer.reserve(payloadLength);
                state.ExpectedLength = payloadLength;
                state.NextSequenceNumber = 1;
                state.Active = true;

                const size_t bytesInFrame = std::min(static_cast<size_t>(dataLength - 2), std::min(payloadLength, static_cast<size_t>(ISOTP_FIRST_FRAME_PAYLOAD_LENGTH)));
                state.Buffer.insert(state.Buffer.end(), &data.Data[2], &data.Data[2] + bytesInFrame);

                if(index < _transmitIdsLength)
                {
                    CANData_t flowControlFrame;
                    Clear(flowControlFrame);
                    flowControlFrame.Data[0] = ISOTP_FLOW_CONTROL | ISOTP_FLOW_STATUS_CONTINUE_TO_SEND;
                    flowControlFrame.Data[1] = 0;
                    flowControlFrame.Data[2] = 0;
                    sendCallback(_transmitIds[index], flowControlFrame, 3);
                }

                if(state.Buffer.size() >= state.ExpectedLength)
                {
                    state.Active = false;
                    Receive(state.Buffer.data(), state.ExpectedLength);
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
                if(!state.Active || sequenceNumber != state.NextSequenceNumber)
                {
                    state.Active = false;
                    return;
                }

                const size_t remaining = state.ExpectedLength - state.Buffer.size();
                const size_t bytesInFrame = std::min(static_cast<size_t>(dataLength - 1), remaining);
                state.Buffer.insert(state.Buffer.end(), &data.Data[1], &data.Data[1] + bytesInFrame);

                state.NextSequenceNumber = static_cast<uint8_t>((state.NextSequenceNumber + 1) & 0x0F);

                if(state.Buffer.size() >= state.ExpectedLength)
                {
                    state.Active = false;
                    Receive(state.Buffer.data(), state.ExpectedLength);
                }
                return;
            }

            case ISOTP_FLOW_CONTROL:
            default:
                return;
        }
    }
}
#endif
