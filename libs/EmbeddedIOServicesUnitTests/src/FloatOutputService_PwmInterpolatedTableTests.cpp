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
			float *Table = (float *)(pwmConfig + 1);
			Table[0] = 0.1f;
			Table[1] = 0.2f;
			Table[2] = 0.3f;
			Table[3] = 0.4f;
			Table[4] = 0.5f;
			Table[5] = 0.6f;
			Table[6] = 0.7f;
			Table[7] = 0.8f;
			Table[8] = 0.9f;
			Table[9] = 0.9f;
			Table[10] = 0.9f;

			void *config = malloc(1000000);
			void *buildConfig = config;

			//pwm polynomial service id
			*((uint8_t *)buildConfig) = 3;
			buildConfig = (void *)(((uint8_t *)buildConfig) + 1);

			memcpy(buildConfig, pwmConfig, pwmConfig->Size());
			buildConfig = (void *)((uint8_t *)buildConfig + sizeof(pwmConfig->Size()));

			EXPECT_CALL(_timerService, GetTicksPerSecond())
				.WillRepeatedly(Return(5000));
			EXPECT_CALL(_pwmService, InitPin(1, Out, 10000)).Times(1);
			uint32_t size = 0;
			_floatOutputService = IFloatOutputService::CreateFloatOutputService(&_hardwareAbstractionCollection, config, &size);
		}

		~FloatOutputService_PwmInterpolatedTableTests() override 
		{
			free(_floatOutputService);
		}
	};

	TEST_F(FloatOutputService_PwmInterpolatedTableTests, WhenSettingValueWithinTable_ThenCorrectPinAndValueIsSet)
	{
		EXPECT_CALL(_pwmService, WritePin(1, PwmValue {0.0001f, 0.00004f})).Times(1);
		_floatOutputService->SetOutput(2.2f);
	}

	TEST_F(FloatOutputService_PwmInterpolatedTableTests, WhenSettingValueAboveMaxValue_ThenCorrectPinAndValueIsSet)
	{
		EXPECT_CALL(_pwmService, WritePin(1, PwmValue {0.0001f, 0.0000899999976f})).Times(1);
		_floatOutputService->SetOutput(10);
	}

	TEST_F(FloatOutputService_PwmInterpolatedTableTests, WhenSettingValuBelowMinValue_ThenCorrectPinAndValueIsSet)
	{
		EXPECT_CALL(_pwmService, WritePin(1, PwmValue {0.0001f, 0.00001f})).Times(1);
		_floatOutputService->SetOutput(0);
	}
}
