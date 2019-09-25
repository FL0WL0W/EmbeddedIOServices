#include "Variables/Variable_Operation.h"
#include "Operations/Operation_LookupTable.h"

#ifdef OPERATION_LOOKUPTABLE_H
namespace Operations
{
	Operation_LookupTable::Operation_LookupTable(const Operation_LookupTableConfig * const &config)
	{
		_config = config;
	}

	float Operation_LookupTable::Execute(float x)
	{
		switch(_config->TableType)
		{
			case VariableType::UINT8:
				return static_cast<float>(Interpolation::InterpolateTable1<uint8_t>(x, _config->MaxXValue, _config->MinXValue, _config->XResolution, reinterpret_cast<const uint8_t*>(_config->Table())));
			case VariableType::UINT16:
				return static_cast<float>(Interpolation::InterpolateTable1<uint16_t>(x, _config->MaxXValue, _config->MinXValue, _config->XResolution, reinterpret_cast<const uint16_t*>(_config->Table())));
			case VariableType::UINT32:
				return static_cast<float>(Interpolation::InterpolateTable1<uint16_t>(x, _config->MaxXValue, _config->MinXValue, _config->XResolution, reinterpret_cast<const uint16_t*>(_config->Table())));
			case VariableType::UINT64:
				return static_cast<float>(Interpolation::InterpolateTable1<uint64_t>(x, _config->MaxXValue, _config->MinXValue, _config->XResolution, reinterpret_cast<const uint64_t*>(_config->Table())));
			case VariableType::INT8:
				return static_cast<float>(Interpolation::InterpolateTable1<int8_t>(x, _config->MaxXValue, _config->MinXValue, _config->XResolution, reinterpret_cast<const int8_t*>(_config->Table())));
			case VariableType::INT16:
				return static_cast<float>(Interpolation::InterpolateTable1<int16_t>(x, _config->MaxXValue, _config->MinXValue, _config->XResolution, reinterpret_cast<const int16_t*>(_config->Table())));
			case VariableType::INT32:
				return static_cast<float>(Interpolation::InterpolateTable1<int32_t>(x, _config->MaxXValue, _config->MinXValue, _config->XResolution, reinterpret_cast<const int32_t*>(_config->Table())));
			case VariableType::INT64:
				return static_cast<float>(Interpolation::InterpolateTable1<int64_t>(x, _config->MaxXValue, _config->MinXValue, _config->XResolution, reinterpret_cast<const int64_t*>(_config->Table())));
			case VariableType::FLOAT:
				return Interpolation::InterpolateTable1<float>(x, _config->MaxXValue, _config->MinXValue, _config->XResolution, reinterpret_cast<const float*>(_config->Table()));
			case VariableType::DOUBLE:
				return static_cast<float>(Interpolation::InterpolateTable1<double>(x, _config->MaxXValue, _config->MinXValue, _config->XResolution, reinterpret_cast<const double*>(_config->Table())));
		}
		return 0;
	}

	IOperationBase *Operation_LookupTable::Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
	{
		const Operation_LookupTableConfig *tableConfig = reinterpret_cast<const Operation_LookupTableConfig *>(config);
		sizeOut += tableConfig->Size();
		return new Operation_LookupTable(tableConfig);
	}
	
	IOPERATION_REGISTERFACTORY_CPP(Operation_LookupTable, 2, float, float)
}
#endif