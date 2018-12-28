#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "HardwareAbstraction/HardwareAbstractionCollection.h"
#include "IOServices/BooleanInputService/IBooleanInputService.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace HardwareAbstraction;
using namespace IOServices;

namespace UnitTests
{
	class BooleanInputService_StaticTest : public ::testing::Test 
	{
		protected:
		HardwareAbstractionCollection _hardwareAbstractionCollection;
		IBooleanInputService *_booleanInputServiceFalse;
		IBooleanInputService *_booleanInputServiceTrue;

		BooleanInputService_StaticTest() 
		{
			bool *inputConfig = (bool *)malloc(sizeof(bool));

			unsigned int size = 0;

			*inputConfig = false;
			void *config = malloc(sizeof(bool) + sizeof(unsigned char));
			*(unsigned char *)config = 1;
			memcpy(((unsigned char *)config + 1), inputConfig, sizeof(bool));
			_booleanInputServiceFalse = IBooleanInputService::CreateBooleanInputService(&_hardwareAbstractionCollection, config, &size);

			*inputConfig = true;
			config = malloc(sizeof(bool) + sizeof(unsigned char));
			*(unsigned char *)config = 1;
			memcpy(((unsigned char *)config + 1), inputConfig, sizeof(bool));
			_booleanInputServiceTrue = IBooleanInputService::CreateBooleanInputService(&_hardwareAbstractionCollection, config, &size);
		}

		~BooleanInputService_StaticTest() override 
		{
			free(_booleanInputServiceFalse);
			free(_booleanInputServiceTrue);
		}
	};

	TEST_F(BooleanInputService_StaticTest, WhenGettingStaticValueFalse_ThenFalseIsReturned)
	{
		_booleanInputServiceFalse->ReadValue();
		ASSERT_EQ(false, _booleanInputServiceFalse->Value);
	}

	TEST_F(BooleanInputService_StaticTest, WhenGettingStaticValueTrue_ThenTrueIsReturned)
	{
		_booleanInputServiceTrue->ReadValue();
		ASSERT_EQ(true, _booleanInputServiceTrue->Value);
	}
}