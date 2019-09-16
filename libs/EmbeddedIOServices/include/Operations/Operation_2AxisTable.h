#include "Operations/IOperation.h"
#include "Service/IService.h"
#include "Service/ServiceLocator.h"
#include "Packed.h"
#include "Interpolation.h"
#include "VariableType.h"

#ifndef OPERATION_2AXISTABLE_H
#define OPERATION_2AXISTABLE_H
namespace Operations
{
	PACK(
	struct Operation_2AxisTableConfig
	{
	private:
		Operation_2AxisTableConfig()
		{
			
		}
		
	public:		
		constexpr const unsigned int Size() const
		{
			return sizeof(Operation_2AxisTableConfig) +
				(VariableTypeSizeOf(TableType) * XResolution * YResolution);
		}

		constexpr const void *Table() const { return this + 1; }
		
		float MinXValue;
		float MaxXValue;
		uint8_t XResolution;
		float MinYValue;
		float MaxYValue;
		uint8_t YResolution;
		VariableType TableType;
	});

	class Operation_2AxisTable : public IOperation<float, float, float>
	{
	protected:
		const Operation_2AxisTableConfig *_config;
	public:		
        Operation_2AxisTable(const Operation_2AxisTableConfig * const &config);

		float Execute(float x, float y) override;

		static IOperationBase *Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut);
		ISERVICE_REGISTERFACTORY_H
	};
}
#endif