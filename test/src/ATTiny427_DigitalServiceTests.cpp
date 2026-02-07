#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "DigitalService_ATTiny427Expander.h"
#include "ATTiny427_ExpanderService.h"
#include "PinDirection.h"
#include <cstring>
#include <memory>

using namespace testing;
using namespace EmbeddedIOServices;

namespace ATTiny427_DigitalServiceTests
{
    // Test parameters for comprehensive coverage
    struct TestCase
    {
        ATTiny427_ExpanderComm commType;
        digitalpin_t pin;
        PinDirection direction;
    };

    class ATTiny427_DigitalServiceTest : public ::testing::TestWithParam<TestCase>
    {
    protected:
        TestCase testCase;
        std::unique_ptr<ATTiny427_ExpanderService> expanderService;
        std::unique_ptr<DigitalService_ATTiny427Expander> digitalService;
        uint8_t transmitBuffer[256];

        void SetUp() override
        {
            testCase = GetParam();
            expanderService = std::make_unique<ATTiny427_ExpanderService>(testCase.commType);
            digitalService = std::make_unique<DigitalService_ATTiny427Expander>(expanderService.get());
            const size_t size = expanderService->Transmit(transmitBuffer);
            std::memset(transmitBuffer, 0, size); // Clear transmit buffer
            expanderService->Receive(transmitBuffer, size); // Clear transmit buffer
        }

        // Helper to check if pin number is in valid range (0-23 for ATTiny427)
        bool IsValidPin(digitalpin_t pin)
        {
            return pin <= 23;
        }
    };

    // Create test cases for all valid pins with different communication methods and directions
    std::list<TestCase> CreateTestCases()
    {
        std::list<TestCase> testCases;
        
        for(auto commType : {ATTiny427_ExpanderComm_UART0, ATTiny427_ExpanderComm_UART0Alternate, 
                             ATTiny427_ExpanderComm_UART1, ATTiny427_ExpanderComm_UART1Alternate, 
                             ATTiny427_ExpanderComm_SPI, ATTiny427_ExpanderComm_SPIAlternate})
        {
            for(digitalpin_t pin = 0; pin <= 23; pin++)
            {
                for(auto direction : {In, Out})
                {
                    testCases.push_back({commType, pin, direction});
                }
            }
        }
        return testCases;
    }

    INSTANTIATE_TEST_SUITE_P(
        ATTiny427_DigitalServiceTest,
        ATTiny427_DigitalServiceTest,
        ::testing::ValuesIn(CreateTestCases())
    );

    TEST_P(ATTiny427_DigitalServiceTest, WhenInitPinInput_ThenDirectionConfiguredCorrectly)
    {
        SCOPED_TRACE(testing::Message() << "Pin: " << testCase.pin << ", CommType: " << testCase.commType);

        // Initialize pin as input
        digitalService->InitPin(testCase.pin, In);
        
        const size_t size = expanderService->Transmit(transmitBuffer);
        
        if(testCase.direction == In)
        {
            size_t idx = 6; // skip reads
            uint16_t address;
            switch(DigitalService_ATTiny427Expander::PinToDigitalPort(testCase.pin))
            {
                case PORTA: address = ADDRESS_VPORTA_DIR; break;
                case PORTB: address = ADDRESS_VPORTB_DIR; break;
                case PORTC: address = ADDRESS_VPORTC_DIR; break;
                default: FAIL() << "Invalid pin number";
            }
            ASSERT_EQ(transmitBuffer[idx++], 0xB1); //write 1 byte with high address of 0
            ASSERT_EQ(transmitBuffer[idx++], address); //low address
            ASSERT_EQ(transmitBuffer[idx++], 0x00); //data should be 0 for input
            ASSERT_EQ(size, idx) << "Expected transmit data for InitPin Input";
        }
    }

    TEST_P(ATTiny427_DigitalServiceTest, WhenInitPinOutput_ThenDirectionConfiguredCorrectly)
    {
        SCOPED_TRACE(testing::Message() << "Pin: " << testCase.pin << ", CommType: " << testCase.commType);

        // Initialize pin as output
        digitalService->InitPin(testCase.pin, Out);
        
        const size_t size = expanderService->Transmit(transmitBuffer);
        
        if(testCase.direction == Out)
        {
            size_t idx = 6; // skip reads
            uint16_t address;
            switch(DigitalService_ATTiny427Expander::PinToDigitalPort(testCase.pin))
            {
                case PORTA: address = ADDRESS_VPORTA_DIR; break;
                case PORTB: address = ADDRESS_VPORTB_DIR; break;
                case PORTC: address = ADDRESS_VPORTC_DIR; break;
                default: FAIL() << "Invalid pin number";
            }
            ASSERT_EQ(transmitBuffer[idx++], 0xB1); //write 1 byte with high address of 0
            ASSERT_EQ(transmitBuffer[idx++], address); //low address
            ASSERT_EQ(transmitBuffer[idx++], DigitalService_ATTiny427Expander::PinToDigitalPin(testCase.pin)); //data should be 1 for output
            ASSERT_EQ(size, idx) << "Expected transmit data for InitPin Output";
        }
    }

