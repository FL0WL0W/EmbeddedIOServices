#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "VariableBus/FloatVariableService/FloatVariableService_PolynomialTranslation.h"
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
	class FloatVariableService_PolynomialTranslationTest : public ::testing::Test 
	{
		protected:
		ServiceLocator *_serviceLocator;
		FloatVariableService_PolynomialTranslationConfig<4> *_translationConfig;
		CallBackGroup *_tickCallbackGroup;
		float _inputValue = 0;
		float *_outputValuePointer = 0;
		unsigned int _size = 0;

		FloatVariableService_PolynomialTranslationTest() 
		{			
			_serviceLocator = new ServiceLocator();
			_translationConfig = (FloatVariableService_PolynomialTranslationConfig<4> *)malloc(sizeof(FloatVariableService_PolynomialTranslationConfig<4>) + 4 * 11);
			
			_translationConfig->A[0] = -10;
			_translationConfig->A[1] = 10;
			_translationConfig->A[2] = 10;
			_translationConfig->A[3] = 10;
			_translationConfig->A[4] = 0;
			_translationConfig->MaxValue = 150;
			_translationConfig->MinValue = -40;

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
			FloatVariableService_PolynomialTranslation<4>::BuildFloatVariableService_PolynomialTranslation(_serviceLocator, config, _size);
			_outputValuePointer = _serviceLocator->LocateAndCast<float>(VARIABLE_BUS, 2);
		}
	};

	TEST_F(FloatVariableService_PolynomialTranslationTest, ConfigsAreCorrect)
	{
		ASSERT_EQ(28, _translationConfig->Size());
		ASSERT_EQ(33, _size);
	}

	TEST_F(FloatVariableService_PolynomialTranslationTest, WhenGettingValueWithinLimits_ThenCorrectValueIsReturned)
	{
		_inputValue = 0.0f;
		_tickCallbackGroup->Execute();
		ASSERT_FLOAT_EQ(-10, *_outputValuePointer);

		_inputValue = 1.0f;
		_tickCallbackGroup->Execute();
		ASSERT_FLOAT_EQ(20, *_outputValuePointer);

		_inputValue = 0.5f;
		_tickCallbackGroup->Execute();
		ASSERT_FLOAT_EQ(-1.25f, *_outputValuePointer);
	}

	TEST_F(FloatVariableService_PolynomialTranslationTest, WhenGettingValueAboveMaxValue_ThenCorrectValueIsReturned)
	{
		_inputValue = 100.0f;
		_tickCallbackGroup->Execute();
		ASSERT_FLOAT_EQ(150, *_outputValuePointer);
	}

	TEST_F(FloatVariableService_PolynomialTranslationTest, WhenGettingValueBelowMinValue_ThenCorrectValueIsReturned)
	{
		_inputValue = -100.0f;
		_tickCallbackGroup->Execute();
		ASSERT_FLOAT_EQ(-40, *_outputValuePointer);
	}
}
