#include "Operations/IOperation.h"
#include "Service/IService.h"
#include "Service/ServiceLocator.h"
#include "Packed.h"
#include "Interpolation.h"
#include "ScalarVariable.h"
#include "HardwareAbstraction/ITimerService.h"

/*
To create this operator
uint16									6001(BUILDER_OPERATION)
uint16									xx(InstanceID of Operation)
uint16									14(FactoryID)
ScalarVariableType						ResultType
MathOperation							Operation

To use this operator on a variable in the main loop
uint16									7001(BUILDER_VARIABLE)
uint16									14(FactoryID)
uint16									xx(InstanceID of Variable Result)
uint16									xx(InstanceID of Variable X)
uint16									xx(InstanceID of Variable Y)

To create a CallBack to use this operator on a variable
uint16									7002(BUILDER_VARIABLE_TRANSLATE_CALL_BACK)
uint16									xx(InstanceID of CallBack)
uint16									14(FactoryID)
uint16									xx(InstanceID of Variable Result)
uint16									xx(InstanceID of Variable X)
uint16									xx(InstanceID of Variable Y)
*/

#ifndef OPERATION_GETTICK_H
#define OPERATION_GETTICK_H
namespace Operations
{
	class Operation_GetTick : public IOperation<ScalarVariable>
	{
	protected:
		static Operation_GetTick *_instance;
		HardwareAbstraction::ITimerService *_timerService;
	public:		
        Operation_GetTick(HardwareAbstraction::ITimerService *timerService);

		ScalarVariable Execute() override;

		static IOperationBase *Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut);
		ISERVICE_REGISTERFACTORY_H
	};
}
#endif