    TEST_P(ATTiny427_DigitalServiceTest, WhenReadPinBeforeInit_ThenReturnsZero)
    {
        SCOPED_TRACE(testing::Message() << "Pin: " << testCase.pin << ", CommType: " << testCase.commType);

        // Don't initialize, just read
        bool value = digitalService->ReadPin(testCase.pin);
        
        ASSERT_FALSE(value) << "Reading uninitialized pin should return false";
    }

    TEST_P(ATTiny427_DigitalServiceTest, WhenWritePin_ThenRegisterWrittenCorrectly)
    {
        SCOPED_TRACE(testing::Message() << "Pin: " << testCase.pin << ", CommType: " << testCase.commType);

        // Initialize pin as output
        digitalService->InitPin(testCase.pin, Out);
        expanderService->Transmit(transmitBuffer);

        // Write to the pin
        digitalService->WritePin(testCase.pin, true);
        const size_t size = expanderService->Transmit(transmitBuffer);
        
        // Should transmit a write command
        size_t idx = 6; // skip reads
        uint16_t address;
        switch(DigitalService_ATTiny427Expander::PinToDigitalPort(testCase.pin))
        {
            case PORTA: address = ADDRESS_VPORTA_OUT; break;
            case PORTB: address = ADDRESS_VPORTB_OUT; break;
            case PORTC: address = ADDRESS_VPORTC_OUT; break;
            default: FAIL() << "Invalid pin number";
        }
        ASSERT_EQ(transmitBuffer[idx++], 0xB1); //write 1 byte with high address of 0
        ASSERT_EQ(transmitBuffer[idx++], address); //low address
        ASSERT_EQ(transmitBuffer[idx++], DigitalService_ATTiny427Expander::PinToDigitalPin(testCase.pin)); //data should be 1 to set pin to true
        ASSERT_EQ(size, idx) << "Expected transmit data for InitPin Output";
    }

    TEST_P(ATTiny427_DigitalServiceTest, WhenWritePinMultipleTimes_ThenAllValuesTransmitted)
    {
        SCOPED_TRACE(testing::Message() << "Pin: " << testCase.pin << ", CommType: " << testCase.commType);

        // Initialize pin as output
        digitalService->InitPin(testCase.pin, Out);
        expanderService->Transmit(transmitBuffer);

        // Write multiple times
        digitalService->WritePin(testCase.pin, true);
        
        digitalService->WritePin(testCase.pin, false);
        
        digitalService->WritePin(testCase.pin, true);
        size_t size = expanderService->Transmit(transmitBuffer);

        // Should transmit a write command
        size_t idx = 6; // skip reads
        uint16_t address;
        switch(DigitalService_ATTiny427Expander::PinToDigitalPort(testCase.pin))
        {
            case PORTA: address = ADDRESS_VPORTA_OUT; break;
            case PORTB: address = ADDRESS_VPORTB_OUT; break;
            case PORTC: address = ADDRESS_VPORTC_OUT; break;
            default: FAIL() << "Invalid pin number";
        }
        ASSERT_EQ(transmitBuffer[idx++], 0xB1); //write 1 byte with high address of 0
        ASSERT_EQ(transmitBuffer[idx++], address); //low address
        ASSERT_EQ(transmitBuffer[idx++], DigitalService_ATTiny427Expander::PinToDigitalPin(testCase.pin)); //data should be 1 to set pin to true
        ASSERT_EQ(transmitBuffer[idx++], 0xB1); //write 1 byte with high address of 0
        ASSERT_EQ(transmitBuffer[idx++], address); //low address
        ASSERT_EQ(transmitBuffer[idx++], 0x00); //data should be 0 to reset pin to false 
        ASSERT_EQ(transmitBuffer[idx++], 0xB1); //write 1 byte with high address of 0
        ASSERT_EQ(transmitBuffer[idx++], address); //low address
        ASSERT_EQ(transmitBuffer[idx++], DigitalService_ATTiny427Expander::PinToDigitalPin(testCase.pin)); //data should be 1 to set pin to true
        ASSERT_EQ(size, idx) << "Expected transmit data for InitPin Output";

    }

