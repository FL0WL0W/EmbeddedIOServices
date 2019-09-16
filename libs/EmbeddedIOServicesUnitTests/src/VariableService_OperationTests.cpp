#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "VariableBus/VariableService_Operation.h"
#include "Service/ServiceLocator.h"
#include "MockAnalogService.h"
#include "Service/HardwareAbstractionServiceBuilder.h"
#include "Service/VariableBusServiceBuilderIds.h"
#include "Service/OperationBusServiceBuilderIds.h"
#include "Operations/Operation_Polynomial.h"
#include "Operations/Operation_DigitalPinWrite.h"
#include "MockDigitalService.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace HardwareAbstraction;
using namespace VariableBus;
using namespace Service;

namespace UnitTests
{
	class VariableService_OperationTests : public ::testing::Test 
	{
		protected:
		MockDigitalService _digitalService;
		ServiceLocator *_serviceLocator;
		CallBackGroup *_tickCallbackGroup;
		float *_inputValuePointer = 0;
		bool *_inputValuePointer2 = 0;
		float *_outputValuePointer = 0;

		VariableService_OperationTests() 
		{
			_serviceLocator = new ServiceLocator();
			
			unsigned int expectedSize = sizeof(uint8_t) + sizeof(uint16_t) * 4;
			void *config = malloc(expectedSize);
			void *buildConfig = config;

			//instanceId 1
			*((uint8_t *)buildConfig) = 1;
			buildConfig = (void *)(((uint8_t *)buildConfig) + 1);

			//factoryId 1
			*((uint16_t *)buildConfig) = 1;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			//output variableId 1
			*((uint16_t *)buildConfig) = 1;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			//operation id 1
			*((uint16_t *)buildConfig) = 1;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			//input variableId 2
			*((uint16_t *)buildConfig) = 2;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);
			
			unsigned int expectedSize2 = sizeof(uint8_t) + sizeof(uint16_t) * 3;
			void *config2 = malloc(expectedSize2);
			buildConfig = config2;

			//instanceId 1
			*((uint8_t *)buildConfig) = 1;
			buildConfig = (void *)(((uint8_t *)buildConfig) + 1);

			//factoryId 2
			*((uint16_t *)buildConfig) = 2;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			//operation id 1
			*((uint16_t *)buildConfig) = 2;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			//input variableId 3
			*((uint16_t *)buildConfig) = 3;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			unsigned int size = 0;
			unsigned int size2 = 0;
			_tickCallbackGroup = new CallBackGroup();
			_serviceLocator->Register(TICK_CALL_BACK_GROUP, _tickCallbackGroup);
			_inputValuePointer = (float *)malloc(sizeof(float));
			_serviceLocator->Register(BUILDER_VARIABLEBUS, 2, _inputValuePointer);
			_inputValuePointer2 = (bool *)malloc(sizeof(bool));
			*_inputValuePointer2 = 0;
			_serviceLocator->Register(BUILDER_VARIABLEBUS, 3, _inputValuePointer2);
			Operations::Operation_PolynomialConfig *operationPolynomialConfig = (Operations::Operation_PolynomialConfig *)malloc(sizeof(Operations::Operation_PolynomialConfig) + sizeof(float) * 2);
			operationPolynomialConfig->Degree = 1;
			operationPolynomialConfig->MinValue = -1000;
			operationPolynomialConfig->MaxValue = 1000;
			((float *)operationPolynomialConfig->A())[0] = 0;
			((float *)operationPolynomialConfig->A())[1] = 1;
			_serviceLocator->Register(BUILDER_OPERATIONBUS, 1, new Operations::Operation_Polynomial(operationPolynomialConfig));
			EXPECT_CALL(_digitalService, InitPin(1, Out)).Times(1);
			EXPECT_CALL(_digitalService, WritePin(1, false)).Times(1);
			_serviceLocator->Register(BUILDER_OPERATIONBUS, 2, new Operations::Operation_DigitalPinWrite(&_digitalService, 1, false, false));
			VariableService_Operation<float, float>::RegisterFactory(1);
			VariableService_Operation<void, bool>::RegisterFactory(2);
			IVariableService::Build(_serviceLocator, config, size);
			EXPECT_EQ(expectedSize, size);
			IVariableService::Build(_serviceLocator, config2, size2);
			EXPECT_EQ(expectedSize2, size2);
			_outputValuePointer = _serviceLocator->LocateAndCast<float>(BUILDER_VARIABLEBUS, 1);
		}
	};

	TEST_F(VariableService_OperationTests, WhenGettingValue_ThenCorrectValueIsReturned)
	{
		EXPECT_CALL(_digitalService, WritePin(1, true)).Times(1);
		*_inputValuePointer2 = true;
		*_inputValuePointer = 13.37f;
		_tickCallbackGroup->Execute();
		ASSERT_FLOAT_EQ(*_inputValuePointer, *_outputValuePointer);
		
		*_inputValuePointer = 12.34f;
		ASSERT_FLOAT_EQ(13.37f, *_outputValuePointer);
		EXPECT_CALL(_digitalService, WritePin(1, false)).Times(1);
		*_inputValuePointer2 = 0;
		_tickCallbackGroup->Execute();
		ASSERT_FLOAT_EQ(*_inputValuePointer, *_outputValuePointer);
	}
}
