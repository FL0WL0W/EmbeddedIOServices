#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "AnalogService_ATTiny427Expander.h"
#include "ATTiny427_ExpanderService.h"
#include <cstring>
#include <memory>

using namespace testing;
using namespace EmbeddedIOServices;

namespace ATTiny427_AnalogServiceTests
{
    // Test parameters for comprehensive coverage
    struct TestCase
    {
        ATTiny427_ExpanderComm commType;
        analogpin_t pin;
        uint8_t accumulate;
    };

    class ATTiny427_AnalogServiceTest : public ::testing::TestWithParam<TestCase>
    {
    protected:
        TestCase testCase;
        std::unique_ptr<ATTiny427_ExpanderService> expanderService;
        std::unique_ptr<AnalogService_ATTiny427Expander> analogService;
        uint8_t transmitBuffer[256];

        void SetUp() override
        {
            testCase = GetParam();
            expanderService = std::make_unique<ATTiny427_ExpanderService>(testCase.commType);
            analogService = std::make_unique<AnalogService_ATTiny427Expander>(expanderService.get(), testCase.accumulate);
            expanderService->Receive(transmitBuffer, expanderService->Transmit(transmitBuffer)); // Clear transmit buffer
        }

        // Helper to get bit position from pin
        uint8_t GetAnalogChannelBit(analogpin_t pin)
        {
            uint8_t channel = AnalogService_ATTiny427Expander::PinToAnalogChannel(pin);
            if(channel == 0xFF || channel == 0)
                return 0xFF;
            return channel;
        }
    };

    // Create test cases for all valid pins with different communication methods and accumulate values
    std::list<TestCase> CreateTestCases()
    {
        std::list<TestCase> testCases;
        analogpin_t validPins[] = {1, 2, 3, 4, 5, 6, 7, 13, 12, 9, 8, 16, 17, 18, 19};
        
        for(auto commType : {ATTiny427_ExpanderComm_UART0, ATTiny427_ExpanderComm_UART0Alternate, 
                             ATTiny427_ExpanderComm_UART1, ATTiny427_ExpanderComm_UART1Alternate, 
                             ATTiny427_ExpanderComm_SPI, ATTiny427_ExpanderComm_SPIAlternate})
        {
            for(auto pin : validPins)
            {
                for(uint8_t accumulate : {0, 1, 5, 15})
                {
                    testCases.push_back({commType, pin, accumulate});
                }
            }
        }
        return testCases;
    }

    INSTANTIATE_TEST_SUITE_P(
        ATTiny427_AnalogServiceTest,
        ATTiny427_AnalogServiceTest,
        ::testing::ValuesIn(CreateTestCases())
    );

