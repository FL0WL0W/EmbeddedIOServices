#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "CommunicationService_ISOTP.h"
#include "MockCANService.h"
#include <vector>
using namespace testing;
using namespace EmbeddedIOServices;

namespace UnitTests
{
    class CommunicationService_ISOTPTests : public ::testing::Test
    {
    protected:
        MockCANService _canService;
        const CANIdentifier_t _listenId = { 0x700, 0 };
        const CANIdentifier_t _transmitId = { 0x708, 0 };
    };

    TEST_F(CommunicationService_ISOTPTests, SendsSingleFrameForSevenBytesOrLess)
    {
        CommunicationService_ISOTP service(&_canService, _listenId, _transmitId);
        const uint8_t payload[3] = { 0x22, 0xF1, 0x90 };

        EXPECT_CALL(_canService, Send(_transmitId, _, 4))
            .WillOnce([](const CANIdentifier_t identifier, const CANData_t data, const uint8_t dataLength) {
                EXPECT_EQ(0x03, data.Data[0]);
                EXPECT_EQ(0x22, data.Data[1]);
                EXPECT_EQ(0xF1, data.Data[2]);
                EXPECT_EQ(0x90, data.Data[3]);
            });

        service.Send(payload, sizeof(payload));
    }

    TEST_F(CommunicationService_ISOTPTests, SendsFirstFrameAndConsecutiveFramesForLongPayload)
    {
        CommunicationService_ISOTP service(&_canService, _listenId, _transmitId);
        const uint8_t payload[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        InSequence sequence;
        EXPECT_CALL(_canService, Send(_transmitId, _, 8))
            .WillOnce([](const CANIdentifier_t identifier, const CANData_t data, const uint8_t dataLength) {
                EXPECT_EQ(0x10, data.Data[0]);
                EXPECT_EQ(0x0A, data.Data[1]);
                EXPECT_EQ(0, data.Data[2]);
                EXPECT_EQ(5, data.Data[7]);
            });
        EXPECT_CALL(_canService, Send(_transmitId, _, 5))
            .WillOnce([](const CANIdentifier_t identifier, const CANData_t data, const uint8_t dataLength) {
                EXPECT_EQ(0x21, data.Data[0]);
                EXPECT_EQ(6, data.Data[1]);
                EXPECT_EQ(9, data.Data[4]);
            });

        service.Send(payload, sizeof(payload));
        // Simulate flow control response so consecutive frames are sent
        const CANData_t flowControl = { 0x30, 0x00, 0x00, 0, 0, 0, 0, 0 };
        _canService.Receive(_listenId, flowControl, 3);
    }

    TEST_F(CommunicationService_ISOTPTests, ReceivesSingleFramePayload)
    {
        CommunicationService_ISOTP service(&_canService, _listenId, _transmitId);
        std::vector<uint8_t> received;
        service.RegisterReceiveCallBack([&received](communication_send_callback_t send, const void *data, size_t length) {
            const uint8_t * const bytes = reinterpret_cast<const uint8_t *>(data);
            received.assign(bytes, bytes + length);
            return length;
        });

        const CANData_t frame = { 0x03, 0x22, 0xF1, 0x90, 0, 0, 0, 0 };
        _canService.Receive(_listenId, frame, 4);

        ASSERT_EQ(3u, received.size());
        EXPECT_EQ(0x22, received[0]);
        EXPECT_EQ(0xF1, received[1]);
        EXPECT_EQ(0x90, received[2]);
    }

    TEST_F(CommunicationService_ISOTPTests, ReassemblesMultiFramePayloadAndSendsFlowControl)
    {
        CommunicationService_ISOTP service(&_canService, _listenId, _transmitId);
        std::vector<uint8_t> received;
        service.RegisterReceiveCallBack([&received](communication_send_callback_t send, const void *data, size_t length) {
            const uint8_t * const bytes = reinterpret_cast<const uint8_t *>(data);
            received.assign(bytes, bytes + length);
            return length;
        });

        EXPECT_CALL(_canService, Send(_transmitId, _, 3))
            .WillOnce([](const CANIdentifier_t identifier, const CANData_t data, const uint8_t dataLength) {
                EXPECT_EQ(0x30, data.Data[0]);
                EXPECT_EQ(0x00, data.Data[1]);
                EXPECT_EQ(0x00, data.Data[2]);
            });

        const CANData_t firstFrame = { 0x10, 0x0A, 0, 1, 2, 3, 4, 5 };
        const CANData_t consecutiveFrame = { 0x21, 6, 7, 8, 9, 0, 0, 0 };
        _canService.Receive(_listenId, firstFrame, 8);
        _canService.Receive(_listenId, consecutiveFrame, 5);

        ASSERT_EQ(10u, received.size());
        for(size_t i = 0; i < received.size(); ++i)
        {
            EXPECT_EQ(i, received[i]);
        }
    }

    TEST_F(CommunicationService_ISOTPTests, AbortsMultiFrameSendOnFlowControlOverflow)
    {
        CommunicationService_ISOTP service(&_canService, _listenId, _transmitId);
        const uint8_t payload[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        // Only first frame should be sent; consecutive frames must NOT be sent
        EXPECT_CALL(_canService, Send(_transmitId, _, 8)).Times(1);

        service.Send(payload, sizeof(payload));
        // Simulate overflow flow control (FS=2)
        const CANData_t flowControlOverflow = { 0x32, 0x00, 0x00, 0, 0, 0, 0, 0 };
        _canService.Receive(_listenId, flowControlOverflow, 3);
    }
}
