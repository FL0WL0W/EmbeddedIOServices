#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Operations/Operation_DigitalPinRead.h"
#include "Service/ServiceLocator.h"
#include "MockDigitalService.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace HardwareAbstraction;
using namespace Service;
using namespace Operations;

namespace UnitTests
{
	class Operation_DigitalPinReadTests : public ::testing::Test 
	{
		protected:
		MockDigitalService _digitalService;
		HardwareAbstractionCollection _hardwareAbstractionCollection;
		ServiceLocator *_serviceLocator;
		IOperation<bool> *_operationInverted;
		IOperation<bool> *_operationUninverted;

		Operation_DigitalPinReadTests() 
		{
			_serviceLocator = new ServiceLocator();

			_hardwareAbstractionCollection.DigitalService = &_digitalService;
			_serviceLocator->Register(HARDWARE_ABSTRACTION_COLLECTION_ID, &_hardwareAbstractionCollection);
			_serviceLocator->Register(DIGITAL_SERVICE_ID, _hardwareAbstractionCollection.DigitalService);

			unsigned int expectedSizeUninverted = sizeof(uint16_t) + sizeof(uint16_t) + sizeof(bool);
			void *configUninverted = malloc(expectedSizeUninverted);
			void *buildConfig = configUninverted;

			//digital pin factory id
			*((uint16_t *)buildConfig) = 4;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			//pin 1
			*((uint16_t *)buildConfig) = 1;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			*((bool *)buildConfig) = false;
			buildConfig = (void *)(((bool *)buildConfig) + 1);
			
			unsigned int expectedSizeInverted = sizeof(uint16_t) + sizeof(uint16_t) + sizeof(bool);
			void *configInverted = malloc(expectedSizeInverted);
			buildConfig = configInverted;

			//digital pin factory id
			*((uint16_t *)buildConfig) = 4;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			//pin 2
			*((uint16_t *)buildConfig) = 2;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			*((bool *)buildConfig) = true;
			buildConfig = (void *)(((bool *)buildConfig) + 1);

			EXPECT_CALL(_digitalService, InitPin(1, In)).Times(1);
			EXPECT_CALL(_digitalService, InitPin(2, In)).Times(1);
			unsigned int sizeInverted = 0;
			unsigned int sizeUnInverted = 0;
			Operation_DigitalPinRead::RegisterFactory();
			_operationUninverted = static_cast<IOperation<bool> *>(IOperationBase::Create(_serviceLocator, configUninverted, sizeInverted));
			_operationInverted = static_cast<IOperation<bool> *>(IOperationBase::Create(_serviceLocator, configInverted, sizeUnInverted));
			EXPECT_EQ(expectedSizeUninverted, sizeUnInverted);
			EXPECT_EQ(expectedSizeInverted, sizeInverted);
		}
	};

	TEST_F(Operation_DigitalPinReadTests, WhenGettingHighForUninverted_ThenTrueIsReturned)
	{
		EXPECT_CALL(_digitalService, ReadPin(1)).Times(1).WillOnce(Return(true));
		ASSERT_EQ(true, _operationUninverted->Execute());
	}

	TEST_F(Operation_DigitalPinReadTests, WhenGettingLowForUninverted_ThenFalseIsReturned)
	{
		EXPECT_CALL(_digitalService, ReadPin(1)).Times(1).WillOnce(Return(false));
		ASSERT_EQ(false, _operationUninverted->Execute());
	}

	TEST_F(Operation_DigitalPinReadTests, WhenGettingHighForInverted_ThenFalseIsReturned)
	{
		EXPECT_CALL(_digitalService, ReadPin(2)).Times(1).WillOnce(Return(true));
		ASSERT_EQ(false, _operationInverted->Execute());
	}

	TEST_F(Operation_DigitalPinReadTests, WhenGettingLowForInverted_ThenTrueIsReturned)
	{
		EXPECT_CALL(_digitalService, ReadPin(2)).Times(1).WillOnce(Return(false));
		ASSERT_EQ(true, _operationInverted->Execute());
	}
}
