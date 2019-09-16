#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "VariableBus/IVariableService.h"
#include "VariableBus/VariableService_DigitalPinRecord.h"
#include "Service/ServiceLocator.h"
#include "MockDigitalService.h"
#include "MockTimerService.h"
#include "HardwareAbstraction/HardwareAbstractionCollection.h"
#include "Service/HardwareAbstractionServiceBuilder.h"
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::SaveArg;
using::testing::_;

using namespace HardwareAbstraction;
using namespace Service;
using namespace VariableBus;

namespace UnitTests
{
	class VariableService_DigitalPinRecordTests : public ::testing::Test 
	{
		protected:
		MockDigitalService _digitalService;
		MockTimerService _timerService;
		HardwareAbstractionCollection _hardwareAbstractionCollection;
		ServiceLocator *_serviceLocator;
		IVariableService *_variableService;
		ICallBack *_callBack = 0;
		Record *_record;

		VariableService_DigitalPinRecordTests() 
		{
			_serviceLocator = new ServiceLocator();

			_hardwareAbstractionCollection.DigitalService = &_digitalService;
			_hardwareAbstractionCollection.TimerService = &_timerService;
			_serviceLocator->Register(HARDWARE_ABSTRACTION_COLLECTION_ID, &_hardwareAbstractionCollection);
			_serviceLocator->Register(DIGITAL_SERVICE_ID, _hardwareAbstractionCollection.DigitalService);
			_serviceLocator->Register(TIMER_SERVICE_ID, _hardwareAbstractionCollection.TimerService);

			unsigned int expectedSize = sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint8_t) + sizeof(uint16_t) + sizeof(bool);
			void *config = malloc(expectedSize);
			void *buildConfig = config;

			//digital pin factory id
			*((uint16_t *)buildConfig) = 4;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			//variable id
			*((uint16_t *)buildConfig) = 1;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			//length
			*((uint8_t *)buildConfig) = 8;
			buildConfig = (void *)(((uint8_t *)buildConfig) + 1);

			//pin 1
			*((uint16_t *)buildConfig) = 1;
			buildConfig = (void *)(((uint16_t *)buildConfig) + 1);

			*((bool *)buildConfig) = false;
			buildConfig = (void *)(((bool *)buildConfig) + 1);
			
			EXPECT_CALL(_digitalService, ScheduleRecurringInterrupt(1, _))
				.Times(1)
				.WillOnce(SaveArg<1>(&_callBack));

			unsigned int size = 0;
			VariableService_DigitalPinRecord::RegisterFactory();
			_variableService = IVariableService::Create(_serviceLocator, config, size);
			EXPECT_EQ(expectedSize, size);
			_record = _serviceLocator->LocateAndCast<Record>(BUILDER_VARIABLEBUS, 1);
		}
	};

	TEST_F(VariableService_DigitalPinRecordTests, WhenGettingHighForUninverted_ThenTrueIsReturned)
	{
		ASSERT_EQ(0, _record->Last);
		EXPECT_CALL(_timerService, GetTick()).Times(1).WillOnce(Return(10));
	 	EXPECT_CALL(_digitalService, ReadPin(1)).Times(1).WillOnce(Return(false));
		_callBack->Execute();
		ASSERT_EQ(1, _record->Last);
		ASSERT_EQ(true, _record->Frames[_record->Last].Valid);
		ASSERT_EQ(false, _record->Frames[_record->Last].State);
		ASSERT_EQ(10, _record->Frames[_record->Last].Tick);

		EXPECT_CALL(_timerService, GetTick()).Times(1).WillOnce(Return(11));
	 	EXPECT_CALL(_digitalService, ReadPin(1)).Times(1).WillOnce(Return(false));
		_callBack->Execute();
		ASSERT_EQ(1, _record->Last);
		ASSERT_EQ(true, _record->Frames[_record->Last].Valid);
		ASSERT_EQ(false, _record->Frames[_record->Last].State);
		ASSERT_EQ(10, _record->Frames[_record->Last].Tick);

		EXPECT_CALL(_timerService, GetTick()).Times(1).WillOnce(Return(12));
	 	EXPECT_CALL(_digitalService, ReadPin(1)).Times(1).WillOnce(Return(true));
		_callBack->Execute();
		ASSERT_EQ(2, _record->Last);
		ASSERT_EQ(true, _record->Frames[_record->Last].Valid);
		ASSERT_EQ(true, _record->Frames[_record->Last].State);
		ASSERT_EQ(12, _record->Frames[_record->Last].Tick);
	}

	// TEST_F(VariableService_DigitalPinRecordTests, WhenGettingLowForUninverted_ThenFalseIsReturned)
	// {
	// 	EXPECT_CALL(_digitalService, ReadPin(1)).Times(1).WillOnce(Return(false));
	// 	ASSERT_EQ(false, _operationUninverted->Execute());
	// }

	// TEST_F(VariableService_DigitalPinRecordTests, WhenGettingHighForInverted_ThenFalseIsReturned)
	// {
	// 	EXPECT_CALL(_digitalService, ReadPin(2)).Times(1).WillOnce(Return(true));
	// 	ASSERT_EQ(false, _operationInverted->Execute());
	// }

	// TEST_F(VariableService_DigitalPinRecordTests, WhenGettingLowForInverted_ThenTrueIsReturned)
	// {
	// 	EXPECT_CALL(_digitalService, ReadPin(2)).Times(1).WillOnce(Return(false));
	// 	ASSERT_EQ(true, _operationInverted->Execute());
	// }
}
