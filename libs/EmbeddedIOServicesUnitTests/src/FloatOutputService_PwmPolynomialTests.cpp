#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "IOServices/FloatOutputService/FloatOutputService_PwmPolynomial.h"
#include "MockTimerService.h"
#include "MockPwmService.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace HardwareAbstraction;
using namespace IOServices;

namespace UnitTests
{
	class FloatOutputService_PwmPolynomialTests : public ::testing::Test 
	{
		protected:
		MockTimerService _timerService;
		MockPwmService _pwmService;
		HardwareAbstractionCollection _hardwareAbstractionCollection;
		IFloatOutputService *_floatOutputService;

		FloatOutputService_PwmPolynomialTests() 
		{			
			_hardwareAbstractionCollection.TimerService = &_timerService;
			_hardwareAbstractionCollection.PwmService = &_pwmService;
			FloatOutputService_PwmPolynomialConfig<4> *pwmConfig = (FloatOutputService_PwmPolynomialConfig<4> *)malloc(sizeof(FloatOutputService_PwmPolynomialConfig<4>));
			
			pwmConfig->Frequency = 10000;
			pwmConfig->PwmPin = 1;
			pwmConfig->MinDutyCycle = 0.1f;
			pwmConfig->MaxDutyCycle = 0.9f;
			pwmConfig->A[0] = -1;
			pwmConfig->A[1] = 1;
			pwmConfig->A[2] = 1;
			pwmConfig->A[3] = 1;
			pwmConfig->A[4] = 0;

			void *config = malloc(pwmConfig->Size() + 1);
			void *buildConfig = config;

			//pwm polynomial service id
			*((unsigned char *)buildConfig) = 1;
			buildConfig = (void *)(((unsigned char *)buildConfig) + 1);

			memcpy(buildConfig, pwmConfig, sizeof(FloatOutputService_PwmPolynomialConfig<4>));
			buildConfig = (void *)((unsigned char *)buildConfig + sizeof(FloatOutputService_PwmPolynomialConfig<4>));

			EXPECT_CALL(_timerService, GetTicksPerSecond())
				.WillRepeatedly(Return(5000));
			EXPECT_CALL(_pwmService, InitPin(1, Out, 10000)).Times(1);
			unsigned int size = 0;
			_floatOutputService = IFloatOutputService::CreateFloatOutputService(&_hardwareAbstractionCollection, config, &size);
		}

		~FloatOutputService_PwmPolynomialTests() override 
		{
			free(_floatOutputService);
		}
	};

	TEST_F(FloatOutputService_PwmPolynomialTests, WhenSettingValueWithinLimits_ThenCorrectPinAndValueIsSet)
	{
		EXPECT_CALL(_pwmService, WritePin(1, PwmValue {0.0001f, 0.0000176000059f})).Times(1);
		_floatOutputService->SetOutput(0.6f);
	}

	TEST_F(FloatOutputService_PwmPolynomialTests, WhenSettingValueAboveMaxValue_ThenCorrectPinAndValueIsSet)
	{
		EXPECT_CALL(_pwmService, WritePin(1, PwmValue {0.0001f, 0.0000899999976f})).Times(1);
		_floatOutputService->SetOutput(10);
	}

	TEST_F(FloatOutputService_PwmPolynomialTests, WhenSettingValuBelowMinValue_ThenCorrectPinAndValueIsSet)
	{
		EXPECT_CALL(_pwmService, WritePin(1, PwmValue {0.0001f, 0.00001f})).Times(1);
		_floatOutputService->SetOutput(0);
	}
}
