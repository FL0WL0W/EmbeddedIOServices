#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Operations/Operation_2AxisTable.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace Operations;
using namespace Service;

namespace UnitTests
{
	class Operation_2AxisTableTest : public ::testing::Test 
	{
		protected:
		ServiceLocator *_serviceLocator;
		Operation_2AxisTableConfig *_config;
		IOperation<ScalarVariable, ScalarVariable, ScalarVariable> *_operation;
		unsigned int _size = 0;

		Operation_2AxisTableTest() 
		{			
			_serviceLocator = new ServiceLocator();
			_config = (Operation_2AxisTableConfig *)malloc(sizeof(Operation_2AxisTableConfig) + 4 * 40);
			
			_config->MinXValue = 0;
			_config->MaxXValue = 2.97f;
			_config->XResolution = 10;
			_config->MinYValue = 0;
			_config->MaxYValue = 3.3f;
			_config->YResolution = 4;
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

			void *config = malloc(_config->Size() + 2);
			void *buildConfig = config;

			//factory id 1
			*((uint16_t *)buildConfig) = 3;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			memcpy(buildConfig, _config, _config->Size());
			buildConfig = (void *)((uint8_t *)buildConfig + _config->Size());

			Operation_2AxisTable::RegisterFactory();
			_operation = static_cast<IOperation<ScalarVariable, ScalarVariable, ScalarVariable> *>(IOperationBase::Create(_serviceLocator, config, _size));
		}
	};

	TEST_F(Operation_2AxisTableTest, ConfigsAreCorrect)
	{
		ASSERT_EQ(182, _config->Size());
		ASSERT_EQ(184, _size);
		ASSERT_EQ((float *)(_config + 1), _config->Table());
		ASSERT_EQ(-10, ((float *)_config->Table())[0]);
	}

	TEST_F(Operation_2AxisTableTest, WhenGettingValueInTable_ThenCorrectValueIsReturned)
	{
		ASSERT_EQ(-10, ScalarVariableTo<int8_t>(_operation->Execute(ScalarVariableFrom(static_cast<uint8_t>(0)), ScalarVariableFrom(static_cast<int8_t>(0)))));
		ASSERT_EQ(20, ScalarVariableTo<uint8_t>(_operation->Execute(ScalarVariableFrom(static_cast<float>(0.99f)), ScalarVariableFrom(static_cast<uint16_t>(0)))));
		ASSERT_EQ(0, ScalarVariableTo<uint16_t>(_operation->Execute(ScalarVariableFrom(static_cast<double>(0.33f)), ScalarVariableFrom(static_cast<int16_t>(0)))));
		ASSERT_NEAR(-1.25f, ScalarVariableTo<float>(_operation->Execute(ScalarVariableFrom(static_cast<float>(0.28875f)), ScalarVariableFrom(static_cast<uint32_t>(0)))), 0.001f);
		ASSERT_EQ(0, ScalarVariableTo<int16_t>(_operation->Execute(ScalarVariableFrom(static_cast<int32_t>(0)), ScalarVariableFrom(static_cast<float>(1.1f)))));
		ASSERT_EQ(30, ScalarVariableTo<uint32_t>(_operation->Execute(ScalarVariableFrom(static_cast<float>(0.99f)), ScalarVariableFrom(static_cast<float>(1.1f)))));
		ASSERT_EQ(10, ScalarVariableTo<int32_t>(_operation->Execute(ScalarVariableFrom(static_cast<float>(0.33f)), ScalarVariableFrom(static_cast<float>(1.1f)))));
		ASSERT_NEAR(8.75f, ScalarVariableTo<float>(_operation->Execute(ScalarVariableFrom(static_cast<float>(0.28875f)), ScalarVariableFrom(static_cast<float>(1.1f)))), 0.001f);
	}

	TEST_F(Operation_2AxisTableTest, WhenGettingValueAboveMaxValue_ThenCorrectValueIsReturned)
	{
		ASSERT_EQ(80, ScalarVariableTo<uint64_t>(_operation->Execute(ScalarVariableFrom(static_cast<float>(100.0f)), ScalarVariableFrom(static_cast<uint64_t>(0)))));
	}

	TEST_F(Operation_2AxisTableTest, WhenGettingValueBelowMinValue_ThenCorrectValueIsReturned)
	{
		ASSERT_EQ(-10, ScalarVariableTo<int64_t>(_operation->Execute(ScalarVariableFrom(static_cast<float>(-1.0f)), ScalarVariableFrom(static_cast<int64_t>(0)))));
	}
}
