#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "ATTiny427_EVSYSService.h"
#include <cstring>

using namespace testing;
using namespace EmbeddedIOServices;

namespace UnitTests
{
    class ATTiny427_EVSYSServiceTest : public ::testing::Test
    {
    protected:
        ATTiny427_ExpanderService expanderService{ATTiny427_ExpanderComm_UART0};
        ATTiny427_EVSYSService evsysService{&expanderService};
        uint8_t transmitBuffer[256];
    };

    // Tests for service initialization
    TEST_F(ATTiny427_EVSYSServiceTest, WhenServiceCreated_ThenChannelRegistersAreInitialized)
    {
        // Verify that all channel registers are accessible
        for(int i = 0; i < 6; i++)
        {
            ASSERT_NE(nullptr, evsysService.EVSYS_CHANNEL[i]);
        }
    }

    TEST_F(ATTiny427_EVSYSServiceTest, WhenServiceCreated_ThenUserRegistersAreInitialized)
    {
        // Verify that all user registers are accessible
        ASSERT_NE(nullptr, evsysService.EVSYS_CCL_LUT0A);
        ASSERT_NE(nullptr, evsysService.EVSYS_CCL_LUT1A);
        ASSERT_NE(nullptr, evsysService.EVSYS_CCL_LUT2A);
        ASSERT_NE(nullptr, evsysService.EVSYS_CCL_LUT3A);
        ASSERT_NE(nullptr, evsysService.EVSYS_EVOUTA);
        ASSERT_NE(nullptr, evsysService.EVSYS_EVOUTB);
        ASSERT_NE(nullptr, evsysService.EVSYS_EVOUTC);
        ASSERT_NE(nullptr, evsysService.EVSYS_TCB0_CAPT);
        ASSERT_NE(nullptr, evsysService.EVSYS_TCB1_CAPT);
    }

    TEST_F(ATTiny427_EVSYSServiceTest, WhenServiceCreated_ThenUserRegistersAreAccessibleByArray)
    {
        // Verify that user registers are accessible through the array
        for(int i = 0; i < 9; i++)
        {
            ASSERT_NE(nullptr, evsysService.EVSYS_USER[i]);
        }
    }

    // Tests for InitEVSYSUser with low pin group (0-7)
    TEST_F(ATTiny427_EVSYSServiceTest, WhenInitEVSYSUserWithPin0_ThenSucceeds)
    {
        auto &userReg = *evsysService.EVSYS_EVOUTA;
        bool result = evsysService.InitEVSYSUser(userReg, 0);
        
        ASSERT_TRUE(result);
        // User register should be set to a channel + 1
        ASSERT_NE(0, (uint8_t)userReg);
        ASSERT_GE((uint8_t)userReg, 1);
        ASSERT_LE((uint8_t)userReg, 6);
    }

    TEST_F(ATTiny427_EVSYSServiceTest, WhenInitEVSYSUserWithPin7_ThenSucceeds)
    {
        auto &userReg = *evsysService.EVSYS_EVOUTA;
        bool result = evsysService.InitEVSYSUser(userReg, 7);
        
        ASSERT_TRUE(result);
        ASSERT_NE(0, (uint8_t)userReg);
    }

    TEST_F(ATTiny427_EVSYSServiceTest, WhenInitEVSYSUserWithPin8_ThenSucceeds)
    {
        auto &userReg = *evsysService.EVSYS_EVOUTA;
        bool result = evsysService.InitEVSYSUser(userReg, 8);
        
        ASSERT_TRUE(result);
        ASSERT_NE(0, (uint8_t)userReg);
    }

    TEST_F(ATTiny427_EVSYSServiceTest, WhenInitEVSYSUserWithPin15_ThenSucceeds)
    {
        auto &userReg = *evsysService.EVSYS_EVOUTA;
        bool result = evsysService.InitEVSYSUser(userReg, 15);
        
        ASSERT_TRUE(result);
        ASSERT_NE(0, (uint8_t)userReg);
    }

