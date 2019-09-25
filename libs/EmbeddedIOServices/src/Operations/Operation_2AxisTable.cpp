#include "Variables/Variable_Operation.h"
#include "Operations/Operation_2AxisTable.h"

#ifdef OPERATION_2AXISTABLE_H
namespace Operations
{
	Operation_2AxisTable::Operation_2AxisTable(const Operation_2AxisTableConfig * const &config)
	{
		_config = config;
	}

	float Operation_2AxisTable::Execute(float x, float y)
	{
		switch(_config->TableType)
		{
			case VariableType::UINT8:
				return static_cast<float>(Interpolation::InterpolateTable2<uint8_t>(x, _config->MaxXValue, _config->MinXValue, _config->XResolution, y, _config->MaxYValue, _config->MinYValue, _config->YResolution, reinterpret_cast<const uint8_t*>(_config->Table())));
			case VariableType::UINT16:
				return static_cast<float>(Interpolation::InterpolateTable2<uint16_t>(x, _config->MaxXValue, _config->MinXValue, _config->XResolution, y, _config->MaxYValue, _config->MinYValue, _config->YResolution, reinterpret_cast<const uint16_t*>(_config->Table())));
			case VariableType::UINT32:
				return static_cast<float>(Interpolation::InterpolateTable2<uint32_t>(x, _config->MaxXValue, _config->MinXValue, _config->XResolution, y, _config->MaxYValue, _config->MinYValue, _config->YResolution, reinterpret_cast<const uint32_t*>(_config->Table())));
			case VariableType::UINT64:
				return static_cast<float>(Interpolation::InterpolateTable2<uint64_t>(x, _config->MaxXValue, _config->MinXValue, _config->XResolution, y, _config->MaxYValue, _config->MinYValue, _config->YResolution, reinterpret_cast<const uint64_t*>(_config->Table())));
			case VariableType::INT8:
				return static_cast<float>(Interpolation::InterpolateTable2<int8_t>(x, _config->MaxXValue, _config->MinXValue, _config->XResolution, y, _config->MaxYValue, _config->MinYValue, _config->YResolution, reinterpret_cast<const int8_t*>(_config->Table())));
			case VariableType::INT16:
				return static_cast<float>(Interpolation::InterpolateTable2<int16_t>(x, _config->MaxXValue, _config->MinXValue, _config->XResolution, y, _config->MaxYValue, _config->MinYValue, _config->YResolution, reinterpret_cast<const int16_t*>(_config->Table())));
			case VariableType::INT32:
				return static_cast<float>(Interpolation::InterpolateTable2<int32_t>(x, _config->MaxXValue, _config->MinXValue, _config->XResolution, y, _config->MaxYValue, _config->MinYValue, _config->YResolution, reinterpret_cast<const int32_t*>(_config->Table())));
			case VariableType::INT64:
				return static_cast<float>(Interpolation::InterpolateTable2<int64_t>(x, _config->MaxXValue, _config->MinXValue, _config->XResolution, y, _config->MaxYValue, _config->MinYValue, _config->YResolution, reinterpret_cast<const int64_t*>(_config->Table())));
			case VariableType::FLOAT:
				return Interpolation::InterpolateTable2<float>(x, _config->MaxXValue, _config->MinXValue, _config->XResolution, y, _config->MaxYValue, _config->MinYValue, _config->YResolution, reinterpret_cast<const float*>(_config->Table()));
			case VariableType::DOUBLE:
				return static_cast<float>(Interpolation::InterpolateTable2<double>(x, _config->MaxXValue, _config->MinXValue, _config->XResolution, y, _config->MaxYValue, _config->MinYValue, _config->YResolution, reinterpret_cast<const double*>(_config->Table())));
		}
		return 0;
	}

	IOperationBase * Operation_2AxisTable::Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
	{
		const Operation_2AxisTableConfig *tableConfig = reinterpret_cast<const Operation_2AxisTableConfig *>(config);
		sizeOut += tableConfig->Size();
		return new Operation_2AxisTable(tableConfig);
	}
	
	IOPERATION_REGISTERFACTORY_CPP(Operation_2AxisTable, 3, float, float, float)
}
#endif