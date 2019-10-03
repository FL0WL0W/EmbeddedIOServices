#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Operations/Operation_DigitalPinWrite.h"
#include "Service/ServiceLocator.h"
#include "MockDigitalService.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace HardwareAbstraction;
using namespace Service;
using namespace Operations;

namespace UnitTests
{
	class Operation_DigitalPinWriteTests : public ::testing::Test 
	{
		protected:
		MockDigitalService _digitalService;
		HardwareAbstractionCollection _hardwareAbstractionCollection;
		ServiceLocator *_serviceLocator;
		IOperation<void, ScalarVariable> *_operationNormalOff;
		IOperation<void, ScalarVariable> *_operationNormalOn;
		IOperation<void, ScalarVariable> *_operationNormalOffHighZ;
		IOperation<void, ScalarVariable> *_operationNormalOnHighZ;

		Operation_DigitalPinWriteTests() 
		{
			_serviceLocator = new ServiceLocator();

			_hardwareAbstractionCollection.DigitalService = &_digitalService;
			_serviceLocator->Register(HARDWARE_ABSTRACTION_COLLECTION_ID, &_hardwareAbstractionCollection);
			_serviceLocator->Register(DIGITAL_SERVICE_ID, _hardwareAbstractionCollection.DigitalService);
			Operation_DigitalPinWrite::RegisterFactory();

			void *configNormalOff = malloc(sizeof(uint16_t) + sizeof(uint16_t) + sizeof(bool) + sizeof(bool));
			void *buildConfig = configNormalOff;

			//digital pin factory id
			*((uint16_t *)buildConfig) = 10;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			//pin 1
			*((uint16_t *)buildConfig) = 1;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			//NormalOn
			*((bool *)buildConfig) = false;
			buildConfig = (void *)(((bool *)buildConfig) + 1);

			//HighZ
			*((bool *)buildConfig) = false;
			buildConfig = (void *)(((bool *)buildConfig) + 1);
			
			unsigned int size = 0;
			EXPECT_CALL(_digitalService, InitPin(1, Out)).Times(1);
			EXPECT_CALL(_digitalService, WritePin(1, false)).Times(1);
			_operationNormalOff = static_cast<IOperation<void, ScalarVariable> *>(IOperationBase::Create(_serviceLocator, configNormalOff, size));

			void *configNormalOn = malloc(sizeof(uint16_t) + sizeof(uint16_t) + sizeof(bool) + sizeof(bool));
			buildConfig = configNormalOn;

			//digital pin factory id
			*((uint16_t *)buildConfig) = 10;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			//pin 2
			*((uint16_t *)buildConfig) = 2;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			//NormalOn
			*((bool *)buildConfig) = true;
			buildConfig = (void *)(((bool *)buildConfig) + 1);

			//HighZ
			*((bool *)buildConfig) = false;
			buildConfig = (void *)(((bool *)buildConfig) + 1);

			size = 0;
			EXPECT_CALL(_digitalService, InitPin(2, Out)).Times(1);
			EXPECT_CALL(_digitalService, WritePin(2, true)).Times(1);
			_operationNormalOn = static_cast<IOperation<void, ScalarVariable> *>(IOperationBase::Create(_serviceLocator, configNormalOn, size));

			void *configNormalOffHighZ = malloc(sizeof(uint16_t) + sizeof(uint16_t) + sizeof(bool) + sizeof(bool));
			buildConfig = configNormalOffHighZ;

			//digital pin factory id
			*((uint16_t *)buildConfig) = 10;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			//pin 2
			*((uint16_t *)buildConfig) = 3;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			//NormalOn
			*((bool *)buildConfig) = false;
			buildConfig = (void *)(((bool *)buildConfig) + 1);

			//HighZ
			*((bool *)buildConfig) = true;
			buildConfig = (void *)(((bool *)buildConfig) + 1);

			size = 0;
			EXPECT_CALL(_digitalService, InitPin(3, Out)).Times(1);
			EXPECT_CALL(_digitalService, WritePin(3, false)).Times(1);
			_operationNormalOffHighZ = static_cast<IOperation<void, ScalarVariable> *>(IOperationBase::Create(_serviceLocator, configNormalOffHighZ, size));

			void *configNormalOnHighZ = malloc(sizeof(uint16_t) + sizeof(uint16_t) + sizeof(bool) + sizeof(bool));
			buildConfig = configNormalOnHighZ;

			//digital pin factory id
			*((uint16_t *)buildConfig) = 10;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			//pin 2
			*((uint16_t *)buildConfig) = 4;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			//NormalOn
			*((bool *)buildConfig) = true;
			buildConfig = (void *)(((bool *)buildConfig) + 1);

			//HighZ
			*((bool *)buildConfig) = true;
			buildConfig = (void *)(((bool *)buildConfig) + 1);

			size = 0;
			EXPECT_CALL(_digitalService, InitPin(4, In)).Times(1);
			_operationNormalOnHighZ = static_cast<IOperation<void, ScalarVariable> *>(IOperationBase::Create(_serviceLocator, configNormalOnHighZ, size));
		}
	};
	