    TEST_F(ATTiny427_EVSYSServiceTest, WhenInitEVSYSUserWithPin16_ThenSucceeds)
    {
        auto &userReg = *evsysService.EVSYS_EVOUTA;
        bool result = evsysService.InitEVSYSUser(userReg, 16);
        
        ASSERT_TRUE(result);
        ASSERT_NE(0, (uint8_t)userReg);
    }

    TEST_F(ATTiny427_EVSYSServiceTest, WhenInitEVSYSUserWithPin22_ThenFails)
    {
        auto &userReg = *evsysService.EVSYS_EVOUTA;
        bool result = evsysService.InitEVSYSUser(userReg, 22);
        
        ASSERT_FALSE(result);
        ASSERT_EQ(0, (uint8_t)userReg);
    }

    TEST_F(ATTiny427_EVSYSServiceTest, WhenInitEVSYSUserWithInvalidPin24_ThenFails)
    {
        auto &userReg = *evsysService.EVSYS_EVOUTA;
        bool result = evsysService.InitEVSYSUser(userReg, 24);
        
        ASSERT_FALSE(result);
    }

    TEST_F(ATTiny427_EVSYSServiceTest, WhenInitEVSYSUserWithInvalidPin100_ThenFails)
    {
        auto &userReg = *evsysService.EVSYS_EVOUTA;
        bool result = evsysService.InitEVSYSUser(userReg, 100);
        
        ASSERT_FALSE(result);
    }

    // Tests for multiple users
    TEST_F(ATTiny427_EVSYSServiceTest, WhenInitMultipleDifferentPins_ThenAllSucceed)
    {
        auto &user1 = *evsysService.EVSYS_EVOUTA;
        auto &user2 = *evsysService.EVSYS_EVOUTB;
        auto &user3 = *evsysService.EVSYS_EVOUTC;
        
        bool result1 = evsysService.InitEVSYSUser(user1, 0);
        bool result2 = evsysService.InitEVSYSUser(user2, 1);
        bool result3 = evsysService.InitEVSYSUser(user3, 2);
        
        ASSERT_TRUE(result1);
        ASSERT_TRUE(result2);
        ASSERT_TRUE(result3);
        
        // All should have non-zero values
        ASSERT_NE(0, (uint8_t)user1);
        ASSERT_NE(0, (uint8_t)user2);
        ASSERT_NE(0, (uint8_t)user3);
    }

    TEST_F(ATTiny427_EVSYSServiceTest, WhenInitSamePinMultipleTimes_ThenChannelReused)
    {
        auto &user1 = *evsysService.EVSYS_EVOUTA;
        auto &user2 = *evsysService.EVSYS_EVOUTB;
        
        bool result1 = evsysService.InitEVSYSUser(user1, 0);
        bool result2 = evsysService.InitEVSYSUser(user2, 0);
        
        ASSERT_TRUE(result1);
        ASSERT_TRUE(result2);
        
        // Should be able to initialize same pin for multiple users
        ASSERT_NE(0, (uint8_t)user1);
        ASSERT_EQ((uint8_t)user1, (uint8_t)user2);
    }

    // Tests for DeinitEVSYSUser
    TEST_F(ATTiny427_EVSYSServiceTest, WhenDeinitEVSYSUser_ThenUserRegisterCleared)
    {
        auto &userReg = *evsysService.EVSYS_EVOUTA;
        
        evsysService.InitEVSYSUser(userReg, 5);
        ASSERT_NE(0, (uint8_t)userReg);
        
        evsysService.DeinitEVSYSUser(userReg);
        ASSERT_EQ(0, (uint8_t)userReg);
    }

    TEST_F(ATTiny427_EVSYSServiceTest, WhenDeinitEVSYSUserNotAlone_ThenChannelNotCleared)
    {
        auto &user1 = *evsysService.EVSYS_EVOUTA;
        auto &user2 = *evsysService.EVSYS_EVOUTB;
        
        evsysService.InitEVSYSUser(user1, 5);
        evsysService.InitEVSYSUser(user2, 5);
        
        uint8_t channelValue1 = user1;
        uint8_t channelValue2 = user2;

        ASSERT_EQ(channelValue1, channelValue2);
        
        // Deinit first user
        evsysService.DeinitEVSYSUser(user1);
        ASSERT_EQ(0, user1);
        
        // Second user and its channel should still be active
        ASSERT_EQ(channelValue2, user2);
        uint8_t channelNum = user2 - 1;
        ASSERT_NE(0, *evsysService.EVSYS_CHANNEL[channelNum]);
    }

