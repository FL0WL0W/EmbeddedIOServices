#include "Operations/IOperation.h"
#include "Service/IService.h"
#include "Service/ServiceLocator.h"
#include "Packed.h"
#include "Interpolation.h"
#include "ScalarVariable.h"

/*
To create this operator
uint16									6001(BUILDER_OPERATION)
uint16									xx(InstanceID of Operation)
uint16									14(FactoryID)
ScalarVariableType						ResultType(FactoryID)

To use this operator on a variable
uint16									7001(BUILDER_VARIABLE)
uint16									14(FactoryID)
uint16									xx(InstanceID of Variable Result)
uint16									xx(InstanceID of Operation)
uint16									xx(InstanceID of Variable X)
uint16									xx(InstanceID of Variable Y)
*/

const enum MathOperation
{
    ADD = 0,
    SUBTRACT = 1,
    MULTIPLY = 2,
    DIVIDE = 3,
	AND = 4,
	OR = 5
};

#ifndef OPERATION_MATH_H
#define OPERATION_MATH_H
namespace Operations
{
	class Operation_Math : public IOperation<ScalarVariable, ScalarVariable, ScalarVariable>
	{
	protected:
		ScalarVariableType _resultType;
		MathOperation _operation;
	public:		
        Operation_Math(ScalarVariableType resultType, MathOperation operation);

		template<typename K> 
		static ScalarVariable Operate(ScalarVariable x, ScalarVariable y, MathOperation operation)
		{
			switch (operation)
			{
				case ADD:
					return ScalarVariableFrom(ScalarVariableTo<K>(x) + ScalarVariableTo<K>(y));
				case SUBTRACT:
					return ScalarVariableFrom(ScalarVariableTo<K>(x) - ScalarVariableTo<K>(y));
				case MULTIPLY:
					return ScalarVariableFrom(ScalarVariableTo<K>(x) * ScalarVariableTo<K>(y));
				case DIVIDE:
					return ScalarVariableFrom(ScalarVariableTo<K>(x) / ScalarVariableTo<K>(y));
				case AND:
					return ScalarVariableFrom(ScalarVariableTo<K>(x) & ScalarVariableTo<K>(y));
				case OR:
					return ScalarVariableFrom(ScalarVariableTo<K>(x) | ScalarVariableTo<K>(y));
			}
			return { ScalarVariableType::UINT8, 0 };
		}
		template<> 
		static ScalarVariable Operate<float>(ScalarVariable x, ScalarVariable y, MathOperation operation)
		{
			switch (operation)
			{
				case ADD:
					return ScalarVariableFrom(ScalarVariableTo<float>(x) + ScalarVariableTo<float>(y));
				case SUBTRACT:
					return ScalarVariableFrom(ScalarVariableTo<float>(x) - ScalarVariableTo<float>(y));
				case MULTIPLY:
					return ScalarVariableFrom(ScalarVariableTo<float>(x) * ScalarVariableTo<float>(y));
				case DIVIDE:
					return ScalarVariableFrom(ScalarVariableTo<float>(x) / ScalarVariableTo<float>(y));
			}
			return { ScalarVariableType::UINT8, 0 };
		}
		template<> 
		static ScalarVariable Operate<double>(ScalarVariable x, ScalarVariable y, MathOperation operation)
		{
			switch (operation)
			{
				case ADD:
					return ScalarVariableFrom(ScalarVariableTo<double>(x) + ScalarVariableTo<double>(y));
				case SUBTRACT:
					return ScalarVariableFrom(ScalarVariableTo<double>(x) - ScalarVariableTo<double>(y));
				case MULTIPLY:
					return ScalarVariableFrom(ScalarVariableTo<double>(x) * ScalarVariableTo<double>(y));
				case DIVIDE:
					return ScalarVariableFrom(ScalarVariableTo<double>(x) / ScalarVariableTo<double>(y));
			}
			return { ScalarVariableType::UINT8, 0 };
		}

		ScalarVariable Execute(ScalarVariable x, ScalarVariable y) override;

		static IOperationBase *Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut);
		ISERVICE_REGISTERFACTORY_H
	};
}
#endif