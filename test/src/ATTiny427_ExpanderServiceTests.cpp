#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "ATTiny427_ExpanderService.h"
#include <cstring>

using namespace testing;
using namespace EmbeddedIOServices;

namespace UnitTests
{
    class ATTiny427_ExpanderServiceTest : public ::testing::Test
    {
    protected:
        ATTiny427_ExpanderService uartService{ATTiny427_ExpanderComm_UART0};
        ATTiny427_ExpanderService spiService{ATTiny427_ExpanderComm_SPI};
        
        uint8_t transmitBuffer[256];
        uint8_t receiveBuffer[256];
    };

    // Tests for basic service initialization
    TEST_F(ATTiny427_ExpanderServiceTest, WhenServiceCreated_ThenCommTypeIsSet)
    {
        ASSERT_EQ(ATTiny427_ExpanderComm_UART0, uartService.Comm);
        ASSERT_EQ(ATTiny427_ExpanderComm_SPI, spiService.Comm);
    }

    TEST_F(ATTiny427_ExpanderServiceTest, WhenWritingToRegister_ThenQueuedCommandIsGenerated)
    {
        memset(transmitBuffer, 0, sizeof(transmitBuffer));
        
        uartService.Write(0x0010, 0xAB);
        size_t size = uartService.Transmit(transmitBuffer);
        
        // Should have generated a transmit with write command
        ASSERT_EQ(size, 3);
        // For UART with 8-bit address, format is: cmd | length, address_low, data
        // cmd = 0xB1 (write 8-bit address high byte 0, length 1)
        ASSERT_EQ(0xB1, transmitBuffer[0]);
        ASSERT_EQ(0x10, transmitBuffer[1]);
        ASSERT_EQ(0xAB, transmitBuffer[2]);
    }

    TEST_F(ATTiny427_ExpanderServiceTest, WhenWritingMultipleValues_ThenAllQueued)
    {
        memset(transmitBuffer, 0, sizeof(transmitBuffer));
        
        uartService.Write(0x0010, 0xAA);
        uartService.Write(0x0012, 0xBB);
        uartService.Write(0x0014, 0xCC);
        
        size_t size = uartService.Transmit(transmitBuffer);
        
        ASSERT_EQ(size, 9);
        // Should contain write commands for multiple addresses
        ASSERT_EQ(0xB1, transmitBuffer[0]);  // cmd for 0x0010
        ASSERT_EQ(0x10, transmitBuffer[1]);  // address low byte
        ASSERT_EQ(0xAA, transmitBuffer[2]);  // data
        ASSERT_EQ(0xB1, transmitBuffer[3]);  // cmd for 0x0012
        ASSERT_EQ(0x12, transmitBuffer[4]);  // address low byte
        ASSERT_EQ(0xBB, transmitBuffer[5]);  // data
        ASSERT_EQ(0xB1, transmitBuffer[6]);  // cmd for 0x0014
        ASSERT_EQ(0x14, transmitBuffer[7]);  // address low byte
        ASSERT_EQ(0xCC, transmitBuffer[8]);  // data
    }

    TEST_F(ATTiny427_ExpanderServiceTest, WhenCoalescingConsecutiveWrites_ThenSingleWriteCommandUsed)
    {
        memset(transmitBuffer, 0, sizeof(transmitBuffer));
        
        // Write consecutive addresses
        uartService.Write(0x0010, 0xAA);
        uartService.Write(0x0011, 0xBB);
        
        size_t size = uartService.Transmit(transmitBuffer);
        
        ASSERT_EQ(size, 4);
        // The two consecutive writes should be coalesced into one write command with count=2
        // cmd = 0xB2 (write 8 bit address high byte 0, length 2)
        ASSERT_EQ(0xB2, transmitBuffer[0]);  // cmd with length 2
        ASSERT_EQ(0x10, transmitBuffer[1]);  // address low byte
        ASSERT_EQ(0xAA, transmitBuffer[2]);  // data for 0x0010
        ASSERT_EQ(0xBB, transmitBuffer[3]);  // data for 0x0011
    }

