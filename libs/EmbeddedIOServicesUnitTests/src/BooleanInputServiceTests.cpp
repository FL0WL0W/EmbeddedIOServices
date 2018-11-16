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
		BooleanInputService *_booleanInputService0;
		BooleanInputService *_booleanInputService1;

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
			_booleanInputService0 = (BooleanInputService *)IBooleanInputService::CreateBooleanInputService(&_hardwareAbstractionCollection, config, &size);

			inputConfig->Pin = 2;
			inputConfig->Inverted = true;
			config = malloc(inputConfig->Size() + 1);
			*(unsigned char *)config = 2;
			memcpy(((unsigned char *)config + 1), inputConfig, inputConfig->Size());
			_booleanInputService1 = (BooleanInputService *)IBooleanInputService::CreateBooleanInputService(&_hardwareAbstractionCollection, config, &size);
		}

		~BooleanInputServiceTest() override 
		{
			free(_booleanInputService0);
			free(_booleanInputService1);
		}
	};

	TEST_F(BooleanInputServiceTest, BooleanInputService_WhenGettingValueThenCorrectValueIsReturned)
	{
		EXPECT_CALL(_digitalService, ReadPin(1)).Times(1).WillOnce(Return(true));
		_booleanInputService0->ReadValue();
		ASSERT_EQ(true, _booleanInputService0->Value);

		EXPECT_CALL(_digitalService, ReadPin(1)).Times(1).WillOnce(Return(false));
		_booleanInputService0->ReadValue();
		ASSERT_EQ(false, _booleanInputService0->Value);

		EXPECT_CALL(_digitalService, ReadPin(2)).Times(1).WillOnce(Return(true));
		_booleanInputService1->ReadValue();
		ASSERT_EQ(false, _booleanInputService1->Value);

		EXPECT_CALL(_digitalService, ReadPin(2)).Times(1).WillOnce(Return(false));
		_booleanInputService1->ReadValue();
		ASSERT_EQ(true, _booleanInputService1->Value);
	}
}