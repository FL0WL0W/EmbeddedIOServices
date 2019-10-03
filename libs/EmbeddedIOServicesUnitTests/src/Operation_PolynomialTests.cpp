#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Operations/Operation_Polynomial.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace Operations;
using namespace Service;

namespace UnitTests
{
	class Operation_PolynomialTest : public ::testing::Test 
	{
		protected:
		ServiceLocator *_serviceLocator;
		Operation_PolynomialConfig *_config;
		IOperation<ScalarVariable, ScalarVariable> *_operation;
		unsigned int _size = 0;

		Operation_PolynomialTest() 
		{			
			_serviceLocator = new ServiceLocator();
			unsigned int expectedSize = sizeof(Operation_PolynomialConfig) + 4 * 4;
			_config = (Operation_PolynomialConfig *)malloc(expectedSize);
			
			((float *)(_config + 1))[0] = -10;
			((float *)(_config + 1))[1] = 10;
			((float *)(_config + 1))[2] = 10;
			((float *)(_config + 1))[3] = 10;
			((float *)(_config + 1))[4] = 0;
			_config->MaxValue = 150;
			_config->MinValue = -40;
			_config->Degree = 4;

			void *config = malloc(_config->Size() + 2);
			void *buildConfig = config;

			//factory id 1
			*((uint16_t *)buildConfig) = 1;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			memcpy(buildConfig, _config, _config->Size());
			buildConfig = (void *)((uint8_t *)buildConfig + _config->Size());

			Operation_Polynomial::RegisterFactory();
			_operation = static_cast<IOperation<ScalarVariable, ScalarVariable> *>(IOperationBase::Create(_serviceLocator, config, _size));
		}
	};

	TEST_F(Operation_PolynomialTest, ConfigsAreCorrect)
	{
		ASSERT_EQ(29, _config->Size());
		ASSERT_EQ(31, _size);
	}

	TEST_F(Operation_PolynomialTest, WhenGettingValueWithinLimits_ThenCorrectValueIsReturned)
	{
		ASSERT_FLOAT_EQ(-10, _operation->Execute(ScalarVariable(0.0f)).To<float>());

		ASSERT_FLOAT_EQ(20, _operation->Execute(ScalarVariable(1.0f)).To<float>());

		ASSERT_FLOAT_EQ(-1.25f, _operation->Execute(ScalarVariable(0.5f)).To<float>());
	}

	TEST_F(Operation_PolynomialTest, WhenGettingValueAboveMaxValue_ThenCorrectValueIsReturned)
	{
		ASSERT_FLOAT_EQ(150, _operation->Execute(ScalarVariable(100.0f)).To<float>());
	}

	TEST_F(Operation_PolynomialTest, WhenGettingValueBelowMinValue_ThenCorrectValueIsReturned)
	{
		ASSERT_FLOAT_EQ(-40, _operation->Execute(ScalarVariable(-100.0f)).To<float>());
	}
}