    TEST_F(ATTiny427_EVSYSServiceTest, WhenDeinitLastUserOfChannel_ThenChannelRegisterCleared)
    {
        auto &userReg = *evsysService.EVSYS_EVOUTA;
        
        evsysService.InitEVSYSUser(userReg, 10);
        uint8_t channelNum = (uint8_t)userReg - 1;
        
        evsysService.DeinitEVSYSUser(userReg);
        
        // Channel register should be cleared to 0
        ASSERT_EQ(0, *evsysService.EVSYS_CHANNEL[channelNum]);
    }

    // Tests for AC (Analog Comparator) mode
    TEST_F(ATTiny427_EVSYSServiceTest, WhenInitEVSYSUserWithAC_ThenSucceeds)
    {
        auto &userReg = *evsysService.EVSYS_EVOUTA;
        bool result = evsysService.InitEVSYSUser(userReg, 0, true);
        
        ASSERT_TRUE(result);
        ASSERT_NE(0, (uint8_t)userReg);
        ASSERT_EQ(0x20, *evsysService.EVSYS_CHANNEL[(uint8_t)userReg - 1]);
    }

    TEST_F(ATTiny427_EVSYSServiceTest, WhenInitMultipleWithAC_ThenAllSucceed)
    {
        auto &user1 = *evsysService.EVSYS_EVOUTA;
        auto &user2 = *evsysService.EVSYS_EVOUTB;
        
        bool result1 = evsysService.InitEVSYSUser(user1, 0, true);
        bool result2 = evsysService.InitEVSYSUser(user2, 0, true);
        
        ASSERT_TRUE(result1);
        ASSERT_TRUE(result2);
        ASSERT_EQ((uint8_t)user1, (uint8_t)user2);
    }

    // Tests for different user registers
    TEST_F(ATTiny427_EVSYSServiceTest, WhenInitAllCCLLUTs_ThenAllSucceed)
    {
        bool result0 = evsysService.InitEVSYSUser(*evsysService.EVSYS_CCL_LUT0A, 0);
        bool result1 = evsysService.InitEVSYSUser(*evsysService.EVSYS_CCL_LUT1A, 1);
        bool result2 = evsysService.InitEVSYSUser(*evsysService.EVSYS_CCL_LUT2A, 2);
        bool result3 = evsysService.InitEVSYSUser(*evsysService.EVSYS_CCL_LUT3A, 3);
        
        ASSERT_TRUE(result0);
        ASSERT_TRUE(result1);
        ASSERT_TRUE(result2);
        ASSERT_TRUE(result3);
    }

    TEST_F(ATTiny427_EVSYSServiceTest, WhenInitAllEVOUTs_ThenAllSucceed)
    {
        bool resultA = evsysService.InitEVSYSUser(*evsysService.EVSYS_EVOUTA, 0);
        bool resultB = evsysService.InitEVSYSUser(*evsysService.EVSYS_EVOUTB, 1);
        bool resultC = evsysService.InitEVSYSUser(*evsysService.EVSYS_EVOUTC, 2);
        
        ASSERT_TRUE(resultA);
        ASSERT_TRUE(resultB);
        ASSERT_TRUE(resultC);
    }

    TEST_F(ATTiny427_EVSYSServiceTest, WhenInitAllTimerCaptures_ThenAllSucceed)
    {
        bool result0 = evsysService.InitEVSYSUser(*evsysService.EVSYS_TCB0_CAPT, 0);
        bool result1 = evsysService.InitEVSYSUser(*evsysService.EVSYS_TCB1_CAPT, 1);
        
        ASSERT_TRUE(result0);
        ASSERT_TRUE(result1);
    }

