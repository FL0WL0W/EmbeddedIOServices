#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Operations/Operation_AnalogPinRead.h"
#include "Service/ServiceLocator.h"
#include "MockAnalogService.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace HardwareAbstraction;
using namespace Service;
using namespace Operations;

namespace UnitTests
{
	class Operation_AnalogPinReadTests : public ::testing::Test 
	{
		protected:
		MockAnalogService _analogService;
		HardwareAbstractionCollection _hardwareAbstractionCollection;
		ServiceLocator *_serviceLocator;
		IOperation<float> *_operation;

		Operation_AnalogPinReadTests() 
		{
			_serviceLocator = new ServiceLocator();

			_hardwareAbstractionCollection.AnalogService = &_analogService;
			_serviceLocator->Register(HARDWARE_ABSTRACTION_COLLECTION_ID, &_hardwareAbstractionCollection);
			_serviceLocator->Register(ANALOG_SERVICE_ID, _hardwareAbstractionCollection.AnalogService);

			unsigned int expectedSize = sizeof(uint16_t) + sizeof(uint16_t);
			void *config = malloc(expectedSize);
			void *buildConfig = config;

			//analog pin factory id
			*((uint16_t *)buildConfig) = 5;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			//pin 1
			*((uint16_t *)buildConfig) = 1;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			EXPECT_CALL(_analogService, InitPin(1)).Times(1);
			unsigned int size = 0;
			Operation_AnalogPinRead::RegisterFactory();
			_operation = static_cast<IOperation<float> *>(IOperationBase::Create(_serviceLocator, config, size));
			EXPECT_EQ(expectedSize, size);
		}
	};

	TEST_F(Operation_AnalogPinReadTests, WhenGettingValue_ThenCorrectValueIsReturned)
	{
		EXPECT_CALL(_analogService, ReadPin(1)).Times(1).WillOnce(Return(0.0f));
		ASSERT_FLOAT_EQ(0.0f, _operation->Execute());

		EXPECT_CALL(_analogService, ReadPin(1)).Times(1).WillOnce(Return(1.0f));
		ASSERT_FLOAT_EQ(1.0f, _operation->Execute());

		EXPECT_CALL(_analogService, ReadPin(1)).Times(1).WillOnce(Return(0.5f));
		ASSERT_FLOAT_EQ(0.5f, _operation->Execute());
	}
}
