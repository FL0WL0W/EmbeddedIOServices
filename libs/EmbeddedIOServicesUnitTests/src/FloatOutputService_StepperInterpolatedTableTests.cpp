#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "IOServices/FloatOutputService/FloatOutputService_StepperInterpolatedTable.h"
#include "MockStepperOutputService.h"
using ::testing::AtLeast;
using ::testing::Return;

using namespace HardwareAbstraction;
using namespace IOServices;

namespace UnitTests
{
	class FloatOutputService_StepperInterpolatedTableTests : public ::testing::Test 
	{
		protected:
		MockStepperOutputService _stepperService;
		IFloatOutputService *_floatOutputService;

		FloatOutputService_StepperInterpolatedTableTests() 
		{			
			FloatOutputService_StepperInterpolatedTableConfig *stepperConfig = (FloatOutputService_StepperInterpolatedTableConfig *)malloc(sizeof(FloatOutputService_StepperInterpolatedTableConfig) + 4 * 11);
			
			stepperConfig->MinValue = 10;
			stepperConfig->MaxValue = 110;
			stepperConfig->Resolution = 11;
			int *Table = (int *)(stepperConfig + 1);
			Table[0] = -10;
			Table[1] = 0;
			Table[2] = 10;
			Table[3] = 20;
			Table[4] = 30;
			Table[5] = 40;
			Table[6] = 50;
			Table[7] = 60;
			Table[8] = 70;
			Table[9] = 80;
			Table[10] = 90;

			_floatOutputService = new FloatOutputService_StepperInterpolatedTable(stepperConfig, &_stepperService);
		}

		~FloatOutputService_StepperInterpolatedTableTests() override 
		{
			free(_floatOutputService);
		}
	};

	TEST_F(FloatOutputService_StepperInterpolatedTableTests, WhenCalibrating_ThenCalibrateIsPassedThroughToTheStepperService)
	{
		EXPECT_CALL(_stepperService, Calibrate()).Times(1);
		_floatOutputService->Calibrate();
	}

	TEST_F(FloatOutputService_StepperInterpolatedTableTests, WhenSettingValueWithinTable_ThenCorrectValueIsSet)
	{
		EXPECT_CALL(_stepperService, Step(-10)).Times(1);
		_floatOutputService->SetOutput(10);

		EXPECT_CALL(_stepperService, Step(9)).Times(1);
		_floatOutputService->SetOutput(19.25);
	}

	TEST_F(FloatOutputService_StepperInterpolatedTableTests, WhenSettingValueAboveMaxValue_ThenCorrectValueIsSet)
	{
		EXPECT_CALL(_stepperService, Step(90)).Times(1);
		_floatOutputService->SetOutput(120);
	}

	TEST_F(FloatOutputService_StepperInterpolatedTableTests, WhenSettingValueBelowMinValue_ThenCorrectValueIsSet)
	{
		EXPECT_CALL(_stepperService, Step(-10)).Times(1);
		_floatOutputService->SetOutput(0);
	}
}
