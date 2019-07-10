#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "VariableBus/FloatVariableService/FloatVariableService_2AxisTableTranslation.h"
#include "Service/ServiceLocator.h"
#include "Service/HardwareAbstractionServiceBuilder.h"
#include "Service/VariableBusServiceBuilderRegister.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace HardwareAbstraction;
using namespace VariableBus;
using namespace Service;

namespace UnitTests
{
	class FloatVariableService_2AxisTableTranslationTest : public ::testing::Test 
	{
		protected:
		ServiceLocator *_serviceLocator;
		FloatVariableService_2AxisTableTranslationConfig<float> *_translationConfig;
		CallBackGroup *_tickCallbackGroup;
		float _xValue = 0;
		float _yValue = 0;
		float *_outputValuePointer = 0;
		unsigned int _size = 0;

		FloatVariableService_2AxisTableTranslationTest() 
		{			
			_serviceLocator = new ServiceLocator();
			_translationConfig = (FloatVariableService_2AxisTableTranslationConfig<float> *)malloc(sizeof(FloatVariableService_2AxisTableTranslationConfig<float>) + 4 * 40);
			
			_translationConfig->MinXValue = 0;
			_translationConfig->MaxXValue = 2.97f;
			_translationConfig->XResolution = 10;
			_translationConfig->MinYValue = 0;
			_translationConfig->MaxYValue = 3.3f;
			_translationConfig->YResolution = 4;
			_translationConfig->PostMultiplier = 1.0f;
			float * Table = (float *)(_translationConfig + 1);
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
			Table[10] = 0;
			Table[11] = 10;
			Table[12] = 20;
			Table[13] = 30;
			Table[14] = 40;
			Table[15] = 50;
			Table[16] = 60;
			Table[17] = 70;
			Table[18] = 80;
			Table[19] = 90;
			Table[20] = 10;
			Table[21] = 20;
			Table[22] = 30;
			Table[23] = 40;
			Table[24] = 50;
			Table[25] = 60;
			Table[26] = 70;
			Table[27] = 80;
			Table[28] = 90;
			Table[29] = 100;
			Table[30] = 20;
			Table[31] = 30;
			Table[32] = 40;
			Table[33] = 50;
			Table[34] = 60;
			Table[35] = 70;
			Table[36] = 80;
			Table[37] = 90;
			Table[38] = 100;
			Table[39] = 110;

			void *config = malloc(_translationConfig->Size() + 7);
			void *buildConfig = config;

			//number of translations 1
			*((uint8_t *)buildConfig) = 1;
			buildConfig = (void *)(((uint8_t *)buildConfig) + 1);
			//variableId 2
			*((uint16_t *)buildConfig) = 3;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);
			//xvariableId 1
			*((uint16_t *)buildConfig) = 1;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);
			//yvariableId 1
			*((uint16_t *)buildConfig) = 2;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			memcpy(buildConfig, _translationConfig, _translationConfig->Size());
			buildConfig = (void *)((uint8_t *)buildConfig + _translationConfig->Size());

			_tickCallbackGroup = new CallBackGroup();
			_serviceLocator->Register(TICK_CALL_BACK_GROUP, _tickCallbackGroup);
			_serviceLocator->Register(VARIABLE_BUS, 1, &_xValue);
			_serviceLocator->Register(VARIABLE_BUS, 2, &_yValue);
			FloatVariableService_2AxisTableTranslation<float>::BuildFloatVariableService_2AxisTableTranslation(_serviceLocator, config, _size);
			_outputValuePointer = _serviceLocator->LocateAndCast<float>(VARIABLE_BUS, 3);
		}
	};

	TEST_F(FloatVariableService_2AxisTableTranslationTest, ConfigsAreCorrect)
	{
		ASSERT_EQ(182, _translationConfig->Size());
		ASSERT_EQ(189, _size);
		ASSERT_EQ((float *)(_translationConfig + 1), _translationConfig->Table());
		ASSERT_EQ(-10, _translationConfig->Table()[0]);
	}

	TEST_F(FloatVariableService_2AxisTableTranslationTest, WhenGettingValueInTable_ThenCorrectValueIsReturned)
	{
		_xValue = 0.0f;
		_yValue = 0.0f;
		_tickCallbackGroup->Execute();
		ASSERT_NEAR(-10, *_outputValuePointer, 0.001f);

		_xValue = 0.99f;
		_tickCallbackGroup->Execute();
		ASSERT_NEAR(20, *_outputValuePointer, 0.001f);

		_xValue = 0.33f;
		_tickCallbackGroup->Execute();
		ASSERT_NEAR(0, *_outputValuePointer, 0.001f);

		_xValue = 0.28875f;
		_tickCallbackGroup->Execute();
		ASSERT_NEAR(-1.25f, *_outputValuePointer, 0.001f);
		
		_xValue = 0.0f;
		_yValue = 1.1f;
		_tickCallbackGroup->Execute();
		ASSERT_NEAR(0, *_outputValuePointer, 0.001f);

		_xValue = 0.99f;
		_tickCallbackGroup->Execute();
		ASSERT_NEAR(30, *_outputValuePointer, 0.001f);

		_xValue = 0.33f;
		_tickCallbackGroup->Execute();
		ASSERT_NEAR(10, *_outputValuePointer, 0.001f);

		_xValue = 0.28875f;
		_tickCallbackGroup->Execute();
		ASSERT_NEAR(8.75f, *_outputValuePointer, 0.001f);
	}

	TEST_F(FloatVariableService_2AxisTableTranslationTest, WhenGettingValueAboveMaxValue_ThenCorrectValueIsReturned)
	{
		_xValue = 100.0f;
		_yValue = 0.0f;
		_tickCallbackGroup->Execute();
		ASSERT_FLOAT_EQ(80, *_outputValuePointer);
	}

	TEST_F(FloatVariableService_2AxisTableTranslationTest, WhenGettingValueBelowMinValue_ThenCorrectValueIsReturned)
	{
		_xValue = -1.0f;
		_yValue = 0.0f;
		_tickCallbackGroup->Execute();
		ASSERT_FLOAT_EQ(-10, *_outputValuePointer);
	}
}