    // Tests for register access
    TEST_F(ATTiny427_ExpanderServiceTest, WhenGettingRegister_ThenRegisterIsReturned)
    {
        auto &reg = uartService.GetRegister(0x0020);
        ASSERT_EQ(0x0020, reg.GetAddress());
    }

    TEST_F(ATTiny427_ExpanderServiceTest, WhenAccessingSameRegisterTwice_ThenSameRegisterReturned)
    {
        auto &reg1 = uartService.GetRegister(0x0030);
        auto &reg2 = uartService.GetRegister(0x0030);
        ASSERT_EQ(&reg1, &reg2);
    }

    // Tests for Poller functionality
    TEST_F(ATTiny427_ExpanderServiceTest, WhenCreatingPoller_ThenPollerIsInitialized)
    {
        uint8_t callbackCalled = 0;
        auto callback = [&callbackCalled](uint8_t *data) { callbackCalled = 1; };
        
        auto poller = std::make_unique<ATTiny427_ExpanderService::ATTiny427_ExpanderPoller>(
            &uartService, 0x0050, 4, callback);
        poller->SetLength(4);
        
        ASSERT_EQ(0x0050, poller->GetAddress());
        ASSERT_EQ(4, poller->GetLength());
        ASSERT_FALSE(poller->IsEnabled());
    }

    TEST_F(ATTiny427_ExpanderServiceTest, WhenEnablingPoller_ThenPollerIsEnabled)
    {
        uint8_t callbackCalled = 0;
        auto callback = [&callbackCalled](uint8_t *data) { callbackCalled = 1; };
        
        auto poller = std::make_unique<ATTiny427_ExpanderService::ATTiny427_ExpanderPoller>(
            &uartService, 0x0050, 4, callback);
        
        poller->SetLength(4);
        ASSERT_FALSE(poller->IsEnabled());

        poller->SetEnabled(true);
        ASSERT_FALSE(poller->IsEnabled());
        memset(transmitBuffer, 0, sizeof(transmitBuffer));
        uartService.Transmit(transmitBuffer);
        
        ASSERT_TRUE(poller->IsEnabled());
    }

    TEST_F(ATTiny427_ExpanderServiceTest, WhenDisablingPoller_ThenPollerIsDisabled)
    {
        uint8_t callbackCalled = 0;
        auto callback = [&callbackCalled](uint8_t *data) { callbackCalled = 1; };
        
        auto poller = std::make_unique<ATTiny427_ExpanderService::ATTiny427_ExpanderPoller>(
            &uartService, 0x0050, 4, callback);

        poller->SetLength(4);
        ASSERT_FALSE(poller->IsEnabled());
        
        poller->SetEnabled(true);
        ASSERT_FALSE(poller->IsEnabled());
        memset(transmitBuffer, 0, sizeof(transmitBuffer));
        uartService.Transmit(transmitBuffer);
        ASSERT_TRUE(poller->IsEnabled());
        
        poller->SetEnabled(false);
        ASSERT_FALSE(poller->IsEnabled());
    }

    TEST_F(ATTiny427_ExpanderServiceTest, WhenSettingPollerLength_ThenLengthIsUpdated)
    {
        uint8_t callbackCalled = 0;
        auto callback = [&callbackCalled](uint8_t *data) { callbackCalled = 1; };
        
        auto poller = std::make_unique<ATTiny427_ExpanderService::ATTiny427_ExpanderPoller>(
            &uartService, 0x0050, 10, callback);
        
        ASSERT_EQ(0, poller->GetLength());
        poller->SetLength(5);
        ASSERT_EQ(5, poller->GetLength());
    }

    TEST_F(ATTiny427_ExpanderServiceTest, WhenSettingLengthExceedsMax_ThenLengthIsLimited)
    {
        uint8_t callbackCalled = 0;
        auto callback = [&callbackCalled](uint8_t *data) { callbackCalled = 1; };
        
        auto poller = std::make_unique<ATTiny427_ExpanderService::ATTiny427_ExpanderPoller>(
            &uartService, 0x0050, 10, callback);
        
        poller->SetLength(20);  // Try to set beyond max
        ASSERT_EQ(10, poller->GetLength());  // Should be limited to max
    }

