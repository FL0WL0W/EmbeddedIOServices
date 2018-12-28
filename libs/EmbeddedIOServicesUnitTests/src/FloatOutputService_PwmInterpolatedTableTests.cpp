#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "IOServices/FloatOutputService/FloatOutputService_PwmInterpolatedTable.h"
#include "MockTimerService.h"
#include "MockPwmService.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace HardwareAbstraction;
using namespace IOServices;

namespace UnitTests
{
	class FloatOutputService_PwmInterpolatedTableTests : public ::testing::Test 
	{
		protected:
		MockTimerService _timerService;
		MockPwmService _pwmService;
		HardwareAbstractionCollection _hardwareAbstractionCollection;
		IFloatOutputService *_floatOutputService;

		FloatOutputService_PwmInterpolatedTableTests() 
		{			
			_hardwareAbstractionCollection.TimerService = &_timerService;
			_hardwareAbstractionCollection.PwmService = &_pwmService;
			FloatOutputService_PwmInterpolatedTableConfig *pwmConfig = (FloatOutputService_PwmInterpolatedTableConfig *)malloc(sizeof(FloatOutputService_PwmInterpolatedTableConfig) + 4 * 11);
			
			pwmConfig->Frequency = 10000;
			pwmConfig->PwmPin = 1;
			pwmConfig->MinValue = 1;
			pwmConfig->MaxValue = 5;
			pwmConfig->Resolution = 11;
			pwmConfig->Table = (float *)(pwmConfig + 1);
			pwmConfig->Table[0] = 0.1;
			pwmConfig->Table[1] = 0.2;
			pwmConfig->Table[2] = 0.3;
			pwmConfig->Table[3] = 0.4;
			pwmConfig->Table[4] = 0.5;
			pwmConfig->Table[5] = 0.6;
			pwmConfig->Table[6] = 0.7;
			pwmConfig->Table[7] = 0.8;
			pwmConfig->Table[8] = 0.9;
			pwmConfig->Table[9] = 0.9;
			pwmConfig->Table[10] = 0.9;

			void *config = malloc(1000000);
			void *buildConfig = config;

			//pwm polynomial service id
			*((unsigned char *)buildConfig) = 3;
			buildConfig = (void *)(((unsigned char *)buildConfig) + 1);

			memcpy(buildConfig, pwmConfig, pwmConfig->Size());
			buildConfig = (void *)((unsigned char *)buildConfig + sizeof(pwmConfig->Size()));

			EXPECT_CALL(_timerService, GetTicksPerSecond())
				.WillRepeatedly(Return(5000));
			EXPECT_CALL(_pwmService, InitPin(1, Out, 10000)).Times(1);
			unsigned int size = 0;
			_floatOutputService = IFloatOutputService::CreateFloatOutputService(&_hardwareAbstractionCollection, config, &size);
		}

		~FloatOutputService_PwmInterpolatedTableTests() override 
		{
			free(_floatOutputService);
		}
	};

	TEST_F(FloatOutputService_PwmInterpolatedTableTests, WhenSettingValueWithinTable_ThenCorrectPinAndValueIsSet)
	{
		EXPECT_CALL(_pwmService, WritePin(1, PwmValue {0.0001, 0.00004})).Times(1);
		_floatOutputService->SetOutput(2.2);
	}

	TEST_F(FloatOutputService_PwmInterpolatedTableTests, WhenSettingValueAboveMaxValue_ThenCorrectPinAndValueIsSet)
	{
		EXPECT_CALL(_pwmService, WritePin(1, PwmValue {0.0001, 0.0000899999976})).Times(1);
		_floatOutputService->SetOutput(10);
	}

	TEST_F(FloatOutputService_PwmInterpolatedTableTests, WhenSettingValuBelowMinValue_ThenCorrectPinAndValueIsSet)
	{
		EXPECT_CALL(_pwmService, WritePin(1, PwmValue {0.0001, 0.00001})).Times(1);
		_floatOutputService->SetOutput(0);
	}
}