#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "ATTiny427_PassthroughService.h"
#include "ATTiny427_EVSYSService.h"
#include "ATTiny427_ExpanderService.h"
#include <cstring>

using namespace testing;
using namespace EmbeddedIOServices;

namespace ATTiny427_PassthroughServiceTests
{
    struct TestCase
    {
        ATTiny427_ExpanderComm commType;
        digitalpin_t pinIn;
        bool inverted;
        bool useAC;
    };

    class ATTiny427_PassthroughServiceTest : public ::testing::TestWithParam<TestCase>
    {
    protected:
        TestCase testCase;
        std::unique_ptr<ATTiny427_ExpanderService> expanderService;
        std::unique_ptr<ATTiny427_EVSYSService> evsysService;
        std::unique_ptr<ATTiny427_PassthroughService> passthroughService;
        uint8_t transmitBuffer[256];
        uint8_t evsysChannel;
        uint8_t evsysChannelGenerator;
        bool useAC;
        
        void SetUp() override
        {
            testCase = GetParam();
            useAC = testCase.useAC && (testCase.pinIn == 7 || testCase.pinIn == 13 || testCase.pinIn == 9 || testCase.pinIn == 14);
            if(useAC)
                evsysChannel = 0;
            else if(testCase.pinIn < 8)
                evsysChannel = 0;
            else if(testCase.pinIn < 16)
                evsysChannel = 4;
            else if(testCase.pinIn < 22)
                evsysChannel = 2;
            else
                evsysChannel = 0xFF; //invalid
            evsysChannelGenerator = useAC ? 0x20 : (0x40 | (testCase.pinIn & 0x7));

            expanderService = std::make_unique<ATTiny427_ExpanderService>(testCase.commType);
            evsysService = std::make_unique<ATTiny427_EVSYSService>(expanderService.get());
            passthroughService = std::make_unique<ATTiny427_PassthroughService>(expanderService.get(), evsysService.get());
        }

        void CheckACSetup(uint8_t &idx, uint8_t pinOut)
        {
            if(!useAC)
                return;

            //setup AC if needed
            ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_AC0_DACREF >> 8); //address high byte
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_AC0_DACREF & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0x85);

