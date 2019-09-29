#include "Variables/Variable_Operation.h"
#include "Operations/Operation_Math.h"

#ifdef OPERATION_MATH_H
namespace Operations
{
	Operation_Math::Operation_Math(ScalarVariableType resultType, MathOperation operation)
	{
		_resultType = resultType;
		_operation = operation;
	}

	ScalarVariable Operation_Math::Execute(ScalarVariable x, ScalarVariable y)
	{
		switch(_resultType)
		{
			case ScalarVariableType::UINT8:
				return Operate<uint8_t>(x, y, _operation);
			case ScalarVariableType::UINT16:
				return Operate<uint16_t>(x, y, _operation);
			case ScalarVariableType::UINT32:
				return Operate<uint32_t>(x, y, _operation);
			case ScalarVariableType::UINT64:
				return Operate<uint64_t>(x, y, _operation);
			case ScalarVariableType::INT8:
				return Operate<int8_t>(x, y, _operation);
			case ScalarVariableType::INT16:
				return Operate<int16_t>(x, y, _operation);
			case ScalarVariableType::INT32:
				return Operate<int32_t>(x, y, _operation);
			case ScalarVariableType::INT64:
				return Operate<int64_t>(x, y, _operation);
			case ScalarVariableType::FLOAT:
				return Operate<float>(x, y, _operation);
			case ScalarVariableType::DOUBLE:
				return Operate<double>(x, y, _operation);
			case ScalarVariableType::BOOLEAN:
				return Operate<bool>(x, y, _operation);
		}
		return { ScalarVariableType::UINT8, 0 };
	}

	IOperationBase * Operation_Math::Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
	{
		return new Operation_Math(IService::CastAndOffset<ScalarVariableType>(config, sizeOut), IService::CastAndOffset<MathOperation>(config, sizeOut));
	}
	
	IOPERATION_REGISTERFACTORY_CPP(Operation_Math, 14, ScalarVariable, ScalarVariable, ScalarVariable)
}
#endif