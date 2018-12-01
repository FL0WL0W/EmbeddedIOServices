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
			stepperConfig->Table = (int *)(stepperConfig + 1);
			stepperConfig->Table[0] = -10;
			stepperConfig->Table[1] = 0;
			stepperConfig->Table[2] = 10;
			stepperConfig->Table[3] = 20;
			stepperConfig->Table[4] = 30;
			stepperConfig->Table[5] = 40;
			stepperConfig->Table[6] = 50;
			stepperConfig->Table[7] = 60;
			stepperConfig->Table[8] = 70;
			stepperConfig->Table[9] = 80;
			stepperConfig->Table[10] = 90;

			_floatOutputService = new FloatOutputService_StepperInterpolatedTable(stepperConfig, &_stepperService);
		}

		~FloatOutputService_StepperInterpolatedTableTests() override 
		{
			free(_floatOutputService);
		}
	};

	TEST_F(FloatOutputService_StepperInterpolatedTableTests, FloatOutputService_StepperInterpolatedTable_WhenGettingValueThenCorrectValueIsReturned)
	{
		EXPECT_CALL(_stepperService, Step(-10)).Times(1);
		_floatOutputService->SetOutput(10);

		EXPECT_CALL(_stepperService, Step(9)).Times(1);
		_floatOutputService->SetOutput(19.25);

		EXPECT_CALL(_stepperService, Step(91)).Times(1);
		_floatOutputService->SetOutput(110);

		EXPECT_CALL(_stepperService, Step(-100)).Times(1);
		_floatOutputService->SetOutput(10);
	}
}