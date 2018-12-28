#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "MockDigitalService.h"
#include "HardwareAbstraction/HardwareAbstractionCollection.h"
#include "IOServices/BooleanOutputService/IBooleanOutputService.h"
#include "IOServices/BooleanOutputService/BooleanOutputService.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace HardwareAbstraction;
using namespace IOServices;

namespace UnitTests
{
	class BooleanOutputServiceTest : public ::testing::Test 
	{
		protected:
		MockDigitalService _digitalService;
		HardwareAbstractionCollection _hardwareAbstractionCollection;
		IBooleanOutputService *_booleanOutputService0;
		IBooleanOutputService *_booleanOutputService1;
		IBooleanOutputService *_booleanOutputService2;
		IBooleanOutputService *_booleanOutputService3;

		BooleanOutputServiceTest() 
		{
			_hardwareAbstractionCollection.DigitalService = &_digitalService;

			BooleanOutputServiceConfig *outputConfig = (BooleanOutputServiceConfig *)malloc(sizeof(BooleanOutputServiceConfig));

			outputConfig->Pin = 1;
			outputConfig->NormalOn = false;
			outputConfig->HighZ = false;

			void *config = malloc(outputConfig->Size() + 1);
			*(unsigned char *)config = 1;
			memcpy(((unsigned char *)config + 1), outputConfig, outputConfig->Size());

			unsigned int size = 0;
			_booleanOutputService0 = IBooleanOutputService::CreateBooleanOutputService(&_hardwareAbstractionCollection, config, &size);

			outputConfig->Pin = 3;
			outputConfig->HighZ = true;
			config = malloc(outputConfig->Size() + 1);
			*(unsigned char *)config = 1;
			memcpy(((unsigned char *)config + 1), outputConfig, outputConfig->Size());
			_booleanOutputService2 = IBooleanOutputService::CreateBooleanOutputService(&_hardwareAbstractionCollection, config, &size);

			outputConfig->Pin = 2;
			outputConfig->NormalOn = true;
			outputConfig->HighZ = false;
			config = malloc(outputConfig->Size() + 1);
			*(unsigned char *)config = 1;
			memcpy(((unsigned char *)config + 1), outputConfig, outputConfig->Size());
			_booleanOutputService1 = IBooleanOutputService::CreateBooleanOutputService(&_hardwareAbstractionCollection, config, &size);

			outputConfig->Pin = 4;
			outputConfig->HighZ = true;
			config = malloc(outputConfig->Size() + 1);
			*(unsigned char *)config = 1;
			memcpy(((unsigned char *)config + 1), outputConfig, outputConfig->Size());
			_booleanOutputService3 = IBooleanOutputService::CreateBooleanOutputService(&_hardwareAbstractionCollection, config, &size);
		}

		~BooleanOutputServiceTest() override 
		{
			free(_booleanOutputService0);
			free(_booleanOutputService1);
			free(_booleanOutputService2);
			free(_booleanOutputService3);
		}
	};

	TEST_F(BooleanOutputServiceTest, WhenSettingOutputForNormalOff_ThenPinWrittenTrue)
	{
		EXPECT_CALL(_digitalService, WritePin(1, true)).Times(1);
		_booleanOutputService0->OutputSet();
	}

	TEST_F(BooleanOutputServiceTest, WhenCallingOutputSetCallBackForNormalOff_ThenPinWrittenTrue)
	{
		EXPECT_CALL(_digitalService, WritePin(1, true)).Times(1);
		IBooleanOutputService::OutputSetCallBack(_booleanOutputService0);
	}

	TEST_F(BooleanOutputServiceTest, WhenWritingTrueForNormalOff_ThenPinWrittenTrue)
	{
		EXPECT_CALL(_digitalService, WritePin(1, true)).Times(1);
		_booleanOutputService0->OutputWrite(true);
	}

	TEST_F(BooleanOutputServiceTest, WhenResettingOutputForNormalOff_ThenPinWrittenFalse)
	{
		EXPECT_CALL(_digitalService, WritePin(1, false)).Times(1);
		_booleanOutputService0->OutputReset();
	}

	TEST_F(BooleanOutputServiceTest, WhenCallingOutputResetCallBackForNormalOff_ThenPinWrittenTrue)
	{
		EXPECT_CALL(_digitalService, WritePin(1, false)).Times(1);
		IBooleanOutputService::OutputResetCallBack(_booleanOutputService0);
	}

	TEST_F(BooleanOutputServiceTest, WhenWritingFalseForNormalOff_ThenPinWrittenTrue)
	{
		EXPECT_CALL(_digitalService, WritePin(1, false)).Times(1);
		_booleanOutputService0->OutputWrite(false);
	}

	TEST_F(BooleanOutputServiceTest, WhenSettingOutputForNormalOn_ThenPinWrittenFalse)
	{
		EXPECT_CALL(_digitalService, WritePin(2, false)).Times(1);
		_booleanOutputService1->OutputSet();
	}

	TEST_F(BooleanOutputServiceTest, WhenResettingOutputForNormalOn_ThenPintWrittenTrue)
	{
		EXPECT_CALL(_digitalService, WritePin(2, true)).Times(1);
		_booleanOutputService1->OutputReset();
	}

	TEST_F(BooleanOutputServiceTest, WhenSettingOutputForNormalOffHighZ_ThenPinSetAsInput)
	{
		EXPECT_CALL(_digitalService, InitPin(3, HardwareAbstraction::In)).Times(1);
		_booleanOutputService2->OutputSet();
	}

	TEST_F(BooleanOutputServiceTest, WhenResettingOutputForNormalOffHighZ_ThenPinSetAsOutputAndWrittenFalse)
	{
		EXPECT_CALL(_digitalService, InitPin(3, HardwareAbstraction::Out)).Times(1);
		EXPECT_CALL(_digitalService, WritePin(3, false)).Times(1);
		_booleanOutputService2->OutputReset();
	}

	TEST_F(BooleanOutputServiceTest, WhenSettingOutputForNormalOnHighZ_ThenPinSetAsOutputAndWrittenFalse)
	{
		EXPECT_CALL(_digitalService, InitPin(4, HardwareAbstraction::Out)).Times(1);
		EXPECT_CALL(_digitalService, WritePin(4, false)).Times(1);
		_booleanOutputService3->OutputSet();
	}

	TEST_F(BooleanOutputServiceTest, WhenResettingOutputForNormalOnHighZ_ThenPinSetAsInput)
	{
		EXPECT_CALL(_digitalService, InitPin(4, HardwareAbstraction::In)).Times(1);
		_booleanOutputService3->OutputReset();
	}
}