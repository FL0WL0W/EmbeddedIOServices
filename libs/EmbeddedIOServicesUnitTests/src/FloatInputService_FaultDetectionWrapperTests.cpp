#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "IOServices/FloatInputService/FloatInputService_AnalogInterpolatedTable.h"
#include "IOServices/FloatInputService/FloatInputService_FaultDetectionWrapper.h"
#include "MockTimerService.h"
#include "MockAnalogService.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace HardwareAbstraction;
using namespace IOServices;

namespace UnitTests
{
	class FloatInputService_FaultDetectionWrapperTests : public ::testing::Test 
	{
		protected:
		MockTimerService _timerService;
		MockAnalogService _analogService;
		HardwareAbstractionCollection _hardwareAbstractionCollection;
		IFloatInputService *_floatInputService;

		FloatInputService_FaultDetectionWrapperTests() 
		{
			_hardwareAbstractionCollection.TimerService = &_timerService;
			_hardwareAbstractionCollection.AnalogService = &_analogService;
			
			FloatInputService_FaultDetectionWrapperConfig *faultConfig = (FloatInputService_FaultDetectionWrapperConfig *)malloc(sizeof(FloatInputService_FaultDetectionWrapperConfig));

			FloatInputService_AnalogInterpolatedTableConfig *analogConfig = (FloatInputService_AnalogInterpolatedTableConfig *)malloc(sizeof(FloatInputService_AnalogInterpolatedTableConfig) + 4 * 11);
			
			//adcPin
			// unsigned short AdcPin;
			// float MinInputValue;
			// float MaxInputValue;
			// unsigned short DotSampleRate;
			// unsigned char Resolution;
			// float *Table;
			analogConfig->AdcPin = 1;
			analogConfig->DotSampleRate = 500;
			analogConfig->MinInputValue = 0;
			analogConfig->MaxInputValue = 3.3;
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

			faultConfig->MinValue = -5;
			faultConfig->MaxValue = 85;
			faultConfig->DefaultValue = 50;

			void *config = malloc(faultConfig->Size() + analogConfig->Size() + 2);
			void *buildConfig = config;

			//fault detection service id
			*((unsigned char *)buildConfig) = 6;
			buildConfig = (void *)(((unsigned char *)buildConfig) + 1);

			memcpy(buildConfig, faultConfig, faultConfig->Size());
			buildConfig = (void *)((unsigned char *)buildConfig + faultConfig->Size());

			//analog InterpolatedTable service id
			*((unsigned char *)buildConfig) = 4;
			buildConfig = (void *)(((unsigned char *)buildConfig) + 1);

			memcpy(buildConfig, analogConfig, analogConfig->Size());
			buildConfig = (void *)((unsigned char *)buildConfig + analogConfig->Size());

			EXPECT_CALL(_timerService, GetTicksPerSecond())
				.WillRepeatedly(Return(5000));
			EXPECT_CALL(_analogService, InitPin(1)).Times(1);
			unsigned int size = 0;
			_floatInputService = IFloatInputService::CreateFloatInputService(&_hardwareAbstractionCollection, config, &size);
		}

		~FloatInputService_FaultDetectionWrapperTests() override 
		{
			free(_floatInputService);
		}
	};

	TEST_F(FloatInputService_FaultDetectionWrapperTests, FloatInputService_FaultDetectionWrapper_WhenGettingValueThenCorrectValueIsReturned)
	{
		EXPECT_CALL(_timerService, GetTick()).Times(1).WillOnce(Return(5));
		EXPECT_CALL(_analogService, ReadPin(1)).Times(1).WillOnce(Return(0));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(50.0f, _floatInputService->Value);
		ASSERT_FLOAT_EQ(0.0f, _floatInputService->ValueDot);

		EXPECT_CALL(_timerService, GetTick()).Times(2).WillRepeatedly(Return(10));
		EXPECT_CALL(_analogService, ReadPin(1)).Times(1).WillOnce(Return(0.99));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(20.0f, _floatInputService->Value);
		ASSERT_FLOAT_EQ(10000.0f, _floatInputService->ValueDot);

		EXPECT_CALL(_timerService, GetTick()).Times(1).WillOnce(Return(15));
		EXPECT_CALL(_analogService, ReadPin(1)).Times(1).WillOnce(Return(0.33));
		_floatInputService->ReadValue();
		ASSERT_NEAR(0.0f, _floatInputService->Value, 0.001f);
		ASSERT_FLOAT_EQ(10000.0f, _floatInputService->ValueDot);

		EXPECT_CALL(_timerService, GetTick()).Times(2).WillRepeatedly(Return(20));
		EXPECT_CALL(_analogService, ReadPin(1)).Times(1).WillOnce(Return(0.28875));
		_floatInputService->ReadValue();
		ASSERT_NEAR(-1.25f, _floatInputService->Value, 0.001f);
		ASSERT_NEAR(-10625.0f, _floatInputService->ValueDot, 0.001f);

		EXPECT_CALL(_timerService, GetTick()).Times(2).WillRepeatedly(Return(30));
		EXPECT_CALL(_analogService, ReadPin(1)).Times(1).WillOnce(Return(100));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(50.0, _floatInputService->Value);
		ASSERT_NEAR(0.0f, _floatInputService->ValueDot, 0.001f);

		EXPECT_CALL(_timerService, GetTick()).Times(1).WillOnce(Return(30));
		EXPECT_CALL(_analogService, ReadPin(1)).Times(1).WillOnce(Return(-1));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(50.0, _floatInputService->Value);
		ASSERT_NEAR(0.0f, _floatInputService->ValueDot, 0.001f);
	}
}