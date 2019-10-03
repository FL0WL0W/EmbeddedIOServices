#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Operations/Operation_PulseWidthPinRead.h"
#include "Service/ServiceLocator.h"
#include "MockPwmService.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace HardwareAbstraction;
using namespace Service;
using namespace Operations;

namespace UnitTests
{
	class Operation_PulseWidthPinReadTests : public ::testing::Test 
	{
		protected:
		MockPwmService _pwmService;
		HardwareAbstractionCollection _hardwareAbstractionCollection;
		ServiceLocator *_serviceLocator;
		IOperation<ScalarVariable> *_operation;

		Operation_PulseWidthPinReadTests() 
		{
			_serviceLocator = new ServiceLocator();

			_hardwareAbstractionCollection.PwmService = &_pwmService;
			_serviceLocator->Register(HARDWARE_ABSTRACTION_COLLECTION_ID, &_hardwareAbstractionCollection);
			_serviceLocator->Register(PWM_SERVICE_ID, _hardwareAbstractionCollection.PwmService);

			unsigned int expectedSize = sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint16_t);
			void *config = malloc(expectedSize);
			void *buildConfig = config;

			//pulse width pin factory id
			*((uint16_t *)buildConfig) = 7;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			//pin 1
			*((uint16_t *)buildConfig) = 1;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			//minFrequency 2
			*((uint16_t *)buildConfig) = 2;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			EXPECT_CALL(_pwmService, InitPin(1, In, 2)).Times(1);
			unsigned int size = 0;
			Operation_PulseWidthPinRead::RegisterFactory();
			_operation = static_cast<IOperation<ScalarVariable> *>(IOperationBase::Create(_serviceLocator, config, size));
			EXPECT_EQ(expectedSize, size);
		}
	};

	TEST_F(Operation_PulseWidthPinReadTests, WhenGettingValue_ThenCorrectValueIsReturned)
	{
		PwmValue pwmValue = { 0.1f, 0.05f };
		EXPECT_CALL(_pwmService, ReadPin(1)).Times(1).WillOnce(Return(pwmValue));
		ASSERT_NEAR(0.05f, _operation->Execute().To<float>(), 0.001f);

		pwmValue = { 0.025f, 0.004f };
		EXPECT_CALL(_pwmService, ReadPin(1)).Times(1).WillOnce(Return(pwmValue));
		ASSERT_NEAR(0.004f, _operation->Execute().To<float>(), 0.001f);

		pwmValue = { 0.05f, 0.04f };
		EXPECT_CALL(_pwmService, ReadPin(1)).Times(1).WillOnce(Return(pwmValue));
		ASSERT_NEAR(0.04f, _operation->Execute().To<float>(), 0.001f);

		pwmValue = { 0.05333333333333f, 0.04f };
		EXPECT_CALL(_pwmService, ReadPin(1)).Times(1).WillOnce(Return(pwmValue));
		ASSERT_NEAR(0.04f, _operation->Execute().To<float>(), 0.001f);
	}
}