    TEST_P(ATTiny427_DigitalServiceTest, WhenAttachInterrupt_ThenInterruptTriggeredOnPinChange)
    {
        SCOPED_TRACE(testing::Message() << "Pin: " << testCase.pin << ", CommType: " << testCase.commType);

        // Initialize pin as input
        digitalService->InitPin(testCase.pin, In);
        size_t size = expanderService->Transmit(transmitBuffer);
        memset(transmitBuffer, 0, size); // Clear transmit buffer
        expanderService->Receive(transmitBuffer, size); // return 0s

        // Attach callback
        bool callbackCalled = false;
        callback_t callback = [&callbackCalled]() { callbackCalled = true; };
        
        digitalService->AttachInterrupt(testCase.pin, callback);

        size = expanderService->Transmit(transmitBuffer);
        memset(transmitBuffer, 0, size); // Clear transmit buffer

        const size_t baseIdx = (testCase.commType == ATTiny427_ExpanderComm_SPI || testCase.commType == ATTiny427_ExpanderComm_SPIAlternate) ? 2 : 0; // SPI is offset 2

        switch(DigitalService_ATTiny427Expander::PinToDigitalPort(testCase.pin))
        {
            case PORTA: transmitBuffer[baseIdx + 2] = DigitalService_ATTiny427Expander::PinToDigitalPin(testCase.pin); break; //read with pin now set to 1
            case PORTB: transmitBuffer[baseIdx + 4] = DigitalService_ATTiny427Expander::PinToDigitalPin(testCase.pin); break; //read with pin now set to 1
            case PORTC: transmitBuffer[baseIdx + 6] = DigitalService_ATTiny427Expander::PinToDigitalPin(testCase.pin); break; //read with pin now set to 1
            default: FAIL() << "Invalid pin number"; 
        }

        expanderService->Receive(transmitBuffer, size);
        
        // Callback should not be called after detach
        ASSERT_TRUE(callbackCalled) << "Callback should be called after attach";
    }

    TEST_P(ATTiny427_DigitalServiceTest, WhenDetachInterrupt_ThenNoErrorOccurs)
    {
        SCOPED_TRACE(testing::Message() << "Pin: " << testCase.pin << ", CommType: " << testCase.commType);

        // Initialize pin as input
        digitalService->InitPin(testCase.pin, In);
        expanderService->Transmit(transmitBuffer);

        // Attach a callback
        callback_t dummyCallback = []() {};
        digitalService->AttachInterrupt(testCase.pin, dummyCallback);

        // Detach should not throw
        ASSERT_NO_THROW(digitalService->DetachInterrupt(testCase.pin));
    }

    TEST_P(ATTiny427_DigitalServiceTest, WhenAttachThenDetachInterrupt_ThenCallbackNotCalled)
    {
        SCOPED_TRACE(testing::Message() << "Pin: " << testCase.pin << ", CommType: " << testCase.commType);

        // Initialize pin as input
        digitalService->InitPin(testCase.pin, In);
        size_t size = expanderService->Transmit(transmitBuffer);
        memset(transmitBuffer, 0, size); // Clear transmit buffer
        expanderService->Receive(transmitBuffer, size); // return 0s

        // Attach callback
        bool callbackCalled = false;
        callback_t callback = [&callbackCalled]() { callbackCalled = true; };
        
        digitalService->AttachInterrupt(testCase.pin, callback);
        digitalService->DetachInterrupt(testCase.pin);

        size = expanderService->Transmit(transmitBuffer);
        memset(transmitBuffer, 0, size); // Clear transmit buffer

        const size_t baseIdx = (testCase.commType == ATTiny427_ExpanderComm_SPI || testCase.commType == ATTiny427_ExpanderComm_SPIAlternate) ? 2 : 0; // SPI is offset 2

        switch(DigitalService_ATTiny427Expander::PinToDigitalPort(testCase.pin))
        {
            case PORTA: transmitBuffer[baseIdx + 2] = DigitalService_ATTiny427Expander::PinToDigitalPin(testCase.pin); break; //read with pin now set to 1
            case PORTB: transmitBuffer[baseIdx + 4] = DigitalService_ATTiny427Expander::PinToDigitalPin(testCase.pin); break; //read with pin now set to 1
            case PORTC: transmitBuffer[baseIdx + 6] = DigitalService_ATTiny427Expander::PinToDigitalPin(testCase.pin); break; //read with pin now set to 1
            default: FAIL() << "Invalid pin number"; 
        }

        expanderService->Receive(transmitBuffer, size);
        
        // Callback should not be called after detach
        ASSERT_FALSE(callbackCalled) << "Callback should not be called after detach";
    }

    TEST_P(ATTiny427_DigitalServiceTest, WhenInitInvalidPin_ThenNoErrorOccurs)
    {
        SCOPED_TRACE(testing::Message() << "CommType: " << testCase.commType);

        // Initialize with invalid pin (0xFFFF is typically reserved)
        ASSERT_NO_THROW(digitalService->InitPin(0xFFFF, In));
        ASSERT_NO_THROW(digitalService->InitPin(0xFFFF, Out));
    }

    TEST_P(ATTiny427_DigitalServiceTest, WhenReadInvalidPin_ThenReturnsZero)
    {
        SCOPED_TRACE(testing::Message() << "CommType: " << testCase.commType);

        bool value = digitalService->ReadPin(0xFFFF);
        ASSERT_FALSE(value) << "Reading invalid pin should return false";
    }

    TEST_P(ATTiny427_DigitalServiceTest, WhenWriteInvalidPin_ThenNoErrorOccurs)
    {
        SCOPED_TRACE(testing::Message() << "CommType: " << testCase.commType);

        ASSERT_NO_THROW(digitalService->WritePin(0xFFFF, true));
        ASSERT_NO_THROW(digitalService->WritePin(0xFFFF, false));
    }
}
