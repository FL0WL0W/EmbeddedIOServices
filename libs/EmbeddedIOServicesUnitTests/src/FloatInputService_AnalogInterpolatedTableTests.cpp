#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "IOServices/FloatInputService/FloatInputService_AnalogInterpolatedTable.h"
#include "MockTimerService.h"
#include "MockAnalogService.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace HardwareAbstraction;
using namespace IOServices;

namespace UnitTests
{
	class FloatInputService_AnalogInterpolatedTableTest : public ::testing::Test 
	{
		protected:
		MockTimerService _timerService;
		MockAnalogService _analogService;
		HardwareAbstractionCollection _hardwareAbstractionCollection;
		IFloatInputService *_floatInputService;
		FloatInputService_AnalogInterpolatedTableConfig *_analogConfig;
		unsigned int _size = 0;

		FloatInputService_AnalogInterpolatedTableTest() 
		{
			_hardwareAbstractionCollection.TimerService = &_timerService;
			_hardwareAbstractionCollection.AnalogService = &_analogService;
			
			_analogConfig = (FloatInputService_AnalogInterpolatedTableConfig *)malloc(sizeof(FloatInputService_AnalogInterpolatedTableConfig) + 4 * 11);
			
			//adcPin
			// uint16_t AdcPin;
			// float MinInputValue;
			// float MaxInputValue;
			// uint16_t DotSampleRate;
			// uint8_t Resolution;
			// float *Table;
			_analogConfig->AdcPin = 1;
			_analogConfig->DotSampleRate = 500;
			_analogConfig->MinInputValue = 0;
			_analogConfig->MaxInputValue = 3.3f;
			_analogConfig->Resolution = 11;
			float * Table = (float *)(_analogConfig + 1);
			Table[0] = -10;
			Table[1] = 0;
			Table[2] = 10;
			Table[3] = 20;
			Table[4] = 30;
			Table[5] = 40;
			Table[6] = 50;
			Table[7] = 60;
			Table[8] = 70;
			Table[9] = 80;
			Table[10] = 90;

			void *config = malloc(_analogConfig->Size() + 1);
			void *buildConfig = config;

			//analog InterpolatedTable service id
			*((uint8_t *)buildConfig) = 4;
			buildConfig = (void *)(((uint8_t *)buildConfig) + 1);

			memcpy(buildConfig, _analogConfig, _analogConfig->Size());
			buildConfig = (void *)((uint8_t *)buildConfig + _analogConfig->Size());

			EXPECT_CALL(_timerService, GetTicksPerSecond())
				.WillRepeatedly(Return(5000));
			EXPECT_CALL(_analogService, InitPin(1)).Times(1);
			_floatInputService = IFloatInputService::CreateFloatInputService(&_hardwareAbstractionCollection, config, _size);
		}

		~FloatInputService_AnalogInterpolatedTableTest() override 
		{
			free(_floatInputService);
		}
	};

	TEST_F(FloatInputService_AnalogInterpolatedTableTest, ConfigsAreCorrect)
	{
		ASSERT_EQ(57, _analogConfig->Size());
		ASSERT_EQ(58, _size);
		ASSERT_EQ((float *)(_analogConfig + 1), _analogConfig->Table());
		ASSERT_EQ(-10, _analogConfig->Table()[0]);
	}

	TEST_F(FloatInputService_AnalogInterpolatedTableTest, WhenGettingValueInTable_ThenCorrectValueIsReturned)
	{
		EXPECT_CALL(_timerService, GetTick()).Times(1).WillOnce(Return(5));
		EXPECT_CALL(_analogService, ReadPin(1)).Times(1).WillOnce(Return(0.0f));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(-10, _floatInputService->Value);
		ASSERT_FLOAT_EQ(0, _floatInputService->ValueDot);

		EXPECT_CALL(_timerService, GetTick()).Times(2).WillRepeatedly(Return(10));
		EXPECT_CALL(_analogService, ReadPin(1)).Times(1).WillOnce(Return(0.99f));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(20, _floatInputService->Value);
		ASSERT_FLOAT_EQ(10000, _floatInputService->ValueDot);

		EXPECT_CALL(_timerService, GetTick()).Times(1).WillOnce(Return(15));
		EXPECT_CALL(_analogService, ReadPin(1)).Times(1).WillOnce(Return(0.33f));
		_floatInputService->ReadValue();
		ASSERT_NEAR(0, _floatInputService->Value, 0.001f);
		ASSERT_FLOAT_EQ(10000, _floatInputService->ValueDot);

		EXPECT_CALL(_timerService, GetTick()).Times(2).WillRepeatedly(Return(20));
		EXPECT_CALL(_analogService, ReadPin(1)).Times(1).WillOnce(Return(0.28875f));
		_floatInputService->ReadValue();
		ASSERT_NEAR(-1.25f, _floatInputService->Value, 0.001f);
		ASSERT_NEAR(-10625, _floatInputService->ValueDot, 0.001f);
	}

	TEST_F(FloatInputService_AnalogInterpolatedTableTest, WhenGettingValueAboveMaxValue_ThenCorrectValueIsReturned)
	{
		EXPECT_CALL(_timerService, GetTick()).Times(2).WillRepeatedly(Return(30));
		EXPECT_CALL(_analogService, ReadPin(1)).Times(1).WillOnce(Return(100.0f));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(90, _floatInputService->Value);
	}

	TEST_F(FloatInputService_AnalogInterpolatedTableTest, WhenGettingValueBelowMinValue_ThenCorrectValueIsReturned)
	{
		EXPECT_CALL(_timerService, GetTick()).Times(2).WillOnce(Return(30));
		EXPECT_CALL(_analogService, ReadPin(1)).Times(1).WillOnce(Return(-1.0f));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(-10, _floatInputService->Value);
	}
}
