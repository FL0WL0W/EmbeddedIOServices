#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Operations/Operation_TicksToSeconds.h"
#include "MockTimerService.h"
#include "HardwareAbstraction/HardwareAbstractionCollection.h"
#include "Service/HardwareAbstractionServiceBuilder.h"

using ::testing::AtLeast;
using ::testing::Return;

using namespace HardwareAbstraction;
using namespace Operations;
using namespace Service;

namespace UnitTests
{
	class Operation_TicksToSecondsTests : public ::testing::Test 
	{
		protected:
		ServiceLocator *_serviceLocator;
		IOperation<ScalarVariable, ScalarVariable> *_operation;
		MockTimerService _timerService;
		HardwareAbstractionCollection _hardwareAbstractionCollection;
		unsigned int _size = 0;

		Operation_TicksToSecondsTests() 
		{			
			_serviceLocator = new ServiceLocator();
			_hardwareAbstractionCollection.TimerService = &_timerService;
			_serviceLocator->Register(HARDWARE_ABSTRACTION_COLLECTION_ID, &_hardwareAbstractionCollection);
			_serviceLocator->Register(TIMER_SERVICE_ID, _hardwareAbstractionCollection.TimerService);

			void *config = malloc(2);

			//factory id 17
			*((uint16_t *)config) = 18;

			EXPECT_CALL(_timerService, GetTicksPerSecond()).WillRepeatedly(Return(5000));
			Operation_TicksToSeconds::RegisterFactory();
			_operation = static_cast<IOperation<ScalarVariable, ScalarVariable> *>(IOperationBase::Create(_serviceLocator, config, _size));
		}
	};

	TEST_F(Operation_TicksToSecondsTests, WhenGettingValues_ThenCorrectValueIsReturned)
	{
		ASSERT_FLOAT_EQ(0.0F, _operation->Execute(ScalarVariable::FromTick(0)).To<float>());

		ASSERT_FLOAT_EQ(0.1f, _operation->Execute(ScalarVariable::FromTick(500)).To<float>());
	}
}
