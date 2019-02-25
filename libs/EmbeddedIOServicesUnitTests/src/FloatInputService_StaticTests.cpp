#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "IOServices/FloatInputService/IFloatInputService.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace HardwareAbstraction;
using namespace IOServices;

namespace UnitTests
{
	class FloatInputService_StaticTest : public ::testing::Test 
	{
		protected:
		HardwareAbstractionCollection _hardwareAbstractionCollection;
		IFloatInputService *_floatInputService;

		FloatInputService_StaticTest() 
		{
			float *analogConfig = (float *)malloc(sizeof(float) + sizeof(float));

			void *config = malloc(sizeof(float) + sizeof(float) + sizeof(unsigned char));
			void *buildConfig = config;
			//analog static service id
			*((unsigned char *)buildConfig) = 1;
			buildConfig = (void *)(((unsigned char *)buildConfig) + 1);

			*analogConfig = 5123.2f;
			*(analogConfig + 1) = 2321.5;
			
			memcpy(buildConfig, analogConfig, sizeof(float) + sizeof(float));
			buildConfig = (void *)((unsigned char *)buildConfig + sizeof(float) + sizeof(float));

			unsigned int size = 0;
			_floatInputService = IFloatInputService::CreateFloatInputService(&_hardwareAbstractionCollection, config, &size);
		}

		~FloatInputService_StaticTest() override 
		{
			free(_floatInputService);
		}
	};

	TEST_F(FloatInputService_StaticTest, WhenGettingStaticValue_ThenCorrectValueIsReturned)
	{
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(5123.2f, _floatInputService->Value);
		ASSERT_FLOAT_EQ(2321.5f, _floatInputService->ValueDot);
	}
}