    TEST_F(ATTiny427_ExpanderServiceTest, WhenPollerReceivesCompleteData_ThenCallbackIsCalled)
    {
        uint8_t callbackData[4] = {0};
        uint8_t callbackCalled = 0;
        
        auto callback = [&callbackCalled, &callbackData](uint8_t *data) {
            callbackCalled = 1;
            std::memcpy(callbackData, data, 4);
        };
        
        auto poller = std::make_unique<ATTiny427_ExpanderService::ATTiny427_ExpanderPoller>(
            &uartService, 0x0050, 4, callback);
        
        poller->SetLength(4);
        poller->SetReadIdx(0);
        
        uint8_t testData[4] = {0x11, 0x22, 0x33, 0x44};
        poller->Receive(testData, 4, 0);
        
        ASSERT_EQ(1, callbackCalled);
        ASSERT_EQ(0x11, callbackData[0]);
        ASSERT_EQ(0x44, callbackData[3]);
    }

    TEST_F(ATTiny427_ExpanderServiceTest, WhenPollerIsDisabled_ThenReceiveDoesNothing)
    {
        uint8_t callbackCalled = 0;
        auto callback = [&callbackCalled](uint8_t *data) { callbackCalled = 1; };
        
        auto poller = std::make_unique<ATTiny427_ExpanderService::ATTiny427_ExpanderPoller>(
            &uartService, 0x0050, 4, callback);
        
        poller->SetLength(4);
        // Don't enable poller
        
        uint8_t testData[4] = {0x11, 0x22, 0x33, 0x44};
        poller->Receive(testData, 4, 0);
        
        ASSERT_EQ(0, callbackCalled);
    }

    TEST_F(ATTiny427_ExpanderServiceTest, WhenReceivingDataPartially_ThenCallbackWaitsForComplete)
    {
        uint8_t callbackCalled = 0;
        auto callback = [&callbackCalled](uint8_t *data) { callbackCalled++; };
        
        auto poller = std::make_unique<ATTiny427_ExpanderService::ATTiny427_ExpanderPoller>(
            &uartService, 0x0050, 4, callback);
        
        poller->SetLength(4);
        poller->SetReadIdx(0);
        
        // Receive 2 bytes
        uint8_t testData1[2] = {0x11, 0x22};
        poller->Receive(testData1, 2, 0);
        ASSERT_EQ(0, callbackCalled);  // Not called yet
        
        // Receive remaining 2 bytes
        uint8_t testData2[2] = {0x33, 0x44};
        poller->Receive(testData2, 2, 2);
        ASSERT_EQ(1, callbackCalled);  // Now called
    }

    // Tests for Receive functionality
    TEST_F(ATTiny427_ExpanderServiceTest, WhenReceivingData_ThenDataIsPassedToPollers)
    {
        uint8_t callbackData[4] = {0};
        uint8_t callbackCalled = 0;
        
        auto callback = [&callbackCalled, &callbackData](uint8_t *data) {
            callbackCalled = 1;
            std::memcpy(callbackData, data, 4);
        };
        
        auto poller = std::make_unique<ATTiny427_ExpanderService::ATTiny427_ExpanderPoller>(
            &uartService, 0x0050, 4, callback);
        
        poller->SetLength(4);
        poller->SetEnabled(true);
        
        // Enable poller through transmit
        memset(transmitBuffer, 0, sizeof(transmitBuffer));
        uartService.Transmit(transmitBuffer);
        
        // Receive data
        uint8_t testData[8] = {0x32, 0x50, 0xAA, 0xBB, 0xCC, 0xDD, 0x00, 0x00 }; // added some extra bytes just cause
        uartService.Receive(testData, 8);
        
        ASSERT_EQ(1, callbackCalled);
    }

