#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Operations/Operation_LookupTable.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace Operations;
using namespace Service;

namespace UnitTests
{
	class Operation_LookupTableTests : public ::testing::Test 
	{
		protected:
		ServiceLocator *_serviceLocator;
		Operation_LookupTableConfig *_config;
		IOperation<ScalarVariable, ScalarVariable> *_operation;
		unsigned int _size = 0;

		Operation_LookupTableTests() 
		{			
			_serviceLocator = new ServiceLocator();
			unsigned int expectedSize = sizeof(Operation_LookupTableConfig) + 4 * 11;
			_config = (Operation_LookupTableConfig *)malloc(expectedSize);
			
			_config->MinXValue = 0;
			_config->MaxXValue = 3.3f;
			_config->XResolution = 11;
			_config->TableType = ScalarVariableType::FLOAT;
			float * Table = (float *)(_config + 1);
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

			void *config = malloc(_config->Size() + 2);
			void *buildConfig = config;

			//factory id 2
			*((uint16_t *)buildConfig) = 2;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			memcpy(buildConfig, _config, _config->Size());
			buildConfig = (void *)((uint8_t *)buildConfig + _config->Size());

			Operation_LookupTable::RegisterFactory();
			_operation = static_cast<IOperation<ScalarVariable, ScalarVariable> *>(IOperationBase::Create(_serviceLocator, config, _size));
		}
	};

	TEST_F(Operation_LookupTableTests, ConfigsAreCorrect)
	{
		ASSERT_EQ(57, _config->Size());
		ASSERT_EQ(59, _size);
		ASSERT_EQ((float *)(_config + 1), _config->Table());
		ASSERT_EQ(-10, reinterpret_cast<const float*>(_config->Table())[0]);
	}

	TEST_F(Operation_LookupTableTests, WhenGettingValueInTable_ThenCorrectValueIsReturned)
	{
		ASSERT_FLOAT_EQ(-10, _operation->Execute(ScalarVariable(0.0f)).To<float>());

		ASSERT_FLOAT_EQ(20, _operation->Execute(ScalarVariable(0.99f)).To<float>());

		ASSERT_NEAR(0, _operation->Execute(ScalarVariable(0.33f)).To<float>(), 0.001f);

		ASSERT_NEAR(-1.25f, _operation->Execute(ScalarVariable(0.28875f)).To<float>(), 0.001f);
	}

	TEST_F(Operation_LookupTableTests, WhenGettingValueAboveMaxValue_ThenCorrectValueIsReturned)
	{
		ASSERT_FLOAT_EQ(90, _operation->Execute(ScalarVariable(100.0f)).To<float>());
	}

	TEST_F(Operation_LookupTableTests, WhenGettingValueBelowMinValue_ThenCorrectValueIsReturned)
	{
		ASSERT_FLOAT_EQ(-10, _operation->Execute(ScalarVariable(-1.0f)).To<float>());
	}
}
