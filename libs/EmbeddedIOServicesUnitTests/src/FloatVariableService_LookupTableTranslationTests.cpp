#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "VariableBus/FloatVariableService/FloatVariableService_LookupTableTranslation.h"
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
	class FloatVariableService_LookupTableTranslationTest : public ::testing::Test 
	{
		protected:
		ServiceLocator *_serviceLocator;
		FloatVariableService_LookupTableTranslationConfig<float> *_translationConfig;
		CallBackGroup *_tickCallbackGroup;
		float _inputValue = 0;
		float *_outputValuePointer = 0;
		unsigned int _size = 0;

		FloatVariableService_LookupTableTranslationTest() 
		{			
			_serviceLocator = new ServiceLocator();
			_translationConfig = (FloatVariableService_LookupTableTranslationConfig<float> *)malloc(sizeof(FloatVariableService_LookupTableTranslationConfig<float>) + 4 * 11);
			
			_translationConfig->MinXValue = 0;
			_translationConfig->MaxXValue = 3.3f;
			_translationConfig->XResolution = 11;
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
			Table[10] = 90;

			void *config = malloc(_translationConfig->Size() + 5);
			void *buildConfig = config;

			//number of translations 1
			*((uint8_t *)buildConfig) = 1;
			buildConfig = (void *)(((uint8_t *)buildConfig) + 1);
			//variableId 2
			*((uint16_t *)buildConfig) = 2;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);
			//xvariableId 1
			*((uint16_t *)buildConfig) = 1;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			memcpy(buildConfig, _translationConfig, _translationConfig->Size());
			buildConfig = (void *)((uint8_t *)buildConfig + _translationConfig->Size());

			_tickCallbackGroup = new CallBackGroup();
			_serviceLocator->Register(TICK_CALL_BACK_GROUP, _tickCallbackGroup);
			_serviceLocator->Register(VARIABLE_BUS, 1, &_inputValue);
			FloatVariableService_LookupTableTranslation<float>::BuildFloatVariableService_LookupTableTranslation(_serviceLocator, config, _size);
			_outputValuePointer = _serviceLocator->LocateAndCast<float>(VARIABLE_BUS, 2);
		}
	};

	TEST_F(FloatVariableService_LookupTableTranslationTest, ConfigsAreCorrect)
	{
		ASSERT_EQ(57, _translationConfig->Size());
		ASSERT_EQ(62, _size);
		ASSERT_EQ((float *)(_translationConfig + 1), _translationConfig->Table());
		ASSERT_EQ(-10, _translationConfig->Table()[0]);
	}

	TEST_F(FloatVariableService_LookupTableTranslationTest, WhenGettingValueInTable_ThenCorrectValueIsReturned)
	{
		_inputValue = 0.0f;
		_tickCallbackGroup->Execute();
		ASSERT_FLOAT_EQ(-10, *_outputValuePointer);

		_inputValue = 0.99f;
		_tickCallbackGroup->Execute();
		ASSERT_FLOAT_EQ(20, *_outputValuePointer);

		_inputValue = 0.33f;
		_tickCallbackGroup->Execute();
		ASSERT_NEAR(0, *_outputValuePointer, 0.001f);

		_inputValue = 0.28875f;
		_tickCallbackGroup->Execute();
		ASSERT_NEAR(-1.25f, *_outputValuePointer, 0.001f);
	}

	TEST_F(FloatVariableService_LookupTableTranslationTest, WhenGettingValueAboveMaxValue_ThenCorrectValueIsReturned)
	{
		_inputValue = 100.0f;
		_tickCallbackGroup->Execute();
		ASSERT_FLOAT_EQ(90, *_outputValuePointer);
	}

	TEST_F(FloatVariableService_LookupTableTranslationTest, WhenGettingValueBelowMinValue_ThenCorrectValueIsReturned)
	{
		_inputValue = -1.0f;
		_tickCallbackGroup->Execute();
		ASSERT_FLOAT_EQ(-10, *_outputValuePointer);
	}
}