            uint8_t AC_MUXCTRLA;
            uint8_t AC_CTRLA = 0b10000111;
            switch(testCase.pinIn)
            {
                case 13:
                    AC_MUXCTRLA = 0x03 | (0x1 << 3);
                    break;
                case 9:
                    AC_MUXCTRLA = 0x03 | (0x2 << 3);
                    break;
                case 14:
                    AC_MUXCTRLA = 0x03 | (0x3 << 3);
                    break;
            }
            if(pinOut == 5)
            {
                AC_MUXCTRLA |= testCase.inverted? 0b10000000 : 0;
                AC_CTRLA |= 0b01000000; //enable output
            }
            
            ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_AC0_MUXCTRLA & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], AC_MUXCTRLA);

            ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_AC0_CTRLA & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], AC_CTRLA);
        }
    };

    std::list<TestCase> CreateTestCases()
    {
        std::list<TestCase> testCases;
        for(auto commType : {ATTiny427_ExpanderComm_UART0, ATTiny427_ExpanderComm_UART0Alternate, ATTiny427_ExpanderComm_UART1, ATTiny427_ExpanderComm_UART1Alternate, ATTiny427_ExpanderComm_SPI, ATTiny427_ExpanderComm_SPIAlternate})
        {
            for(digitalpin_t pinIn = 0; pinIn < 23; pinIn++)
            {
                for(bool inverted : {false, true})
                {
                    for(bool useAC : {false, true})
                    {
                        testCases.push_back({commType, pinIn, inverted, useAC});
                    }
                }
            }
        }
        return testCases;
    }

    INSTANTIATE_TEST_SUITE_P(
        ATTiny427_PassthroughServiceTest,
        ATTiny427_PassthroughServiceTest,
        ::testing::ValuesIn(CreateTestCases())
    );

    TEST_P(ATTiny427_PassthroughServiceTest, WhenInitPassthrough_InPinDeinittedCorrectly)
    {
        const digitalpin_t pinOut = testCase.pinIn == 10? 15 : 10;
        //initdeinit so registers are known state
        passthroughService->InitPassthrough(testCase.pinIn, pinOut, testCase.inverted, testCase.useAC);
        passthroughService->DeinitPassthrough(pinOut);
        expanderService->Transmit(transmitBuffer);

        if(!passthroughService->InitPassthrough(testCase.pinIn == 7? 13 : 7, testCase.pinIn, testCase.inverted, testCase.useAC))
        {
            //if init failed, nothing should be transmitted
            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        //we don't care about this init, so just clear out the commands with a transmit
        expanderService->Transmit(transmitBuffer);

        //lets just use EVOUTB for testing since all comms have it
        bool result = passthroughService->InitPassthrough(testCase.pinIn, pinOut, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        const size_t endPadding = 3+(testCase.inverted? 4 : 0)+(useAC? 10 : 0);
        const size_t size = expanderService->Transmit(transmitBuffer);
        ASSERT_GE(size, endPadding + 8);
        for(uint8_t idx = 0; idx < size-2; idx++)
        {
            //look for EVOUTB setup
            if( transmitBuffer[idx] == 0xA1 &&
                transmitBuffer[idx + 1] == (ADDRESS_EVSYS_EVOUTB & 0xFF) &&
                transmitBuffer[idx + 2] == (evsysChannel + 1))
            {
                ASSERT_EQ(idx, size-endPadding);
                return;
            }
        }
        ASSERT_TRUE(false) << "EVOUTB setup not found in transmitted data";
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenInitPassthroughToPin2_ThenEVOUTAIsConfiguredCorrectly)
    {
        if(testCase.pinIn == 2)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 2, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        if(testCase.commType == ATTiny427_ExpanderComm_SPI || 
            testCase.commType == ATTiny427_ExpanderComm_UART0Alternate || 
            testCase.commType == ATTiny427_ExpanderComm_UART1)
        {
            SCOPED_TRACE(testing::Message() << "UnsupportedPassthrough: " << testCase.commType);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 2, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 2, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 2, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTMUX_EVSYSROUTEA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTMUX_EVSYSROUTEA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x00); //this is the primary EVOUTA mapping
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], evsysChannelGenerator); //generator set to pinIn
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_EVOUTA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], evsysChannel + 1); //set EVOUTA to channel
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTA_PINCTRL(2) >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTA_PINCTRL(2) & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], testCase.inverted ? 0x80 : 0x00); //inverted or not

        CheckACSetup(idx, 2);
        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenDeinitPassthroughToPin2_ThenEVOUTADeconfiguredCorrectly)
    {
        if(testCase.pinIn == 2)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        if(testCase.commType == ATTiny427_ExpanderComm_SPI || 
            testCase.commType == ATTiny427_ExpanderComm_UART0Alternate || 
            testCase.commType == ATTiny427_ExpanderComm_UART1)
        {
            SCOPED_TRACE(testing::Message() << "UnsupportedPassthrough: " << testCase.commType);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 2, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        expanderService->Transmit(transmitBuffer);

        passthroughService->DeinitPassthrough(2);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_EVOUTA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_EVOUTA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0); //set EVOUTA to 0 disabled
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high addr
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0); //generator set to 0 (disabled)
        if(testCase.inverted)
        {
            ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTA_PINCTRL(2) >> 8); //address high byte
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTA_PINCTRL(2) & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0x00); //remove inverted
        }

        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenInitPassthroughToPin4_ThenLUT0IsConfiguredCorrectly)
    {
        if(testCase.pinIn == 4)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 4, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        if(testCase.commType == ATTiny427_ExpanderComm_SPI)
        {
            SCOPED_TRACE(testing::Message() << "UnsupportedPassthrough: " << testCase.commType);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 4, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 4, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 4, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        if(useAC)
        {
            CheckACSetup(idx, 4);
            ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

            ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT0CTRLB & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0x66); 
            ASSERT_EQ(transmitBuffer[idx++], 0x00); 
        }
        else
        {
            switch(testCase.pinIn)
            {
                case 0: 
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT0CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x05); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    break;
                case 1:
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT0CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x50); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    break;
                case 2:
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT0CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x05); 
                    break;
                default:
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], evsysChannelGenerator); //generator set to pinIn
                    ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CCL_LUT0A & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], evsysChannel + 1); //set EVOUTA to channel

                    ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT0CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x03); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    break;
            }
        }
        ASSERT_EQ(transmitBuffer[idx++], testCase.inverted? 0x1 : 0xFE);

        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTMUX_CCLROUTEA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTMUX_CCLROUTEA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x00); //this is the primary LUT0 output

        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT0CTRLA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT0CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x41); //turn on CCL LUT0

        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x41); //turn on CCL

        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenDeinitPassthroughToPin4_ThenLUT0DeconfiguredCorrectly)
    {
        if(testCase.pinIn == 4)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        if(testCase.commType == ATTiny427_ExpanderComm_SPI)
        {
            SCOPED_TRACE(testing::Message() << "UnsupportedPassthrough: " << testCase.commType);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 4, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        expanderService->Transmit(transmitBuffer);

        const bool usesEVSYS = *evsysService->EVSYS_CCL_LUT0A != 0;

        passthroughService->DeinitPassthrough(4);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x40); //set CCL_CTRLA to 0 disabled
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT0CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x00); //set CCL_CTRLA to 0 disabled
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CCL_LUT0A & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0); //set EVSYS to 0 disabled
        if(usesEVSYS)
        {
            ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high addr
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0); //generator set to 0 (disabled)
        }

        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenInitPassthroughToPin5_ThenLUT3IsConfiguredCorrectly)
    {
        if(testCase.pinIn == 5)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 5, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 5, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 5, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        if(useAC)
        {
            CheckACSetup(idx, 5);
            ASSERT_EQ(size, idx);
            return;
        }
        switch(testCase.pinIn)
        {
            case 16: 
                ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
                ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT3CTRLB & 0xFF); //address low byte
                ASSERT_EQ(transmitBuffer[idx++], 0x05); 
                ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                break;
            case 17:
                ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
                ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT3CTRLB & 0xFF); //address low byte
                ASSERT_EQ(transmitBuffer[idx++], 0x50); 
                ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                break;
            case 18:
                ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
                ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT3CTRLB & 0xFF); //address low byte
                ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                ASSERT_EQ(transmitBuffer[idx++], 0x05); 
                break;
            default:
                ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) >> 8); //address high byte
                ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
                ASSERT_EQ(transmitBuffer[idx++], evsysChannelGenerator); //generator set to pinIn
                ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
                ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CCL_LUT3A & 0xFF); //address low byte
                ASSERT_EQ(transmitBuffer[idx++], evsysChannel + 1); //set EVOUTA to channel

                ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
                ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT3CTRLB & 0xFF); //address low byte
                ASSERT_EQ(transmitBuffer[idx++], 0x03); 
                ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                break;
        }
        ASSERT_EQ(transmitBuffer[idx++], testCase.inverted? 0x1 : 0xFE);

        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTMUX_CCLROUTEA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTMUX_CCLROUTEA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x08); //this is the alternate LUT3 output

        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT3CTRLA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT3CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x41); //turn on CCL LUT3

        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x41); //turn on CCL

        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenDeinitPassthroughToPin5_ThenLUT3DeconfiguredCorrectly)
    {
        if(testCase.pinIn == 5)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        if(testCase.commType == ATTiny427_ExpanderComm_SPI)
        {
            SCOPED_TRACE(testing::Message() << "UnsupportedPassthrough: " << testCase.commType);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 5, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        expanderService->Transmit(transmitBuffer);

        const bool usesEVSYS = *evsysService->EVSYS_CCL_LUT3A != 0;

        passthroughService->DeinitPassthrough(5);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        if(useAC)
        {
            ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_AC0_CTRLA >> 8); //address high byte
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_AC0_CTRLA & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0b10000111); //set CCL_CTRLA to 0 disabled
            ASSERT_EQ(size, idx);
            return;
        }
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x40); //set CCL_CTRLA to 0 disabled
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT3CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x00); //set CCL_CTRLA to 0 disabled
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CCL_LUT3A & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0); //set EVSYS to 0 disabled
        if(usesEVSYS)
        {
            ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high addr
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0); //generator set to 0 (disabled)
        }

        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenInitPassthroughToPin7_ThenEVOUTAIsConfiguredCorrectly)
    {
        if(testCase.pinIn == 7)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 7, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        if(testCase.commType == ATTiny427_ExpanderComm_SPIAlternate || 
            testCase.commType == ATTiny427_ExpanderComm_UART1Alternate)
        {
            SCOPED_TRACE(testing::Message() << "EVOUTA for Pin7 not on: " << testCase.commType);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 7, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 7, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTMUX_EVSYSROUTEA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTMUX_EVSYSROUTEA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x01); //this is the alternate EVOUTA mapping
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], evsysChannelGenerator); //generator set to pinIn
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_EVOUTA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], evsysChannel + 1); //set EVOUTA to channel
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTA_PINCTRL(7) >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTA_PINCTRL(7) & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], testCase.inverted ? 0x80 : 0x00); //inverted or not

        CheckACSetup(idx, 7);
        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenDeinitPassthroughToPin7_ThenEVOUTADeconfiguredCorrectly)
    {
        if(testCase.pinIn == 7)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        if(testCase.commType == ATTiny427_ExpanderComm_SPIAlternate || 
            testCase.commType == ATTiny427_ExpanderComm_UART1Alternate)
        {
            SCOPED_TRACE(testing::Message() << "EVOUTA for Pin7 not on: " << testCase.commType);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 7, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        expanderService->Transmit(transmitBuffer);

        passthroughService->DeinitPassthrough(7);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_EVOUTA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_EVOUTA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0); //set EVOUTA to 0 disabled
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high addr
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0); //generator set to 0 (disabled)
        if(testCase.inverted)
        {
            ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTA_PINCTRL(7) >> 8); //address high byte
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTA_PINCTRL(7) & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0x00); //remove inverted
        }

        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenInitPassthroughToPin7_ThenLUT1IsConfiguredCorrectly)
    {
        if(testCase.pinIn == 7)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 7, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        if(testCase.commType == ATTiny427_ExpanderComm_SPI || 
            testCase.commType == ATTiny427_ExpanderComm_UART0 ||
            testCase.commType == ATTiny427_ExpanderComm_UART0Alternate ||
            testCase.commType == ATTiny427_ExpanderComm_UART1)
        {
            SCOPED_TRACE(testing::Message() << "LUT1 for Pin7 not on: " << testCase.commType);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 7, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 7, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        if(useAC)
        {
            CheckACSetup(idx, 17);
            ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

            ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT1CTRLB & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0x66); 
            ASSERT_EQ(transmitBuffer[idx++], 0x00); 
        }
        else
        {
            switch(testCase.pinIn)
            {
                case 19: 
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT1CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x05); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    break;
                case 20:
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT1CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x50); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    break;
                case 21:
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT1CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x05); 
                    break;
                default:
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], evsysChannelGenerator); //generator set to pinIn
                    ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CCL_LUT1A & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], evsysChannel + 1); //set EVOUTA to channel

                    ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT1CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x03); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    break;
            }
        }
        ASSERT_EQ(transmitBuffer[idx++], testCase.inverted? 0x1 : 0xFE);

        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTMUX_CCLROUTEA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTMUX_CCLROUTEA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x00); //this is the primary LUT1 output

        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT1CTRLA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT1CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x41); //turn on CCL LUT1

        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x41); //turn on CCL

        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenDeinitPassthroughToPin7_ThenLUT1DeconfiguredCorrectly)
    {
        if(testCase.pinIn == 7)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        if(testCase.commType == ATTiny427_ExpanderComm_SPI || 
            testCase.commType == ATTiny427_ExpanderComm_UART0 ||
            testCase.commType == ATTiny427_ExpanderComm_UART0Alternate ||
            testCase.commType == ATTiny427_ExpanderComm_UART1)
        {
            SCOPED_TRACE(testing::Message() << "LUT1 for Pin7 not on: " << testCase.commType);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 7, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        expanderService->Transmit(transmitBuffer);

        const bool usesEVSYS = *evsysService->EVSYS_CCL_LUT1A != 0;

        passthroughService->DeinitPassthrough(7);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x40); //set CCL_CTRLA to 0 disabled
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT1CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x00); //set CCL_CTRLA to 0 disabled
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CCL_LUT1A & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0); //set EVSYS to 0 disabled
        if(usesEVSYS)
        {
            ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high addr
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0); //generator set to 0 (disabled)
        }

        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenInitPassthroughToPin10_ThenEVOUTBIsConfiguredCorrectly)
    {
        if(testCase.pinIn == 10)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 10, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 10, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 10, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTMUX_EVSYSROUTEA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTMUX_EVSYSROUTEA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x00); //this is the primary EVOUTB mapping
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], evsysChannelGenerator); //generator set to pinIn
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_EVOUTB & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], evsysChannel + 1); //set EVOUTB to channel
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTB_PINCTRL(2) >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTB_PINCTRL(2) & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], testCase.inverted ? 0x80 : 0x00); //inverted or not

        CheckACSetup(idx, 10);
        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenDeinitPassthroughToPin10_ThenEVOUTBDeconfiguredCorrectly)
    {
        if(testCase.pinIn == 10)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 10, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        expanderService->Transmit(transmitBuffer);

        passthroughService->DeinitPassthrough(10);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_EVOUTB >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_EVOUTB & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0); //set EVOUTB to 0 disabled
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high addr
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0); //generator set to 0 (disabled)
        if(testCase.inverted)
        {
            ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTB_PINCTRL(2) >> 8); //address high byte
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTB_PINCTRL(2) & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0x00); //remove inverted
        }

        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenInitPassthroughToPin11_ThenLUT2IsConfiguredCorrectly)
    {
        if(testCase.pinIn == 11)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 11, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 11, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 11, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        if(useAC)
        {
            CheckACSetup(idx, 11);
            ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

            ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT2CTRLB & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0x66); 
            ASSERT_EQ(transmitBuffer[idx++], 0x00); 
        }
        else
        {
            switch(testCase.pinIn)
            {
                case 8: 
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT2CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x05); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    break;
                case 9:
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT2CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x50); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    break;
                case 10:
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT2CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x05); 
                    break;
                default:
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], evsysChannelGenerator); //generator set to pinIn
                    ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CCL_LUT2A & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], evsysChannel + 1); //set EVOUTA to channel

                    ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT2CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x03); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    break;
            }
        }
        ASSERT_EQ(transmitBuffer[idx++], testCase.inverted? 0x1 : 0xFE);

        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTMUX_CCLROUTEA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTMUX_CCLROUTEA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x00); //this is the primary LUT2 output

        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT2CTRLA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT2CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x41); //turn on CCL LUT2

        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x41); //turn on CCL

        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenDeinitPassthroughToPin11_ThenLUT2DeconfiguredCorrectly)
    {
        if(testCase.pinIn == 11)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 11, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        expanderService->Transmit(transmitBuffer);

        const bool usesEVSYS = *evsysService->EVSYS_CCL_LUT2A != 0;

        passthroughService->DeinitPassthrough(11);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x40); //set CCL_CTRLA to 0 disabled
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT2CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x00); //set CCL_CTRLA to 0 disabled
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CCL_LUT2A & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0); //set EVSYS to 0 disabled
        if(usesEVSYS)
        {
            ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high addr
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0); //generator set to 0 (disabled)
        }

        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenInitPassthroughToPin12_ThenLUT0IsConfiguredCorrectly)
    {
        if(testCase.pinIn == 12)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 12, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 12, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 12, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        if(useAC)
        {
            CheckACSetup(idx, 12);
            ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

            ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT0CTRLB & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0x66); 
            ASSERT_EQ(transmitBuffer[idx++], 0x00); 
        }
        else
        {
            switch(testCase.pinIn)
            {
                case 0: 
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT0CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x05); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    break;
                case 1:
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT0CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x50); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    break;
                case 2:
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT0CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x05); 
                    break;
                default:
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], evsysChannelGenerator); //generator set to pinIn
                    ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CCL_LUT0A & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], evsysChannel + 1); //set EVOUTA to channel

                    ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT0CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x03); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    break;
            }
        }
        ASSERT_EQ(transmitBuffer[idx++], testCase.inverted? 0x1 : 0xFE);

        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTMUX_CCLROUTEA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTMUX_CCLROUTEA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x01); //this is the alternate LUT0 output

        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT0CTRLA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT0CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x41); //turn on CCL LUT0

        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x41); //turn on CCL

        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenDeinitPassthroughToPin12_ThenLUT0DeconfiguredCorrectly)
    {
        if(testCase.pinIn == 12)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 12, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        expanderService->Transmit(transmitBuffer);

        const bool usesEVSYS = *evsysService->EVSYS_CCL_LUT0A != 0;

        passthroughService->DeinitPassthrough(12);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x40); //set CCL_CTRLA to 0 disabled
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT0CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x00); //set CCL_CTRLA to 0 disabled
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CCL_LUT0A & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0); //set EVSYS to 0 disabled
        if(usesEVSYS)
        {
            ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high addr
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0); //generator set to 0 (disabled)
        }

        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenInitPassthroughToPin14_ThenLUT2IsConfiguredCorrectly)
    {
        if(testCase.pinIn == 14)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 14, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 14, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 14, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        if(useAC)
        {
            CheckACSetup(idx, 14);
            ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

            ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT2CTRLB & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0x66); 
            ASSERT_EQ(transmitBuffer[idx++], 0x00); 
        }
        else
        {
            switch(testCase.pinIn)
            {
                case 8: 
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT2CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x05); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    break;
                case 9:
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT2CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x50); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    break;
                case 10:
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT2CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x05); 
                    break;
                default:
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], evsysChannelGenerator); //generator set to pinIn
                    ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CCL_LUT2A & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], evsysChannel + 1); //set EVOUTA to channel

                    ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT2CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x03); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    break;
            }
        }
        ASSERT_EQ(transmitBuffer[idx++], testCase.inverted? 0x1 : 0xFE);

        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTMUX_CCLROUTEA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTMUX_CCLROUTEA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x04); //this is the alternate LUT2 output

        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT2CTRLA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT2CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x41); //turn on CCL LUT2

        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x41); //turn on CCL

        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenDeinitPassthroughToPin14_ThenLUT2DeconfiguredCorrectly)
    {
        if(testCase.pinIn == 14)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 14, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        expanderService->Transmit(transmitBuffer);

        const bool usesEVSYS = *evsysService->EVSYS_CCL_LUT2A != 0;

        passthroughService->DeinitPassthrough(14);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x40); //set CCL_CTRLA to 0 disabled
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT2CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x00); //set CCL_CTRLA to 0 disabled
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CCL_LUT2A & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0); //set EVSYS to 0 disabled
        if(usesEVSYS)
        {
            ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high addr
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0); //generator set to 0 (disabled)
        }

        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenInitPassthroughToPin15_ThenEVOUTBIsConfiguredCorrectly)
    {
        if(testCase.pinIn == 15)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 15, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 15, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 15, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTMUX_EVSYSROUTEA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTMUX_EVSYSROUTEA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x02); //this is the alternate EVOUTB mapping
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], evsysChannelGenerator); //generator set to pinIn
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_EVOUTB & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], evsysChannel + 1); //set EVOUTB to channel
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTB_PINCTRL(7) >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTB_PINCTRL(7) & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], testCase.inverted ? 0x80 : 0x00); //inverted or not

        CheckACSetup(idx, 15);
        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenDeinitPassthroughToPin15_ThenEVOUTBDeconfiguredCorrectly)
    {
        if(testCase.pinIn == 15)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 15, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        expanderService->Transmit(transmitBuffer);

        passthroughService->DeinitPassthrough(15);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_EVOUTB >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_EVOUTB & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0); //set EVOUTB to 0 disabled
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high addr
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0); //generator set to 0 (disabled)
        if(testCase.inverted)
        {
            ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTB_PINCTRL(7) >> 8); //address high byte
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTB_PINCTRL(7) & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0x00); //remove inverted
        }

        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenInitPassthroughToPin17_ThenLUT1IsConfiguredCorrectly)
    {
        if(testCase.pinIn == 17)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 17, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        if(testCase.commType == ATTiny427_ExpanderComm_SPIAlternate || testCase.commType == ATTiny427_ExpanderComm_UART1Alternate)
        {
            SCOPED_TRACE(testing::Message() << "UnsupportedPassthrough: " << testCase.commType);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 17, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 17, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 17, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        if(useAC)
        {
            CheckACSetup(idx, 17);
            ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

            ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT1CTRLB & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0x66); 
            ASSERT_EQ(transmitBuffer[idx++], 0x00); 
        }
        else
        {
            switch(testCase.pinIn)
            {
                case 19: 
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT1CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x05); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    break;
                case 20:
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT1CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x50); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    break;
                case 21:
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT1CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x05); 
                    break;
                default:
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], evsysChannelGenerator); //generator set to pinIn
                    ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CCL_LUT1A & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], evsysChannel + 1); //set EVOUTA to channel

                    ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT1CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x03); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    break;
            }
        }
        ASSERT_EQ(transmitBuffer[idx++], testCase.inverted? 0x1 : 0xFE);

        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTMUX_CCLROUTEA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTMUX_CCLROUTEA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x02); //this is the alternate LUT1 output

        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT1CTRLA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT1CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x41); //turn on CCL LUT1

        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x41); //turn on CCL

        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenDeinitPassthroughToPin17_ThenLUT1DeconfiguredCorrectly)
    {
        if(testCase.pinIn == 17)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        if(testCase.commType == ATTiny427_ExpanderComm_SPIAlternate || testCase.commType == ATTiny427_ExpanderComm_UART1Alternate)
        {
            SCOPED_TRACE(testing::Message() << "UnsupportedPassthrough: " << testCase.commType);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 17, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        expanderService->Transmit(transmitBuffer);

        const bool usesEVSYS = *evsysService->EVSYS_CCL_LUT1A != 0;

        passthroughService->DeinitPassthrough(17);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x40); //set CCL_CTRLA to 0 disabled
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT1CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x00); //set CCL_CTRLA to 0 disabled
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CCL_LUT1A & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0); //set EVSYS to 0 disabled
        if(usesEVSYS)
        {
            ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high addr
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0); //generator set to 0 (disabled)
        }

        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenInitPassthroughToPin18_ThenEVOUTCIsConfiguredCorrectly)
    {
        if(testCase.pinIn == 18)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 18, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        if(testCase.commType == ATTiny427_ExpanderComm_SPIAlternate || 
            testCase.commType == ATTiny427_ExpanderComm_UART1Alternate)
        {
            SCOPED_TRACE(testing::Message() << "UnsupportedPassthrough: " << testCase.commType);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 18, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 18, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 18, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], evsysChannelGenerator); //generator set to pinIn
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_EVOUTC & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], evsysChannel + 1); //set EVOUTB to channel
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTC_PINCTRL(2) >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTC_PINCTRL(2) & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], testCase.inverted ? 0x80 : 0x00); //inverted or not

        CheckACSetup(idx, 18);
        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenDeinitPassthroughToPin18_ThenEVOUTCDeconfiguredCorrectly)
    {
        if(testCase.pinIn == 18)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        if(testCase.commType == ATTiny427_ExpanderComm_SPIAlternate || 
            testCase.commType == ATTiny427_ExpanderComm_UART1Alternate)
        {
            SCOPED_TRACE(testing::Message() << "UnsupportedPassthrough: " << testCase.commType);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 18, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        expanderService->Transmit(transmitBuffer);

        passthroughService->DeinitPassthrough(18);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_EVOUTC >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_EVOUTC & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0); //set EVOUTC to 0 disabled
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high addr
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0); //generator set to 0 (disabled)
        if(testCase.inverted)
        {
            ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTC_PINCTRL(2) >> 8); //address high byte
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTC_PINCTRL(2) & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0x00); //remove inverted
        }

        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenInitPassthroughToPin20_ThenLUT3IsConfiguredCorrectly)
    {
        if(testCase.pinIn == 20)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 20, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            const bool result = passthroughService->InitPassthrough(testCase.pinIn, 20, testCase.inverted, testCase.useAC);
            ASSERT_FALSE(result);

            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 20, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        if(useAC)
        {
            CheckACSetup(idx, 20);
            ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

            ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT3CTRLB & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0x66); 
            ASSERT_EQ(transmitBuffer[idx++], 0x00); 
        }
        else
        {
            switch(testCase.pinIn)
            {
                case 16: 
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT3CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x05); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    break;
                case 17:
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT3CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x50); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    break;
                case 18:
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT3CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x05); 
                    break;
                default:
                    ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) >> 8); //address high byte
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], evsysChannelGenerator); //generator set to pinIn
                    ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CCL_LUT3A & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], evsysChannel + 1); //set EVOUTA to channel

                    ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); //turn off CCL to configure

                    ASSERT_EQ(transmitBuffer[idx++], 0xA3); //write 1 byte with 16 bit address with existing high address
                    ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT3CTRLB & 0xFF); //address low byte
                    ASSERT_EQ(transmitBuffer[idx++], 0x03); 
                    ASSERT_EQ(transmitBuffer[idx++], 0x00); 
                    break;
            }
        }
        ASSERT_EQ(transmitBuffer[idx++], testCase.inverted? 0x1 : 0xFE);

        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTMUX_CCLROUTEA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_PORTMUX_CCLROUTEA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x00); //this is the primary LUT3 output

        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT3CTRLA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT3CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x41); //turn on CCL LUT3

        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x41); //turn on CCL

        ASSERT_EQ(size, idx);
    }

    TEST_P(ATTiny427_PassthroughServiceTest, WhenDeinitPassthroughToPin20_ThenLUT3DeconfiguredCorrectly)
    {
        if(testCase.pinIn == 20)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        if(testCase.pinIn > 21)
        {
            SCOPED_TRACE(testing::Message() << "InvalidPin: " << testCase.pinIn);
            return;
        }

        SCOPED_TRACE(testing::Message() << "SupportedPassthrough: " << testCase.commType);

        bool result = passthroughService->InitPassthrough(testCase.pinIn, 20, testCase.inverted, testCase.useAC);
        ASSERT_TRUE(result);

        expanderService->Transmit(transmitBuffer);

        const bool usesEVSYS = *evsysService->EVSYS_CCL_LUT3A != 0;

        passthroughService->DeinitPassthrough(20);

        const size_t size = expanderService->Transmit(transmitBuffer);
        uint8_t idx = 0;
        ASSERT_EQ(transmitBuffer[idx++], 0x81); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA >> 8); //address high byte
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x40); //set CCL_CTRLA to 0 disabled
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_CCL_LUT3CTRLA & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0x00); //set CCL_CTRLA to 0 disabled
        ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CCL_LUT3A & 0xFF); //address low byte
        ASSERT_EQ(transmitBuffer[idx++], 0); //set EVSYS to 0 disabled
        if(usesEVSYS)
        {
            ASSERT_EQ(transmitBuffer[idx++], 0xA1); //write 1 byte with 16 bit address with existing high addr
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_EVSYS_CHANNEL(evsysChannel) & 0xFF); //address low byte
            ASSERT_EQ(transmitBuffer[idx++], 0); //generator set to 0 (disabled)
        }

        ASSERT_EQ(size, idx);
    }

}