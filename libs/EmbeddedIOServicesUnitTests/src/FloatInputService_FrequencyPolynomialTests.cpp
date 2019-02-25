#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "IOServices/FloatInputService/FloatInputService_FrequencyPolynomial.h"
#include "MockTimerService.h"
#include "MockPwmService.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace HardwareAbstraction;
using namespace IOServices;

namespace UnitTests
{
	class FloatInputService_FrequencyPolynomialTest : public ::testing::Test 
	{
		protected:
		MockTimerService _timerService;
		MockPwmService _pwmService;
		HardwareAbstractionCollection _hardwareAbstractionCollection;
		IFloatInputService *_floatInputService;

		FloatInputService_FrequencyPolynomialTest() 
		{
			//setup
			_hardwareAbstractionCollection.TimerService = &_timerService;
			_hardwareAbstractionCollection.PwmService = &_pwmService;

			FloatInputService_FrequencyPolynomialConfig<4> *analogConfig = (FloatInputService_FrequencyPolynomialConfig<4> *)malloc(sizeof(FloatInputService_FrequencyPolynomialConfig<4>));

			//adcPin
			analogConfig->PwmPin = 1;
			analogConfig->A[0] = -10;
			analogConfig->A[1] = 10;
			analogConfig->A[2] = 10;
			analogConfig->A[3] = 10;
			analogConfig->A[4] = 0;
			analogConfig->DotSampleRate = 500;
			analogConfig->MaxValue = 150;
			analogConfig->MinValue = 30;
			analogConfig->MinFrequency = 10;

			void *config = malloc(analogConfig->Size() + 1);
			void *buildConfig = config;

			//frequency polynomial service id
			*((unsigned char *)buildConfig) = 3;
			buildConfig = (void *)(((unsigned char *)buildConfig) + 1);

			memcpy(buildConfig, analogConfig, sizeof(FloatInputService_FrequencyPolynomialConfig<4>));
			buildConfig = (void *)((unsigned char *)buildConfig + sizeof(FloatInputService_FrequencyPolynomialConfig<4>));

			EXPECT_CALL(_timerService, GetTicksPerSecond())
				.WillRepeatedly(Return(5000));
			EXPECT_CALL(_pwmService, InitPin(1, In, analogConfig->MinFrequency)).Times(1);
			unsigned int size = 0;
			_floatInputService = IFloatInputService::CreateFloatInputService(&_hardwareAbstractionCollection, config, &size);
		}

		~FloatInputService_FrequencyPolynomialTest() override 
		{
			free(_floatInputService);
		}
	};

	TEST_F(FloatInputService_FrequencyPolynomialTest, WhenGettingValue_ThenCorrectValueIsReturned)
	{
		HardwareAbstraction::PwmValue pwmValue = { 1, 0.5f };
		EXPECT_CALL(_timerService, GetTick()).Times(1).WillOnce(Return(5));
		EXPECT_CALL(_pwmService, ReadPin(1)).Times(1).WillOnce(Return(pwmValue));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(30, _floatInputService->Value);
		ASSERT_FLOAT_EQ(0, _floatInputService->ValueDot);

		pwmValue = { 0.5f, 0.4f };
		EXPECT_CALL(_timerService, GetTick()).Times(2).WillRepeatedly(Return(10));
		EXPECT_CALL(_pwmService, ReadPin(1)).Times(1).WillOnce(Return(pwmValue));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(130, _floatInputService->Value);
		ASSERT_FLOAT_EQ(65000, _floatInputService->ValueDot);

		pwmValue = { 0.25f, 0.2f };
		EXPECT_CALL(_timerService, GetTick()).Times(1).WillOnce(Return(15));
		EXPECT_CALL(_pwmService, ReadPin(1)).Times(1).WillOnce(Return(pwmValue));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(150, _floatInputService->Value);
		ASSERT_FLOAT_EQ(65000, _floatInputService->ValueDot);

		EXPECT_CALL(_timerService, GetTick()).Times(2).WillRepeatedly(Return(20));
		EXPECT_CALL(_pwmService, ReadPin(1)).Times(1).WillOnce(Return(pwmValue));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(150, _floatInputService->Value);
		ASSERT_FLOAT_EQ(10000, _floatInputService->ValueDot);
	}
}