    // Tests for Transmit functionality with mixed commands
    TEST_F(ATTiny427_ExpanderServiceTest, WhenMixingReadAndWriteCommands_ThenBothProcessed)
    {
        memset(transmitBuffer, 0, sizeof(transmitBuffer));
        
        uint8_t callbackCalled = 0;
        auto callback = [&callbackCalled](uint8_t *data) { callbackCalled = 1; };
        
        // Create a poller for reading
        auto poller = std::make_unique<ATTiny427_ExpanderService::ATTiny427_ExpanderPoller>(
            &uartService, 0x0050, 4, callback);
        poller->SetLength(4);
        poller->SetEnabled(true);
        
        // Queue a write
        uartService.Write(0x0010, 0xAB);
        
        // Transmit should contain both
        size_t size = uartService.Transmit(transmitBuffer);
        ASSERT_EQ(size, 6);
        ASSERT_EQ(0x34, transmitBuffer[0]);  // Read command for poller ( read 8-bit address high byte 0, length 4)
        ASSERT_EQ(0x50, transmitBuffer[1]);  // Address low byte for poller
        ASSERT_EQ(0xB1, transmitBuffer[2]);  // Write command ( write 8-bit address high byte 0, length 1 )
        ASSERT_EQ(0x10, transmitBuffer[3]);  // Address low byte for write
        ASSERT_EQ(0xAB, transmitBuffer[4]);  // Data for write
        ASSERT_EQ(0x00, transmitBuffer[5]);  // Padding for read
    }

    // Tests for SPI mode behavior
    TEST_F(ATTiny427_ExpanderServiceTest, WhenUsingSPIMode_ThenInitialWriteIdxIsTwo)
    {
        memset(transmitBuffer, 0, sizeof(transmitBuffer));
        spiService.Write(0x0010, 0xAB);
        
        size_t size = spiService.Transmit(transmitBuffer);
        // SPI should have padding at the beginning
        ASSERT_GT(size, 2);
    }

    // Edge case tests
    TEST_F(ATTiny427_ExpanderServiceTest, WhenTransmittingWithNoCommands_ThenMinimalDataReturned)
    {
        memset(transmitBuffer, 0, sizeof(transmitBuffer));
        size_t size = uartService.Transmit(transmitBuffer);
        // Should return 0 bytes when no commands queued
        ASSERT_EQ(0, size);
    }

    TEST_F(ATTiny427_ExpanderServiceTest, WhenWritingToHighAddress_ThenTwoByteAddressCommand)
    {
        memset(transmitBuffer, 0, sizeof(transmitBuffer));
        
        uartService.Write(0x0100, 0xAB);
        size_t size = uartService.Transmit(transmitBuffer);
        
        ASSERT_EQ(size, 4);
        // High address (>0xFF) should use 2-byte address format
        ASSERT_EQ(0x81, transmitBuffer[0]);
    }

    TEST_F(ATTiny427_ExpanderServiceTest, WhenCommandQueueFull_ThenCommandsAreNotQueued)
    {
        // Queue many commands to potentially fill the buffer
        for(int i = 0; i < 200; i++)
        {
            uartService.Write(0x0010 + i, 0xAB + i);
        }
        
        memset(transmitBuffer, 0, sizeof(transmitBuffer));
        size_t size = uartService.Transmit(transmitBuffer);
        
        // Should process what it can
        ASSERT_GT(size, 200);
        ASSERT_LE(size, 256);
    }

    TEST_F(ATTiny427_ExpanderServiceTest, WhenPollerDestroyed_ThenReadDeleteCommandQueued)
    {
        memset(transmitBuffer, 0, sizeof(transmitBuffer));
        
        uint8_t callbackCalled = 0;
        auto callback = [&callbackCalled](uint8_t *data) { callbackCalled = 1; };
        
        auto poller = new ATTiny427_ExpanderService::ATTiny427_ExpanderPoller(
            &uartService, 0x0050, 4, callback);
        poller->SetLength(4);
        poller->SetEnabled(true);
        auto size1 = uartService.Transmit(transmitBuffer);
        ASSERT_EQ(size1, 6); // read command queued

        delete poller;  // Destroy poller
        
        // After destruction, should be able to transmit without issues
        memset(transmitBuffer, 0, sizeof(transmitBuffer));
        size_t size2 = uartService.Transmit(transmitBuffer);
        // Should complete without error and return no data
        ASSERT_EQ(size2, 0);
    }

