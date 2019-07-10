#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "IOServices/FloatInputService/IFloatInputService.h"
#include "MockPwmService.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace HardwareAbstraction;
using namespace IOServices;

namespace UnitTests
{
	class FloatInputService_FrequencyPinTest : public ::testing::Test 
	{
		protected:
		MockPwmService _pwmService;
		HardwareAbstractionCollection _hardwareAbstractionCollection;
		IFloatInputService *_floatInputService;

		FloatInputService_FrequencyPinTest() 
		{
			_hardwareAbstractionCollection.PwmService = &_pwmService;

			void *config = malloc(sizeof(uint8_t) + sizeof(uint16_t) + sizeof(uint16_t));
			void *buildConfig = config;

			//frequency pin service id
			*((uint8_t *)buildConfig) = 3;
			buildConfig = (void *)(((uint8_t *)buildConfig) + 1);

			//pin 1
			*((uint16_t *)buildConfig) = 1;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			//minFrequency 2
			*((uint16_t *)buildConfig) = 2;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

 			EXPECT_CALL(_pwmService, InitPin(1, In, 2)).Times(1);
			unsigned int size = 0;
			_floatInputService = IFloatInputService::CreateFloatInputService(&_hardwareAbstractionCollection, config, size);
		}

		~FloatInputService_FrequencyPinTest() override 
		{
			free(_floatInputService);
		}
	};

	TEST_F(FloatInputService_FrequencyPinTest, WhenGettingValue_ThenCorrectValueIsReturned)
	{
		PwmValue pwmValue = { 0.1f, 0.05f };
		EXPECT_CALL(_pwmService, ReadPin(1)).Times(1).WillOnce(Return(pwmValue));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(10, _floatInputService->Value);

		pwmValue = { 0.025f, 0.004f };
		EXPECT_CALL(_pwmService, ReadPin(1)).Times(1).WillOnce(Return(pwmValue));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(40, _floatInputService->Value);

		pwmValue = { 0.05f, 0.04f };
		EXPECT_CALL(_pwmService, ReadPin(1)).Times(1).WillOnce(Return(pwmValue));
		_floatInputService->ReadValue();
		ASSERT_NEAR(20, _floatInputService->Value, 0.001f);

		pwmValue = { 0.05333333333333f, 0.04f };
		EXPECT_CALL(_pwmService, ReadPin(1)).Times(1).WillOnce(Return(pwmValue));
		_floatInputService->ReadValue();
		ASSERT_NEAR(18.75f, _floatInputService->Value, 0.001f);
	}
}
