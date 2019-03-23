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

			void *config = malloc(sizeof(float) + sizeof(float) + sizeof(uint8_t));
			void *buildConfig = config;
			//analog static service id
			*((uint8_t *)buildConfig) = 1;
			buildConfig = (void *)(((uint8_t *)buildConfig) + 1);

			*analogConfig = 5123.2f;
			*(analogConfig + 1) = 2321.5;
			
			memcpy(buildConfig, analogConfig, sizeof(float) + sizeof(float));
			buildConfig = (void *)((uint8_t *)buildConfig + sizeof(float) + sizeof(float));

			uint32_t size = 0;
			_floatInputService = IFloatInputService::CreateFloatInputService(&_hardwareAbstractionCollection, config, size);
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
