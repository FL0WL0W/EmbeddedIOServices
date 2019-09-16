#include "Service/HardwareAbstractionServiceBuilder.h"
#include "Service/OperationBusServiceBuilderIds.h"
#include "Service/VariableBusServiceBuilderIds.h"
#include "VariableBus/IVariableService.h"
#include "Operations/IOperation.h"
#include "stdint.h"
#include "Packed.h"
#include "Interpolation.h"
#include <tuple>

#define VARIABLESERVICE_OPERATION_H
namespace VariableBus
{
    template<typename RET, typename... PARAMS>
	class VariableService_Operation : public IVariableService
	{
	protected:
		Operations::IOperation<RET, PARAMS...> *_operation;
		RET *_variable;
		std::tuple<PARAMS*...> _params;

		template<std::size_t... Is>
		RET ExecuteWithTuplePointers(const std::tuple<PARAMS*...>& tuple,
			std::index_sequence<Is...>) {
			return _operation->Execute(*reinterpret_cast<PARAMS*>(std::get<Is>(tuple))...);
		}
	public:		
        VariableService_Operation(Operations::IOperation<RET, PARAMS...> *operation, RET *variable, PARAMS*... params)
		{
			_operation = operation;
			_variable = variable;
			_params = std::tuple<PARAMS*...>(params...);
		}

		void TranslateValue() override
		{
			*_variable = ExecuteWithTuplePointers(_params, std::index_sequence_for<PARAMS...>());
		}

		static IVariableService *Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
		{
			uint32_t variableId = Service::IService::CastAndOffset<uint16_t>(config, sizeOut);
			
			RET *variable = serviceLocator->LocateAndCast<RET>(BUILDER_VARIABLEBUS, variableId);
			if(variable == 0)
			{
				variable = (RET *)malloc(sizeof(RET));
				serviceLocator->Register(BUILDER_VARIABLEBUS, variableId, variable);
			}

			Operations::IOperation<RET, PARAMS...> *operation = serviceLocator->LocateAndCast<Operations::IOperation<RET, PARAMS...>>(BUILDER_OPERATIONBUS, Service::IService::CastAndOffset<uint16_t>(config, sizeOut));
			
			return new VariableService_Operation<RET, PARAMS...>(operation, variable, serviceLocator->LocateAndCast<PARAMS...>(BUILDER_VARIABLEBUS, Service::IService::CastAndOffset<uint16_t>(config, sizeOut)));
		}
		ISERVICE_REGISTERFACTORY_H_TEMPLATE
	};

	template<typename... PARAMS>
	class VariableService_Operation<void, PARAMS...> : public IVariableService
	{
	protected:
		Operations::IOperation<void, PARAMS...> *_operation;
		std::tuple<PARAMS*...> _params;

		template<std::size_t... Is>
		void ExecuteWithTuplePointers(const std::tuple<PARAMS*...>& tuple,
			std::index_sequence<Is...>) {
			_operation->Execute(*reinterpret_cast<PARAMS*>(std::get<Is>(tuple))...);
		}
	public:		
        VariableService_Operation(Operations::IOperation<void, PARAMS...> *operation, PARAMS*... params)
		{
			_operation = operation;
			_params = std::tuple<PARAMS*...>(params...);
		}

		void TranslateValue() override
		{
			ExecuteWithTuplePointers(_params, std::index_sequence_for<PARAMS...>());
		}

		static IVariableService *Create(Service::ServiceLocator * const &serviceLocator, const void *config, unsigned int &sizeOut)
		{
			Operations::IOperation<void, PARAMS...> *operation = serviceLocator->LocateAndCast<Operations::IOperation<void, PARAMS...>>(BUILDER_OPERATIONBUS, Service::IService::CastAndOffset<uint16_t>(config, sizeOut));
			
			return new VariableService_Operation<void, PARAMS...>(operation, serviceLocator->LocateAndCast<PARAMS...>(BUILDER_VARIABLEBUS, Service::IService::CastAndOffset<uint16_t>(config, sizeOut)));
		}
		ISERVICE_REGISTERFACTORY_H_TEMPLATE
	};
}