    // Tests for pin ranges
    TEST_F(ATTiny427_EVSYSServiceTest, WhenInitPinsFromLowGroup_ThenUseCorrectChannels)
    {
        // Pins 0-7 should prefer channels 0-3
        for(int pin = 0; pin < 8; pin++)
        {
            ATTiny427_ExpanderService expanderService2{ATTiny427_ExpanderComm_UART0};
            ATTiny427_EVSYSService evsysService2{&expanderService2};
            auto &userReg = *evsysService2.EVSYS_EVOUTA;
            
            bool result = evsysService2.InitEVSYSUser(userReg, pin);
            ASSERT_TRUE(result);
            ASSERT_EQ(1, (uint8_t)userReg);
        }
    }

    TEST_F(ATTiny427_EVSYSServiceTest, WhenInitPinsFromMidGroup_ThenUseCorrectChannels)
    {
        // Pins 8-15 should prefer channels 4-5 then 0-1
        for(int pin = 8; pin < 16; pin++)
        {
            ATTiny427_ExpanderService expanderService2{ATTiny427_ExpanderComm_UART0};
            ATTiny427_EVSYSService evsysService2{&expanderService2};
            auto &userReg = *evsysService2.EVSYS_EVOUTA;
            
            bool result = evsysService2.InitEVSYSUser(userReg, pin);
            ASSERT_TRUE(result);
            ASSERT_EQ(5, (uint8_t)userReg);
        }
    }

    TEST_F(ATTiny427_EVSYSServiceTest, WhenInitPinsFromHighGroup_ThenUseCorrectChannels)
    {
        // Pins 16-21 should prefer channels 2-5
        for(int pin = 16; pin < 22; pin++)
        {
            ATTiny427_ExpanderService expanderService2{ATTiny427_ExpanderComm_UART0};
            ATTiny427_EVSYSService evsysService2{&expanderService2};
            auto &userReg = *evsysService2.EVSYS_EVOUTA;
            
            bool result = evsysService2.InitEVSYSUser(userReg, pin);
            ASSERT_TRUE(result);
            ASSERT_EQ(3, (uint8_t)userReg);
        }
    }

    // Tests for channel exhaustion
    TEST_F(ATTiny427_EVSYSServiceTest, WhenAllChannelsExhausted_ThenNewInitFails)
    {
        // Exhaust all 6 channels with different pins
        bool r0 = evsysService.InitEVSYSUser(*evsysService.EVSYS_EVOUTA, 0);
        bool r1 = evsysService.InitEVSYSUser(*evsysService.EVSYS_EVOUTB, 1);
        bool r2 = evsysService.InitEVSYSUser(*evsysService.EVSYS_EVOUTC, 2);
        bool r3 = evsysService.InitEVSYSUser(*evsysService.EVSYS_CCL_LUT0A, 3);
        bool r4 = evsysService.InitEVSYSUser(*evsysService.EVSYS_CCL_LUT1A, 17);
        bool r5 = evsysService.InitEVSYSUser(*evsysService.EVSYS_CCL_LUT2A, 18);
        
        ASSERT_TRUE(r0);
        ASSERT_TRUE(r1);
        ASSERT_TRUE(r2);
        ASSERT_TRUE(r3);
        ASSERT_TRUE(r4);
        ASSERT_TRUE(r5);
        
        // Try to initialize with a pin that can't reuse any channel
        bool r7 = evsysService.InitEVSYSUser(*evsysService.EVSYS_CCL_LUT3A, 19);
        ASSERT_FALSE(r7);
    }

    // Tests for integration with ExpanderService
    TEST_F(ATTiny427_EVSYSServiceTest, WhenInitEVSYSUserQueuesWrite_ThenTransmitIncludesCommand)
    {
        memset(transmitBuffer, 0, sizeof(transmitBuffer));
        
        auto &userReg = *evsysService.EVSYS_EVOUTA;
        evsysService.InitEVSYSUser(userReg, 5);
        
        size_t size = expanderService.Transmit(transmitBuffer);
        ASSERT_GT(size, 0);  // Should have transmit data
    }

