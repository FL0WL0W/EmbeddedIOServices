#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "VariableBus/FloatVariableService/FloatVariableService_Input.h"
#include "Service/ServiceLocator.h"
#include "MockAnalogService.h"
#include "Service/HardwareAbstractionServiceBuilder.h"
#include "Service/VariableBusServiceBuilderRegister.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace HardwareAbstraction;
using namespace VariableBus;
using namespace Service;

namespace UnitTests
{
	class FloatVariableService_InputTest : public ::testing::Test 
	{
		protected:
		ServiceLocator *_serviceLocator;
		CallBackGroup *_tickCallbackGroup;
		float *_outputValuePointer = 0;

		FloatVariableService_InputTest() 
		{
			_serviceLocator = new ServiceLocator();
			
			void *config = malloc(sizeof(uint16_t) + sizeof(uint8_t) + sizeof(float));
			void *buildConfig = config;

			//variableId 1
			*((uint16_t *)buildConfig) = 1;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			//static id
			*((uint8_t *)buildConfig) = 1;
			buildConfig = (void *)(((uint8_t *)buildConfig) + 1);

			*((float *)buildConfig) = 13.37f;
			buildConfig = (void *)(((float *)buildConfig) + 1);

			unsigned int size = 0;
			_tickCallbackGroup = new CallBackGroup();
			_serviceLocator->Register(TICK_CALL_BACK_GROUP, _tickCallbackGroup);
			FloatVariableService_Input::BuildFloatVariableService_Input(_serviceLocator, config, size);
			_outputValuePointer = _serviceLocator->LocateAndCast<float>(VARIABLE_BUS, 1);
		}
	};

	TEST_F(FloatVariableService_InputTest, WhenGettingValue_ThenCorrectValueIsReturned)
	{
		_tickCallbackGroup->Execute();
		ASSERT_FLOAT_EQ(13.37f, *_outputValuePointer);
	}
}
