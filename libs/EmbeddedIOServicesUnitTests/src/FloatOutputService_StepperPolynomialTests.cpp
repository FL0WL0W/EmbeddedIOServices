// #include "gmock/gmock.h"
// #include "gtest/gtest.h"
// #include "IOServices/FloatOutputService/IFloatOutputService.h"
// #include "IOServices/FloatOutputService/FloatOutputService_StepperPolynomial.h"
// #include "MockStepperOutputService.h"
// using ::testing::AtLeast;
// using ::testing::Return;

// using namespace HardwareAbstraction;
// using namespace IOServices;

// namespace UnitTests
// {
// 	class FloatOutputService_StepperPolynomialTests : public ::testing::Test 
// 	{
// 		protected:
// 		MockStepperOutputService _stepperService;
// 		IFloatOutputService *_floatOutputService;

// 		FloatOutputService_StepperPolynomialTests() 
// 		{			
// 			FloatOutputService_StepperPolynomialConfig<4> *stepperConfig = (FloatOutputService_StepperPolynomialConfig<4> *)malloc(sizeof(FloatOutputService_StepperPolynomialConfig<4>));
			
// 			stepperConfig->MinStepPosition = -40;
// 			stepperConfig->MaxStepPosition = 150;
// 			stepperConfig->A[0] = -10;
// 			stepperConfig->A[1] = 10;
// 			stepperConfig->A[2] = 10;
// 			stepperConfig->A[3] = 10;
// 			stepperConfig->A[4] = 0;

// 			_floatOutputService = new FloatOutputService_StepperPolynomial<4>(stepperConfig, &_stepperService);
// 		}

// 		~FloatOutputService_StepperPolynomialTests() override 
// 		{
// 			free(_floatOutputService);
// 		}
// 	};

// 	TEST_F(FloatOutputService_StepperPolynomialTests, WhenCalibrating_ThenCalibrateIsPassedThroughToTheStepperService)
// 	{
// 		EXPECT_CALL(_stepperService, Calibrate()).Times(1);
// 		_floatOutputService->Calibrate();
// 	}

// 	TEST_F(FloatOutputService_StepperPolynomialTests, WhenSettingValueWithinLimits_ThenCorrectValueIsSet)
// 	{
// 		EXPECT_CALL(_stepperService, Step(-10)).Times(1);
// 		_floatOutputService->SetOutput(0);

// 		EXPECT_CALL(_stepperService, Step(30)).Times(1);
// 		_floatOutputService->SetOutput(1);

// 		EXPECT_CALL(_stepperService, Step(-21)).Times(1);
// 		_floatOutputService->SetOutput(0.5);

// 		EXPECT_CALL(_stepperService, Step(-1)).Times(1);
// 		_floatOutputService->SetOutput(0.49f);
// 	}

// 	TEST_F(FloatOutputService_StepperPolynomialTests, WhenSettingValueAboveMaxValue_ThenCorrectValueIsSet)
// 	{
// 		EXPECT_CALL(_stepperService, Step(150)).Times(1);
// 		_floatOutputService->SetOutput(100);
// 	}

// 	TEST_F(FloatOutputService_StepperPolynomialTests, WhenSettingValueBelowMinValue_ThenCorrectValueIsSet)
// 	{
// 		EXPECT_CALL(_stepperService, Step(-40)).Times(1);
// 		_floatOutputService->SetOutput(-100);
// 	}
// }
