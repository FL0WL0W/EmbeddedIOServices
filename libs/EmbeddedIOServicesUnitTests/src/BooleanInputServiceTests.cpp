#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "MockDigitalService.h"
#include "HardwareAbstraction/HardwareAbstractionCollection.h"
#include "IOServices/BooleanInputService/BooleanInputService.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace HardwareAbstraction;
using namespace IOServices;

namespace UnitTests
{
	class BooleanInputServiceTest : public ::testing::Test 
	{
		protected:
		MockDigitalService _digitalService;
		HardwareAbstractionCollection _hardwareAbstractionCollection;
		IBooleanInputService *_booleanInputServiceUninverted;
		IBooleanInputService *_booleanInputServiceInverted;

		BooleanInputServiceTest() 
		{
			_hardwareAbstractionCollection.DigitalService = &_digitalService;

			BooleanInputServiceConfig *inputConfig = (BooleanInputServiceConfig *)malloc(sizeof(BooleanInputServiceConfig));

			inputConfig->Pin = 1;
			inputConfig->Inverted = false;

			void *config = malloc(inputConfig->Size() + 1);
			*(unsigned char *)config = 2;
			memcpy(((unsigned char *)config + 1), inputConfig, inputConfig->Size());

			unsigned int size = 0;
			_booleanInputServiceUninverted = IBooleanInputService::CreateBooleanInputService(&_hardwareAbstractionCollection, config, &size);

			inputConfig->Pin = 2;
			inputConfig->Inverted = true;
			config = malloc(inputConfig->Size() + 1);
			*(unsigned char *)config = 2;
			memcpy(((unsigned char *)config + 1), inputConfig, inputConfig->Size());
			_booleanInputServiceInverted = IBooleanInputService::CreateBooleanInputService(&_hardwareAbstractionCollection, config, &size);
		}

		~BooleanInputServiceTest() override 
		{
			free(_booleanInputServiceUninverted);
			free(_booleanInputServiceInverted);
		}
	};

	TEST_F(BooleanInputServiceTest, WhenGettingHighForUninverted_ThenTrueIsReturned)
	{
		EXPECT_CALL(_digitalService, ReadPin(1)).Times(1).WillOnce(Return(true));
		_booleanInputServiceUninverted->ReadValue();
		ASSERT_EQ(true, _booleanInputServiceUninverted->Value);
	}

	TEST_F(BooleanInputServiceTest, WhenGettingLowForUninverted_ThenFalseIsReturned)
	{
		EXPECT_CALL(_digitalService, ReadPin(1)).Times(1).WillOnce(Return(false));
		_booleanInputServiceUninverted->ReadValue();
		ASSERT_EQ(false, _booleanInputServiceUninverted->Value);
	}

	TEST_F(BooleanInputServiceTest, WhenGettingHighForInverted_ThenFalseIsReturned)
	{
		EXPECT_CALL(_digitalService, ReadPin(2)).Times(1).WillOnce(Return(true));
		_booleanInputServiceInverted->ReadValue();
		ASSERT_EQ(false, _booleanInputServiceInverted->Value);
	}

	TEST_F(BooleanInputServiceTest, WhenGettingLowForInverted_ThenTrueIsReturned)
	{
		EXPECT_CALL(_digitalService, ReadPin(2)).Times(1).WillOnce(Return(false));
		_booleanInputServiceInverted->ReadValue();
		ASSERT_EQ(true, _booleanInputServiceInverted->Value);
	}
}