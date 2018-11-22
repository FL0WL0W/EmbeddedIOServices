#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "IOServices/FloatInputService/FloatInputService_AnalogPolynomial.h"
#include "MockTimerService.h"
#include "MockAnalogService.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace HardwareAbstraction;
using namespace IOServices;

namespace UnitTests
{
	class FloatInputService_AnalogPolynomialTest : public ::testing::Test 
	{
		protected:
		MockTimerService _timerService;
		MockAnalogService _analogService;
		HardwareAbstractionCollection _hardwareAbstractionCollection;
		IFloatInputService *_floatInputService;

		FloatInputService_AnalogPolynomialTest() 
		{
			_hardwareAbstractionCollection.TimerService = &_timerService;
			_hardwareAbstractionCollection.AnalogService = &_analogService;
			
			FloatInputService_AnalogPolynomialConfig<4> *analogConfig = (FloatInputService_AnalogPolynomialConfig<4> *)malloc(sizeof(FloatInputService_AnalogPolynomialConfig<4>));

			//adcPin
			analogConfig->AdcPin = 1;
			analogConfig->A[0] = -10;
			analogConfig->A[1] = 10;
			analogConfig->A[2] = 10;
			analogConfig->A[3] = 10;
			analogConfig->A[4] = 0;
			analogConfig->DotSampleRate = 500;
			analogConfig->MaxValue = 150;
			analogConfig->MinValue = -40;

			void *config = malloc(analogConfig->Size() + 1);
			void *buildConfig = config;

			//analog polynomial service id
			*((unsigned char *)buildConfig) = 2;
			buildConfig = (void *)(((unsigned char *)buildConfig) + 1);

			memcpy(buildConfig, analogConfig, sizeof(FloatInputService_AnalogPolynomialConfig<4>));
			buildConfig = (void *)((unsigned char *)buildConfig + sizeof(FloatInputService_AnalogPolynomialConfig<4>));

			EXPECT_CALL(_timerService, GetTicksPerSecond())
				.WillRepeatedly(Return(5000));
			EXPECT_CALL(_analogService, InitPin(1)).Times(1);
			unsigned int size = 0;
			_floatInputService = IFloatInputService::CreateFloatInputService(&_hardwareAbstractionCollection, config, &size);
		}

		~FloatInputService_AnalogPolynomialTest() override 
		{
			free(_floatInputService);
		}
	};

	TEST_F(FloatInputService_AnalogPolynomialTest, FloatInputService_AnalogPolynomial_WhenGettingValueThenCorrectValueIsReturned)
	{
		EXPECT_CALL(_timerService, GetTick()).Times(1).WillOnce(Return(5));
		EXPECT_CALL(_analogService, ReadPin(1)).Times(1).WillOnce(Return(0));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(-10.0f, _floatInputService->Value);
		ASSERT_FLOAT_EQ(0.0f, _floatInputService->ValueDot);

		EXPECT_CALL(_timerService, GetTick()).Times(2).WillRepeatedly(Return(10));
		EXPECT_CALL(_analogService, ReadPin(1)).Times(1).WillOnce(Return(1));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(20.0f, _floatInputService->Value);
		ASSERT_FLOAT_EQ(10000.0f, _floatInputService->ValueDot);

		EXPECT_CALL(_timerService, GetTick()).Times(1).WillOnce(Return(15));
		EXPECT_CALL(_analogService, ReadPin(1)).Times(1).WillOnce(Return(0.5));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(-1.25f, _floatInputService->Value);
		ASSERT_FLOAT_EQ(10000.0f, _floatInputService->ValueDot);

		EXPECT_CALL(_timerService, GetTick()).Times(2).WillRepeatedly(Return(20));
		EXPECT_CALL(_analogService, ReadPin(1)).Times(1).WillOnce(Return(0.5));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(-1.25f, _floatInputService->Value);
		ASSERT_FLOAT_EQ(-10625.0f, _floatInputService->ValueDot);

		EXPECT_CALL(_timerService, GetTick()).Times(2).WillRepeatedly(Return(30));
		EXPECT_CALL(_analogService, ReadPin(1)).Times(1).WillOnce(Return(100));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(150.0f, _floatInputService->Value);

		EXPECT_CALL(_timerService, GetTick()).Times(1).WillOnce(Return(30));
		EXPECT_CALL(_analogService, ReadPin(1)).Times(1).WillOnce(Return(-100));
		_floatInputService->ReadValue();
		ASSERT_FLOAT_EQ(-40.0f, _floatInputService->Value);
	}
}