    TEST_F(ATTiny427_ExpanderServiceTest, WhenWritingZeroValue_ThenZeroIsTransmitted)
    {
        memset(transmitBuffer, 0xFF, sizeof(transmitBuffer));  // Fill with 0xFF first
        
        uartService.Write(0x0010, 0x00);
        size_t size = uartService.Transmit(transmitBuffer);
        
        ASSERT_EQ(size, 3); // cmd, address, data
        // Data byte should be 0x00
        ASSERT_EQ(0xB1, transmitBuffer[0]);
        ASSERT_EQ(0x10, transmitBuffer[1]);
        ASSERT_EQ(0x00, transmitBuffer[2]);
    }

    TEST_F(ATTiny427_ExpanderServiceTest, WhenRegisterOperatorAssignment_ThenWriteIsQueued)
    {
        memset(transmitBuffer, 0, sizeof(transmitBuffer));
        
        auto &reg = uartService.GetRegister(0x0030);
        reg = 0x55;  // Use operator=
        
        size_t size = uartService.Transmit(transmitBuffer);
        ASSERT_EQ(size, 3); // write should be queued
        ASSERT_EQ(0xB1, transmitBuffer[0]);
        ASSERT_EQ(0x30, transmitBuffer[1]);
        ASSERT_EQ(0x55, transmitBuffer[2]);
    }

    TEST_F(ATTiny427_ExpanderServiceTest, WhenRegisterOperatorBitwiseOr_ThenOnlyWritesIfChanged)
    {
        memset(transmitBuffer, 0, sizeof(transmitBuffer));
        
        auto &reg = uartService.GetRegister(0x0040);
        reg = 0x30;
        memset(transmitBuffer, 0, sizeof(transmitBuffer));
        size_t size1 = uartService.Transmit(transmitBuffer);
        ASSERT_EQ(size1, 3); // write should be queued
        ASSERT_EQ(0xB1, transmitBuffer[0]);
        ASSERT_EQ(0x40, transmitBuffer[1]);
        ASSERT_EQ(0x30, transmitBuffer[2]);
        
        // OR with same bits
        reg |= 0x30;
        memset(transmitBuffer, 0, sizeof(transmitBuffer));
        size_t size2 = uartService.Transmit(transmitBuffer);
        ASSERT_EQ(size2, 0); // no write should be queued
        
        // Should not generate new command if value doesn't change
        // This depends on implementation
    }

    TEST_F(ATTiny427_ExpanderServiceTest, WhenMultiplePollers_ThenAllAreQueried)
    {
        memset(transmitBuffer, 0, sizeof(transmitBuffer));
        
        uint8_t callback1Called = 0;
        uint8_t callback2Called = 0;
        
        auto callback1 = [&callback1Called](uint8_t *data) { callback1Called = 1; };
        auto callback2 = [&callback2Called](uint8_t *data) { callback2Called = 1; };
        
        auto poller1 = std::make_unique<ATTiny427_ExpanderService::ATTiny427_ExpanderPoller>(
            &uartService, 0x0050, 4, callback1);
        auto poller2 = std::make_unique<ATTiny427_ExpanderService::ATTiny427_ExpanderPoller>(
            &uartService, 0x0060, 2, callback2);
        
        poller1->SetLength(4);
        poller1->SetEnabled(true);
        poller2->SetLength(2);
        poller2->SetEnabled(true);
        
        memset(transmitBuffer, 0, sizeof(transmitBuffer));
        size_t size = uartService.Transmit(transmitBuffer);
        
        // Should have room for both poller queries
        ASSERT_EQ(size, 8);
        ASSERT_EQ(0x34, transmitBuffer[0]);  // Read command for poller1
        ASSERT_EQ(0x50, transmitBuffer[1]);  // Address low byte for poller1
        ASSERT_EQ(0x00, transmitBuffer[2]);  // padding for poller1
        ASSERT_EQ(0x00, transmitBuffer[3]);  // padding for poller1
        ASSERT_EQ(0x32, transmitBuffer[4]);  // Read command for poller2
        ASSERT_EQ(0x60, transmitBuffer[5]);  // Address low byte for poller2
        ASSERT_EQ(0x00, transmitBuffer[6]);  // padding for poller2
        ASSERT_EQ(0x00, transmitBuffer[7]);  // padding for poller2
    }
}
