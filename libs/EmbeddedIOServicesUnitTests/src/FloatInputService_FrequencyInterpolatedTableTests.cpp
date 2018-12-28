#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "IOServices/FloatInputService/FloatInputService_FrequencyInterpolatedTable.h"
#include "MockTimerService.h"
#include "MockPwmService.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace HardwareAbstraction;
using namespace IOServices;

namespace UnitTests
{
	class FloatInputService_FrequencyInterpolatedTableTest : public ::testing::Test 
	{
		protected:
		MockTimerService _timerService;
		MockPwmService _pwmService;
		HardwareAbstractionCollection _hardwareAbstractionCollection;
		IFloatInputService *_floatInputService;

		FloatInputService_FrequencyInterpolatedTableTest() 
		{
			_hardwareAbstractionCollection.TimerService = &_timerService;
			_hardwareAbstractionCollection.PwmService = &_pwmService;
			
			FloatInputService_FrequencyInterpolatedTableConfig *analogConfig = (FloatInputService_FrequencyInterpolatedTableConfig *)malloc(sizeof(FloatInputService_FrequencyInterpolatedTableConfig) + 4 * 11);
			
			//adcPin
			// unsigned short AdcPin;
			// float MinInputValue;
			// float MaxInputValue;
			// unsigned short DotSampleRate;
			// unsigned char Resolution;
			// float *Table;
			analogConfig->PwmPin = 1;
			analogConfig->DotSampleRate = 500;
			analogConfig->MinFrequency = 10;
			analogConfig->MaxFrequency = 110;
			analogConfig->Resolution = 11;
			analogConfig->Table = (float *)(analogConfig + 1);
			analogConfig->Table[0] = -10;
			analogConfig->Table[1] = 0;
			analogConfig->Table[2] = 10;
			analogConfig->Table[3] = 20;
			analogConfig->Table[4] = 30;
			analogConfig->Table[5] = 40;
			analogConfig->Table[6] = 50;
			analogConfig->Table[7] = 60;
			analogConfig->Table[8] = 70;
			analogConfig->Table[9] = 80;
			analogConfig->Table[10] = 90;

			void *config = malloc(analogConfig->Size() + 1);
			void *buildConfig = config;

			//frequency InterpolatedTable service id
			*((unsigned char *)buildConfig) = 5;
			buildConfig = (void *)(((unsigned char *)buildConfig) + 1);

			memcpy(buildConfig, analogConfig, analogConfig->Size());
			buildConfig = (void *)((unsigned char *)buildConfig + analogConfig->Size());

			EXPECT_CALL(_timerService, GetTicksPerSecond())
				.WillRepeatedly(Return(5000));
			EXPECT_CALL(_pwmService, InitPin(1, In, analogConfig->MinFrequency)).Times(1);
			unsigned int size = 0;
			_floatInputService = IFloatInputService::CreateFloatInputService(&_hardwareAbstractionCollection, config, &size);
		}

		~FloatInputService_FrequencyInterpolatedTableTest() override 
		{
			free(_floatInputService);
		}
	};

	TEST_F(FloatInputService_FrequencyInterpolatedTableTest, WhenGettingValueInTable_ThenCorrectValueIsReturned)
	{
		PwmValue pwmValue = { 0.1f, 0.05f };
		EXPECT_CALL(_timerService, GetTick()).Times(1).WillOnce(Return(5));
		EXPECT_CALL(_pwmService, ReadPin(1)).Times(1).WillOnce(Return(pwmValue));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(-10.0f, _floatInputService->Value);
		ASSERT_FLOAT_EQ(0.0f, _floatInputService->ValueDot);

		pwmValue = { 0.025f, 0.004f };
		EXPECT_CALL(_timerService, GetTick()).Times(2).WillRepeatedly(Return(10));
		EXPECT_CALL(_pwmService, ReadPin(1)).Times(1).WillOnce(Return(pwmValue));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(20.0f, _floatInputService->Value);
		ASSERT_FLOAT_EQ(10000.0f, _floatInputService->ValueDot);

		pwmValue = { 0.05f, 0.04f };
		EXPECT_CALL(_timerService, GetTick()).Times(1).WillOnce(Return(15));
		EXPECT_CALL(_pwmService, ReadPin(1)).Times(1).WillOnce(Return(pwmValue));
		_floatInputService->ReadValue();
		ASSERT_NEAR(0.0f, _floatInputService->Value, 0.001f);
		ASSERT_FLOAT_EQ(10000.0f, _floatInputService->ValueDot);

		pwmValue = { 0.05333333333333f, 0.04f };
		EXPECT_CALL(_timerService, GetTick()).Times(2).WillRepeatedly(Return(20));
		EXPECT_CALL(_pwmService, ReadPin(1)).Times(1).WillOnce(Return(pwmValue));
		_floatInputService->ReadValue();
		ASSERT_NEAR(-1.25f, _floatInputService->Value, 0.001f);
		ASSERT_NEAR(-10625.0f, _floatInputService->ValueDot, 0.001f);
	}

	TEST_F(FloatInputService_FrequencyInterpolatedTableTest, WhenGettingValueAboveMaxValue_ThenCorrectValueIsReturned)
	{
		PwmValue pwmValue = { 0.0001f, 0.04f };
		EXPECT_CALL(_timerService, GetTick()).Times(2).WillRepeatedly(Return(30));
		EXPECT_CALL(_pwmService, ReadPin(1)).Times(1).WillOnce(Return(pwmValue));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(90, _floatInputService->Value);
	}

	TEST_F(FloatInputService_FrequencyInterpolatedTableTest, WhenGettingValueBelowMinValue_ThenCorrectValueIsReturned)
	{
		PwmValue pwmValue = { 1.0f, 0.04f };
		EXPECT_CALL(_timerService, GetTick()).Times(2).WillOnce(Return(30));
		EXPECT_CALL(_pwmService, ReadPin(1)).Times(1).WillOnce(Return(pwmValue));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(-10, _floatInputService->Value);
	}
}