	TEST_F(Operation_DigitalPinWriteTests, WhenWritingTrueForNormalOff_ThenPinWrittenTrue)
	{
		EXPECT_CALL(_digitalService, WritePin(1, true)).Times(1);
		_operationNormalOff->Execute(ScalarVariable(true));
	}
	
	TEST_F(Operation_DigitalPinWriteTests, WhenWritingFalseForNormalOff_ThenPinWrittenFalse)
	{
		EXPECT_CALL(_digitalService, WritePin(1, false)).Times(1);
		_operationNormalOff->Execute(ScalarVariable(false));
	}
	
	TEST_F(Operation_DigitalPinWriteTests, WhenWritingTrueForNormalOn_ThenPinWrittenFalse)
	{
		EXPECT_CALL(_digitalService, WritePin(2, false)).Times(1);
		_operationNormalOn->Execute(ScalarVariable(true));
	}
	
	TEST_F(Operation_DigitalPinWriteTests, WhenWritingFalseForNormalOn_ThenPinWrittenTrue)
	{
		EXPECT_CALL(_digitalService, WritePin(2, true)).Times(1);
		_operationNormalOn->Execute(ScalarVariable(false));
	}
	
	TEST_F(Operation_DigitalPinWriteTests, WhenWritingTrueForNormalOffHighZ_ThenPinConfiguredAsInput)
	{
		EXPECT_CALL(_digitalService, InitPin(3, In)).Times(1);
		_operationNormalOffHighZ->Execute(ScalarVariable(true));
	}
	
	TEST_F(Operation_DigitalPinWriteTests, WhenWritingFalseForNormalOffHighZ_ThenPinWrittenFalse)
	{
		EXPECT_CALL(_digitalService, InitPin(3, Out)).Times(1);
		EXPECT_CALL(_digitalService, WritePin(3, false)).Times(1);
		_operationNormalOffHighZ->Execute(ScalarVariable(false));
	}
	
	TEST_F(Operation_DigitalPinWriteTests, WhenWritingTrueForNormalOnHighZ_ThenPinWrittenFalse)
	{
		EXPECT_CALL(_digitalService, InitPin(4, Out)).Times(1);
		EXPECT_CALL(_digitalService, WritePin(4, false)).Times(1);
		_operationNormalOnHighZ->Execute(ScalarVariable(true));
	}
	
	TEST_F(Operation_DigitalPinWriteTests, WhenWritingFalseForNormalOnHighZ_ThenPinConfiguredAsInput)
	{
		EXPECT_CALL(_digitalService, InitPin(4, In)).Times(1);
		_operationNormalOnHighZ->Execute(ScalarVariable(false));
	}
}
