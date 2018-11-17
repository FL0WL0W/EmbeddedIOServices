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
		IBooleanInputService *_booleanInputService0;
		IBooleanInputService *_booleanInputService1;

		BooleanInputService_StaticTest() 
		{
			bool *inputConfig = (bool *)malloc(sizeof(bool));

			unsigned int size = 0;

			*inputConfig = false;
			void *config = malloc(sizeof(bool) + sizeof(unsigned char));
			*(unsigned char *)config = 1;
			memcpy(((unsigned char *)config + 1), inputConfig, sizeof(bool));
			_booleanInputService0 = IBooleanInputService::CreateBooleanInputService(&_hardwareAbstractionCollection, config, &size);

			*inputConfig = true;
			config = malloc(sizeof(bool) + sizeof(unsigned char));
			*(unsigned char *)config = 1;
			memcpy(((unsigned char *)config + 1), inputConfig, sizeof(bool));
			_booleanInputService1 = IBooleanInputService::CreateBooleanInputService(&_hardwareAbstractionCollection, config, &size);
		}

		~BooleanInputService_StaticTest() override 
		{
			free(_booleanInputService0);
			free(_booleanInputService1);
		}
	};

	TEST_F(BooleanInputService_StaticTest, BooleanInputService_Static_WhenGettingValueThenCorrectValueIsReturned)
	{
		_booleanInputService0->ReadValue();
		ASSERT_EQ(false, _booleanInputService0->Value);

		_booleanInputService1->ReadValue();
		ASSERT_EQ(true, _booleanInputService1->Value);
	}
}