    TEST_P(ATTiny427_AnalogServiceTest, WhenInitPinFirstTime_ThenADCConfiguredCorrectly)
    {
        SCOPED_TRACE(testing::Message() << "Pin: " << testCase.pin << ", CommType: " << testCase.commType);

        // Initialize first analog pin
        analogService->InitPin(testCase.pin);
        
        const size_t size = expanderService->Transmit(transmitBuffer);
        ASSERT_GT(size, 0) << "Expected transmit data for InitPin";

        uint8_t channel = AnalogService_ATTiny427Expander::PinToAnalogChannel(testCase.pin);
        ASSERT_NE(channel, 0xFF) << "Pin should have valid channel";

        // Verify ADC is enabled with low latency
        size_t idx = channel < 8? 6 : 4;

        ASSERT_EQ(transmitBuffer[idx++], 0x83);
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_ADC0_CTRLA >> 8);
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_ADC0_CTRLA & 0xFF);
        ASSERT_EQ(transmitBuffer[idx++], 0b00100001) << "ADC0_CTRLA should be configured with enable and low latency";
        ASSERT_EQ(transmitBuffer[idx++], 0x01) << "ADC0_CTRLB should set prescaler DIV4";
        ASSERT_EQ(transmitBuffer[idx++], 0xA0) << "ADC0_CTRLC should set VDD reference and timebase";
        ASSERT_EQ(transmitBuffer[idx++], 0xA1);
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_ADC0_INTCTRL & 0xFF);
        ASSERT_EQ(transmitBuffer[idx++], 0x01) << "ADC0_INTCTRL should enable RESRDY interrupt";
        ASSERT_EQ(transmitBuffer[idx++], 0xA3);
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_ADC0_CTRLE & 0xFF);
        ASSERT_EQ(transmitBuffer[idx++], 0x05) << "ADC0_CTRLE should set SAMPDUR = 5";
        ASSERT_EQ(transmitBuffer[idx++], 0x00) << "ADC0_CTRLF should disable hardware accumulation";
        ASSERT_EQ(transmitBuffer[idx++], 0x11) << "ADC0_COMMAND should set single 12-bit mode and start";

        ASSERT_EQ(transmitBuffer[idx++], 0x03); // Poller read 3 command
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_ANALOG_VALUE_START >> 8);
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_ANALOG_VALUE_START & 0xFF);
        ASSERT_EQ(transmitBuffer[idx++], 0x00);
        ASSERT_EQ(transmitBuffer[idx++], 0x00);
        ASSERT_EQ(transmitBuffer[idx++], 0x00);

        if(testCase.commType == ATTiny427_ExpanderComm_SPI || testCase.commType == ATTiny427_ExpanderComm_SPIAlternate)
        {
            ASSERT_EQ(transmitBuffer[idx++], 0x00); // Dummy byte for SPI
            ASSERT_EQ(transmitBuffer[idx++], 0x00); // Dummy byte for SPI
        }

        ASSERT_EQ(idx, size);
    }

    TEST_P(ATTiny427_AnalogServiceTest, WhenInitPinFirstTime_ThenAnalogEnableSet)
    {
        SCOPED_TRACE(testing::Message() << "Pin: " << testCase.pin << ", CommType: " << testCase.commType);

        analogService->InitPin(testCase.pin);
        
        const size_t size = expanderService->Transmit(transmitBuffer);
        ASSERT_GT(size, 0);

        uint8_t channel = AnalogService_ATTiny427Expander::PinToAnalogChannel(testCase.pin);
        ASSERT_NE(channel, 0xFF);

        size_t idx = 0;
        if(channel < 8)
        {
            // Should update ANALOG_ENABLE_LOW
            ASSERT_EQ(transmitBuffer[idx++], 0xB1);
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_ANALOG_ENABLE_LOW);
            ASSERT_EQ(transmitBuffer[idx++], (1 << channel)) << "Analog enable low bit should be set for channel " << (int)channel;
            ASSERT_EQ(transmitBuffer[idx++], 0xB1);
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_ANALOG_ACCUMULATE);
            ASSERT_EQ(transmitBuffer[idx++], 0x80 | testCase.accumulate); //accumulate reset
        }
        else
        {
            // Should update ANALOG_ENABLE_HIGH
            ASSERT_EQ(transmitBuffer[idx++], 0xB2);
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_ANALOG_ENABLE_HIGH);
            ASSERT_EQ(transmitBuffer[idx++], (1 << (channel - 8))) << "Analog enable high bit should be set for channel " << (int)channel;
            ASSERT_EQ(transmitBuffer[idx++], 0x80 | testCase.accumulate); //accumulate reset
        }
        idx+=14; // Skip ADC configuration commands
        ASSERT_EQ(transmitBuffer[idx++], 0x03); // Poller read 3 command
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_ANALOG_VALUE_START >> 8);
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_ANALOG_VALUE_START & 0xFF);
        ASSERT_EQ(transmitBuffer[idx++], 0x00);
        ASSERT_EQ(transmitBuffer[idx++], 0x00);
        ASSERT_EQ(transmitBuffer[idx++], 0x00);

        if(testCase.commType == ATTiny427_ExpanderComm_SPI || testCase.commType == ATTiny427_ExpanderComm_SPIAlternate)
        {
            ASSERT_EQ(transmitBuffer[idx++], 0x00); // Dummy byte for SPI
            ASSERT_EQ(transmitBuffer[idx++], 0x00); // Dummy byte for SPI
        }

        ASSERT_EQ(idx, size);
    }

    TEST_P(ATTiny427_AnalogServiceTest, WhenInitPinAlreadyEnabled_ThenNothingTransmitted)
    {
        SCOPED_TRACE(testing::Message() << "Pin: " << testCase.pin << ", CommType: " << testCase.commType);

        // First initialization
        analogService->InitPin(testCase.pin);
        expanderService->Transmit(transmitBuffer);

        // Second initialization of same pin
        analogService->InitPin(testCase.pin);
        const size_t secondSize = expanderService->Transmit(transmitBuffer);
        
        // Should only transmit commands for receiving data, no new configuration
        size_t idx = 0;
        ASSERT_EQ(transmitBuffer[idx++], 0x03); // Poller read 3 command
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_ANALOG_VALUE_START >> 8);
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_ANALOG_VALUE_START & 0xFF);
        ASSERT_EQ(transmitBuffer[idx++], 0x00);
        ASSERT_EQ(transmitBuffer[idx++], 0x00);
        ASSERT_EQ(transmitBuffer[idx++], 0x00);

        if(testCase.commType == ATTiny427_ExpanderComm_SPI || testCase.commType == ATTiny427_ExpanderComm_SPIAlternate)
        {
            ASSERT_EQ(transmitBuffer[idx++], 0x00); // Dummy byte for SPI
            ASSERT_EQ(transmitBuffer[idx++], 0x00); // Dummy byte for SPI
        }
        ASSERT_EQ(secondSize, idx) << "Second InitPin should not transmit data";
    }

    TEST_P(ATTiny427_AnalogServiceTest, WhenInitPinInvalidPin_ThenNothingTransmitted)
    {
        SCOPED_TRACE(testing::Message() << "CommType: " << testCase.commType);

        analogpin_t invalidPins[] = {0, 10, 11, 14, 15, 20, 21, 22, 0xFFFF};
        
        for(auto invalidPin : invalidPins)
        {
            analogService->InitPin(invalidPin);
            const size_t size = expanderService->Transmit(transmitBuffer);
            ASSERT_EQ(size, 0) << "InitPin with invalid pin " << invalidPin << " should not transmit";
        }
    }

    TEST_P(ATTiny427_AnalogServiceTest, WhenInitMultiplePins_ThenAllChannelsConfigured)
    {
        SCOPED_TRACE(testing::Message() << "CommType: " << testCase.commType);

        analogpin_t pin1 = testCase.pin;
        analogpin_t pin2 = testCase.pin + 1;
        switch(pin2)
        {
            case 10:
                pin2 = 12; //skip invalid pins
                break;
            case 14:
                pin2 = 16; //skip invalid pins
                break;
            case 20:
                pin2 = 1; //wrap around
                break;
        }
        
        // Initialize first pin
        analogService->InitPin(pin1);
        expanderService->Transmit(transmitBuffer);

        // Initialize second pin
        analogService->InitPin(pin2);
        const size_t secondSize = expanderService->Transmit(transmitBuffer);
        
        ASSERT_GT(secondSize, 0) << "Second pin initialization should transmit data";

        uint8_t channel1 = AnalogService_ATTiny427Expander::PinToAnalogChannel(pin1);
        uint8_t channel2 = AnalogService_ATTiny427Expander::PinToAnalogChannel(pin2);
        
        // Both enable bits should be set
        uint16_t expectedEnable = (1 << channel1) | (1 << channel2);
        size_t idx = 0;
        if(channel2 < 8)
        {
            // Should update ANALOG_ENABLE_LOW
            ASSERT_EQ(transmitBuffer[idx++], 0xB1);
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_ANALOG_ENABLE_LOW);
            ASSERT_EQ(transmitBuffer[idx++], expectedEnable & 0xFF) << "Analog enable low bit should be set for channel " << (int)channel2;
            ASSERT_EQ(transmitBuffer[idx++], 0xB1);
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_ANALOG_ACCUMULATE);
            ASSERT_EQ(transmitBuffer[idx++], 0x80 | testCase.accumulate); //accumulate reset
        }
        else
        {
            // Should update ANALOG_ENABLE_HIGH
            ASSERT_EQ(transmitBuffer[idx++], 0xB2);
            ASSERT_EQ(transmitBuffer[idx++], ADDRESS_ANALOG_ENABLE_HIGH);
            ASSERT_EQ(transmitBuffer[idx++], expectedEnable >> 8) << "Analog enable high bit should be set for channel " << (int)channel2;
            ASSERT_EQ(transmitBuffer[idx++], 0x80 | testCase.accumulate); //accumulate reset
        }
        ASSERT_EQ(transmitBuffer[idx++], 0x05); //accumulate reset
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_ANALOG_VALUE_START >> 8);
        ASSERT_EQ(transmitBuffer[idx++], ADDRESS_ANALOG_VALUE_START & 0xFF);
        ASSERT_EQ(transmitBuffer[idx++], 0x00);
        ASSERT_EQ(transmitBuffer[idx++], 0x00);
        ASSERT_EQ(transmitBuffer[idx++], 0x00);
        ASSERT_EQ(transmitBuffer[idx++], 0x00);
        ASSERT_EQ(transmitBuffer[idx++], 0x00);
        if(testCase.commType == ATTiny427_ExpanderComm_SPI || testCase.commType == ATTiny427_ExpanderComm_SPIAlternate)
        {
            ASSERT_EQ(transmitBuffer[idx++], 0x00); // Dummy byte for SPI
            ASSERT_EQ(transmitBuffer[idx++], 0x00); // Dummy byte for SPI
        }
        ASSERT_EQ(idx, secondSize);
    }

    TEST_P(ATTiny427_AnalogServiceTest, WhenReadPinBeforeInit_ThenReturnsZero)
    {
        SCOPED_TRACE(testing::Message() << "Pin: " << testCase.pin << ", CommType: " << testCase.commType);

        // Don't initialize, just read
        float value = analogService->ReadPin(testCase.pin);
        
        ASSERT_EQ(value, 0.0f) << "Reading uninitialized pin should return 0";
    }

    TEST_P(ATTiny427_AnalogServiceTest, WhenReadInvalidPin_ThenReturnsZero)
    {
        SCOPED_TRACE(testing::Message() << "CommType: " << testCase.commType);

        analogpin_t invalidPins[] = {0, 10, 11, 14, 15, 20, 21, 22, 0xFFFF};
        
        for(auto invalidPin : invalidPins)
        {
            float value = analogService->ReadPin(invalidPin);
            ASSERT_EQ(value, 0.0f) << "Reading invalid pin " << invalidPin << " should return 0";
        }
    }

    TEST_P(ATTiny427_AnalogServiceTest, WhenReadPinAfterInit_ThenValueReadableAndCorrect)
    {
        SCOPED_TRACE(testing::Message() << "Pin: " << testCase.pin << ", CommType: " << testCase.commType);

        // Initialize pin
        analogService->InitPin(testCase.pin);
        expanderService->Transmit(transmitBuffer);

        uint8_t channel = AnalogService_ATTiny427Expander::PinToAnalogChannel(testCase.pin);
        ASSERT_NE(channel, 0xFF) << "Pin should have valid channel";
        size_t idx = channel < 8? 23 : 21;
        idx += (testCase.commType == ATTiny427_ExpanderComm_SPI || testCase.commType == ATTiny427_ExpanderComm_SPIAlternate)? 2 : 0;
        transmitBuffer[idx++] = 0x01; 
        const uint16_t analogValue = 2.5f / 5.0f * 4095 * (testCase.accumulate + 1); // Mid-scale value for 12-bit ADC
        transmitBuffer[idx++] = analogValue & 0xFF; 
        transmitBuffer[idx++] = analogValue >> 8; 
        expanderService->Receive(transmitBuffer,idx); // Simulate receiving data to set data valid

        // After init, reading should not crash and return a value
        float value = analogService->ReadPin(testCase.pin);
        
        ASSERT_NEAR(value, 2.5f, 5.0f / (4095.0f * (testCase.accumulate + 1))) << "Reading initialized pin should return correct voltage value";
    }
}