    TEST_F(ATTiny427_EVSYSServiceTest, WhenDeinitEVSYSUserQueuesWrite_ThenTransmitIncludesCommand)
    {
        auto &userReg = *evsysService.EVSYS_EVOUTA;
        evsysService.InitEVSYSUser(userReg, 5);
        
        memset(transmitBuffer, 0, sizeof(transmitBuffer));
        expanderService.Transmit(transmitBuffer);
        
        memset(transmitBuffer, 0, sizeof(transmitBuffer));
        evsysService.DeinitEVSYSUser(userReg);
        
        size_t size = expanderService.Transmit(transmitBuffer);
        ASSERT_GT(size, 0);  // Should have transmit data
    }

    // Tests for reassignment behavior
    TEST_F(ATTiny427_EVSYSServiceTest, WhenChannelNeedsReassignment_ThenExistingUserMigrates)
    {
        // Initialize with a pin that exhausts a particular channel
        auto &user1 = *evsysService.EVSYS_EVOUTA;
        evsysService.InitEVSYSUser(user1, 9);
        evsysService.InitEVSYSUser(*evsysService.EVSYS_EVOUTC, 10);
        evsysService.InitEVSYSUser(*evsysService.EVSYS_CCL_LUT0A, 17);
        evsysService.InitEVSYSUser(*evsysService.EVSYS_CCL_LUT1A, 18);

        uint8_t originalChannel = user1;
        
        auto &user2 = *evsysService.EVSYS_EVOUTB;
        // This should trigger reassignment of user1 to a different channel
        bool result = evsysService.InitEVSYSUser(user2, 19);
        
        ASSERT_TRUE(result);
        ASSERT_NE(originalChannel, user1);
        ASSERT_NE(0, user2);
    }

    // Edge case tests
    TEST_F(ATTiny427_EVSYSServiceTest, WhenInitDeinitInit_ThenChannelReleased)
    {
        auto &userReg = *evsysService.EVSYS_EVOUTA;
        uint8_t firstChannel = 0;
        
        // First init
        evsysService.InitEVSYSUser(userReg, 10);
        firstChannel = (uint8_t)userReg;
        
        // Deinit
        evsysService.DeinitEVSYSUser(userReg);
        ASSERT_EQ(0, (uint8_t)userReg);
        
        // Second init with same pin should reuse the channel
        evsysService.InitEVSYSUser(userReg, 10);
        ASSERT_NE(0, (uint8_t)userReg);
    }

    TEST_F(ATTiny427_EVSYSServiceTest, WhenInitDeinitInitDifferentPin_ThenChannelReused)
    {
        auto &userReg = *evsysService.EVSYS_EVOUTA;
        
        evsysService.InitEVSYSUser(userReg, 10);
        uint8_t firstChannel = (uint8_t)userReg;
        
        evsysService.DeinitEVSYSUser(userReg);
        ASSERT_EQ(0, (uint8_t)userReg);
        
        evsysService.InitEVSYSUser(userReg, 11);
        uint8_t secondChannel = (uint8_t)userReg;
        
        // Channels might be different or same depending on implementation
        ASSERT_EQ(firstChannel, secondChannel);
    }

    TEST_F(ATTiny427_EVSYSServiceTest, WhenMultipleServiceInstances_ThenEachManagesOwnState)
    {
        ATTiny427_ExpanderService expander1{ATTiny427_ExpanderComm_UART0};
        ATTiny427_ExpanderService expander2{ATTiny427_ExpanderComm_UART1};
        ATTiny427_EVSYSService evsys1{&expander1};
        ATTiny427_EVSYSService evsys2{&expander2};
        
        bool r1 = evsys1.InitEVSYSUser(*evsys1.EVSYS_EVOUTA, 0);
        bool r2 = evsys2.InitEVSYSUser(*evsys2.EVSYS_EVOUTA, 0);
        
        ASSERT_TRUE(r1);
        ASSERT_TRUE(r2);
        
        // Both should be able to initialize independent users
        evsys1.DeinitEVSYSUser(*evsys1.EVSYS_EVOUTA);
        ASSERT_EQ(0, (uint8_t)*evsys1.EVSYS_EVOUTA);
        ASSERT_NE(0, (uint8_t)*evsys2.EVSYS_EVOUTA);
    }
}
