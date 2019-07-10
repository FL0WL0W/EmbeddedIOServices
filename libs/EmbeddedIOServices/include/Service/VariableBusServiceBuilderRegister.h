#include "Service/ServiceBuilder.h"

#ifndef VARIABLEBUSSERVICEBUILDEREGISTER_H
#define VARIABLEBUSSERVICEBUILDEREGISTER_H

#define VARIABLE_BUS					6001
namespace Service
{
	class VariableBusServiceBuilderRegister
	{
	public:
		static void Register(ServiceBuilder *&serviceBuilder);
	};
}
#endif
