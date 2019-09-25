#include "Operations/IOperation.h"
#include "Service/IService.h"
#include "Service/ServiceLocator.h"
#include "Packed.h"
#include "Interpolation.h"
#include "VariableType.h"

/*
To create this operator
uint16									6001(BUILDER_OPERATION)
uint16									xx(InstanceID of Operation)
uint16									2(FactoryID)
float 									MinXValue
float 									MaxXValue
uint8 									XResolution
VariableType							TableType
TableType[XResolution]					Table

To use this operator on a variable
uint16									7001(BUILDER_VARIABLE)
uint16									2(FactoryID)
uint16									xx(InstanceID of Variable Result)
uint16									xx(InstanceID of Operation)
uint16									xx(InstanceID of Variable Input)
*/

#ifndef OPERATION_LOOKUPTABLE_H
#define OPERATION_LOOKUPTABLE_H
namespace Operations
{
	PACK(
	struct Operation_LookupTableConfig
	{
	private:
		Operation_LookupTableConfig()
		{
			
		}
		
	public:		
		constexpr const unsigned int Size() const
		{
			return sizeof(Operation_LookupTableConfig) +
				(VariableTypeSizeOf(TableType) * XResolution);
		}

		constexpr const void *Table() const { return this + 1; }
		
		float MinXValue;
		float MaxXValue;
		uint8_t XResolution;
		VariableType TableType;
	});

	class Operation_LookupTable : public IOperation<float, float>
	{
	protected:
		const Operation_LookupTableConfig *_config;
	public:		
        Operation_LookupTable(const Operation_LookupTableConfig * const &config);

		float Execute(float x) override;

		static IOperationBase *Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut);
		ISERVICE_REGISTERFACTORY_H
	};
}
#endif