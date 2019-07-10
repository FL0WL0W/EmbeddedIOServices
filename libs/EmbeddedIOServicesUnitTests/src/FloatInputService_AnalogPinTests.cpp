#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "IOServices/FloatInputService/IFloatInputService.h"
#include "MockAnalogService.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace HardwareAbstraction;
using namespace IOServices;

namespace UnitTests
{
	class FloatInputService_AnalogPinTest : public ::testing::Test 
	{
		protected:
		MockAnalogService _analogService;
		HardwareAbstractionCollection _hardwareAbstractionCollection;
		IFloatInputService *_floatInputService;

		FloatInputService_AnalogPinTest() 
		{
			_hardwareAbstractionCollection.AnalogService = &_analogService;

			void *config = malloc(sizeof(uint8_t) + sizeof(uint16_t));
			void *buildConfig = config;

			//analog pin service id
			*((uint8_t *)buildConfig) = 2;
			buildConfig = (void *)(((uint8_t *)buildConfig) + 1);

			//pin 1
			*((uint16_t *)buildConfig) = 1;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			EXPECT_CALL(_analogService, InitPin(1)).Times(1);
			unsigned int size = 0;
			_floatInputService = IFloatInputService::CreateFloatInputService(&_hardwareAbstractionCollection, config, size);
		}

		~FloatInputService_AnalogPinTest() override 
		{
			free(_floatInputService);
		}
	};

	TEST_F(FloatInputService_AnalogPinTest, WhenGettingValue_ThenCorrectValueIsReturned)
	{
		EXPECT_CALL(_analogService, ReadPin(1)).Times(1).WillOnce(Return(0.0f));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(0.0f, _floatInputService->Value);

		EXPECT_CALL(_analogService, ReadPin(1)).Times(1).WillOnce(Return(1.0f));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(1.0f, _floatInputService->Value);

		EXPECT_CALL(_analogService, ReadPin(1)).Times(1).WillOnce(Return(0.5f));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(0.5f, _floatInputService->Value);
	}
}
